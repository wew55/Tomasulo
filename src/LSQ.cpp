/* Tomasulo Algorithm Simulator simulated the different components
 * inside the Out of Order processor, and the main function will call
 * them in a cycle-based while-loop. The order of calling for
 * different component is based on the pipeline stage of “issue”,
 * “execute”, “memory”, “write back” and “commit”.
 *
 * Copyright (C) 2014-2015 Wen Wen (wew55@pitt.edu),
 *                         Joydeep Rakshit (jor73@pitt.edu),
 *                         Lu Zhang (luz14@pitt.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.*/


#include "LSQ.h"

LD_ST_QUEUE::LD_ST_QUEUE(int _qsize) // constructor, sets the size  of the LD/ST-queue
{
    qsize=_qsize;
    lsq.resize(_qsize);
    pos = -1;
    entry_count=0;
    mem_occupy = false;
    st_update = 0;
    st_update_value = 0;

}

bool LD_ST_QUEUE::check_clear(){  // checks whether the queue is free or not
    if(entry_count <= qsize)
        return true;
    else
        return false;
}



void LD_ST_QUEUE::add_entry(int_vec_t int_rat, int_vec_t fp_rat, int_vec_t int_arf, float_vec_t fp_arf, INST i_entry, int curr_cycle, int _inst_index, Config _config) // for entry to the LSQ
{
	LSQ_entry temp;
 	int actual_dest=i_entry.dest_reg;
	temp.offset=i_entry.source_reg_left;
	int actual_source2=i_entry.source_reg_right;
	temp.val2_ready_cycle=-1;
	temp.st_rob_pos=-1;
    	temp.inst_index = _inst_index;
    	temp.fwd_match=false;
    	
    	if(i_entry.inst_type == LD){ // if the instruction is a LD

		temp.type = i_entry.inst_type;

		temp.tag2=int_rat[actual_source2];  // this is for the register used for address calculation
		temp.rob_dest=actual_dest; //fp_rat[actual_dest];  // gets the destination address from the ROB, i.e where in the ROB the value is to be sent


	if (int_rat[actual_source2]==-1){
		temp.val2_ready_cycle=curr_cycle;	    // if the integer register is  ready with value, get it for address calculation
		temp.value2=int_arf[actual_source2];
	}

	else
		temp.value2=0;

    	temp.forward_ready = false;
    	temp.commit_ready = false;
    	temp.committed = false;
    	temp.result = 0; // value to be obtained from the memory and sent to ROB
    	temp.addr = -1;
    	temp.penalty = _config.mem_cycle_of_ldst;
    	temp.penalty_assigned=false;
    	temp.addr_calc = false;
    	temp.mem_access=false;
    	lsq[entry_count]=temp;
    	entry_count++;
    }
    else{//if ST

	temp.type = i_entry.inst_type;

	temp.tag2=int_rat[actual_source2];  // address calculation register
	temp.rob_dest = fp_rat[actual_dest];    // ST doesn't have to send its value to any ROB index, hence use as a tag.

	if (int_rat[actual_source2]==-1){
	        temp.val2_ready_cycle=curr_cycle;	// this is for the register used for address calculation
		temp.value2=int_arf[actual_source2];
	}

	else
		temp.value2=0;

	if (fp_rat[actual_dest]==-1)  // actual_dest is the register from which the value is to be stored to the memory. if the value is available, load it on result
		temp.result=fp_arf[actual_dest];
	else
		temp.result=-1;

	temp.forward_ready = false;
    	temp.commit_ready = false;
    	temp.committed = false;
    	temp.addr =-1;
    	temp.penalty = 0;
    	temp.penalty_assigned=false;
    	temp.addr_calc = false;
    	temp.mem_access=false;
    	lsq[entry_count]=temp;

    	entry_count++;

    }

}


void LD_ST_QUEUE::update_dest(int_vec_t RAT, int rob_pos){

	if (lsq[entry_count-1].type == LD)
	lsq[entry_count-1].rob_dest=RAT[lsq[entry_count-1].rob_dest];
	if (lsq[entry_count-1].type == ST)
	lsq[entry_count-1].st_rob_pos = rob_pos;
}





void LD_ST_QUEUE::update(int rob_index, float _result, int curr_cycle){   // function to update the RS tag fields with values
	for (int i=0; i<entry_count; i++){
        if(lsq[i].type == LD){

		if ((lsq[i].tag2) == rob_index){
			lsq[i].value2 = _result;
			lsq[i].tag2 = -1;
			lsq[i].val2_ready_cycle	= curr_cycle;
		}

        }

        if(lsq[i].type == ST){

		if ((lsq[i].rob_dest) == rob_index){
			lsq[i].result = _result;
			lsq[i].rob_dest = -1;
			lsq[i].val2_ready_cycle=curr_cycle;


		}
		if ((lsq[i].tag2) == rob_index){
			lsq[i].value2 = _result;
			lsq[i].tag2 = -1;
			lsq[i].val2_ready_cycle=curr_cycle;
		}

	}
}
}

int LD_ST_QUEUE::execute(int curr_cycle){    // this function returns if there is any entry ready to be pushed to FU

	for (int i=0; i<entry_count; i++){
        if(lsq[i].type == LD){

            if ((lsq[i].tag2)==-1 && (lsq[i].val2_ready_cycle < curr_cycle) && lsq[i].addr_calc == false){
            //address calculation begins
            lsq[i].addr = lsq[i].offset + lsq[i].value2;
            lsq[i].addr_calc = true;
            return lsq[i].inst_index;//return executed inst index
            //break;//only one addr calculation per cycle;
		}

        }
        if(lsq[i].type == ST){

            if ((lsq[i].tag2)==-1 && (lsq[i].val2_ready_cycle < curr_cycle && lsq[i].addr_calc == false)){ //&& lsq[i].rob_dest == -1){ //&& lsq[i].val2_ready_cycle < curr_cycle){
            //address calculation begins
            lsq[i].addr = lsq[i].offset + lsq[i].value2;
            lsq[i].forward_ready = true;

            lsq[i].addr_calc = true;
            return lsq[i].inst_index;//return executed inst index
            //break;//only one addr calculation per cycle;
		}

        }
	}
    return -1;//No entry is ready to execute then return -1 as executed inst index.


}

void LD_ST_QUEUE::mem(int curr_cycle){

    //mem stage is from oldest to yougest entry
    for(int i=0;i<entry_count;i++){
        if(lsq[i].type == ST &&lsq[i].rob_dest == -1 &&lsq[i].tag2 == -1 && ((i==0 && (lsq[i].val2_ready_cycle +1) < curr_cycle) ||
                (i!=0 && lsq[i].val2_ready_cycle <curr_cycle ))) {
        	 //D_MEM[lsq[i].addr] = lsq[i].result;
        	  st_update = lsq[i].addr;
        	  st_update_value = lsq[i].result;

            if(lsq[i].penalty == 0){
		lsq[i].commit_ready = true;
	    }

        }
        if(lsq[i].type == LD && lsq[i].addr_calc==true && ((i==0 && (lsq[i].val2_ready_cycle +1) < curr_cycle) ||
                (i!=0 && lsq[i].val2_ready_cycle <curr_cycle ))){
            //All LDs come after earlier STs are forward_ready
            bool all_st_forward_ready = true;
            for(int j=0;j<i;j++){
                if(lsq[j].type == ST && lsq[j].forward_ready == false)
                    all_st_forward_ready = false;
            }

            if(all_st_forward_ready){
                bool forward_match = false;
                for(int k=i-1;k>=0;k--){
                    if(lsq[i].addr == lsq[k].addr && lsq[k].type == ST){
                        lsq[i].result = lsq[k].result;
                        forward_match = true;
			lsq[i].fwd_match=true;
                        break;
                    }
                }
                //if forward match
                if(forward_match== true && lsq[i].penalty_assigned == false) {
                        lsq[i].penalty = 1;
                        //
			lsq[i].penalty_assigned = true;

                }
                 else//if forward not match
                    {
                        if(mem_occupy == false){
                        lsq[i].result = D_MEM[lsq[i].addr];
                        mem_occupy = true;
                        lsq[i].mem_access = true;
                        }
                    }
                 if(lsq[i].mem_access == true){

                        lsq[i].penalty--;
                 }
            }

            if(lsq[i].penalty == -1){
                mem_occupy=false;
                lsq[i].commit_ready = true; // might be a problem here.
		}

        }

    }

}

bool LD_ST_QUEUE::get_fwd_match(){

	if (pos != -1)
	return (lsq[pos].fwd_match);

return (false);
}


bool LD_ST_QUEUE::lsq_wb_check(){

	bool ret_value=false;
	for (int i=0; i<entry_count; i++){

		if (lsq[i].commit_ready==true){ //&& lsq[i].committed==false){
            		ret_value=true;
        		wb_inst_index=lsq[i].inst_index;
            		mem_occupy = false;
		}
	}

	return ret_value;
}

int LD_ST_QUEUE::write_back_rob(){
	    for(int i=0;i<entry_count;i++){

        if(lsq[i].type == LD && lsq[i].commit_ready == true && lsq[i].committed == false){
        	pos=i;
            	return lsq[i].rob_dest;
        }

	if(lsq[i].type == ST && lsq[i].commit_ready == true && lsq[i].committed == false){
            	pos=i;
            	return lsq[i].st_rob_pos;
	}
    }
    return -1;

}

float LD_ST_QUEUE::write_back_result(){

    for(int i=0;i<entry_count;i++){
        if(lsq[i].type == LD && lsq[i].commit_ready == true && lsq[i].committed == false){
	        pos=i;
        	return lsq[i].result;
        }
    }
    return -1;

}

void LD_ST_QUEUE::commit(int rob_pos){

	lsq[rob_pos].committed=true;
}



void LD_ST_QUEUE::delete_entry(){

	if (pos!=-1){//&& lsq[pos].type==LD){

		//lsq.erase(lsq.begin()+pos);
    		for (int i=pos; i<(entry_count-1); i++){
        	lsq[i]=lsq[i+1];}
		pos=-1;
		entry_count--;
		//lsq.resize(qsize);
	}

}

int LD_ST_QUEUE::get_inst_index(){//Mem finished inst index
    return wb_inst_index;
}

int LD_ST_QUEUE::get_st_update(){//Mem finished inst index
    return st_update;
}

float LD_ST_QUEUE::get_st_update_value(){//Mem finished inst index
    return st_update_value;
}

