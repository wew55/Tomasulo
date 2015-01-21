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


# include "ROB.h"


ROB::ROB(int cap){

	rob_capacity = cap;
	rob_vect.resize(rob_capacity);
	entry_count=0;
	complete_fill_once=false;
	commit_pos=0;
}

void ROB::add_entry(int PC, int inst_num){


		INST inst_entry = I_MEM[PC];
		rob_entry temp;
		temp.instr_num= inst_num;
		temp.rob_entry_num=entry_count;
		temp.dst=inst_entry.dest_reg;
		temp.result=0.0;
		temp.finished=false;
		temp.committed=false;
		temp.finish_cycle=-1;
		INSType i_temp=inst_entry.inst_type;
		temp.i_type=i_temp;
		if ((i_temp==LD)||(i_temp==ST)||(i_temp==FLOAT_ADD)||(i_temp==FLOAT_MUL))
			temp.rob_type=F;
		else if ((i_temp==INT_ADD)||(i_temp==I_ADD))
			temp.rob_type=I;
		else
			temp.rob_type=NAR;

		rob_vect[entry_count]= temp;

		entry_count++;
		if (entry_count==rob_capacity){
		       	entry_count=0;
			complete_fill_once=true;
		}

}
int ROB::ret_entry_num(){

    return entry_count;
}


int ROB::modify_dst(){

	return ((entry_count-1));
}

void ROB::modify_value(int pos, float value, int t_cycle){

	rob_vect[pos].result=value;
	rob_vect[pos].finished=true;
	rob_vect[pos].finish_cycle=t_cycle; // stores the time when the instruction is finished
}

bool ROB::rob_pos_clear(int pos){

	bool ret_value=false;

	if ((complete_fill_once ==true) && (rob_vect[pos].committed==false))
		ret_value=false;
	else if ((complete_fill_once ==true) && (rob_vect[pos].committed==true))
		ret_value=true;
	else
		ret_value=true;
	return ret_value;
}

bool ROB::clear_check(){

	bool ret_value=false;

	for (int i=0; i<rob_capacity; i++){

		if (rob_pos_clear(i)){
			ret_value=true;
			break;
		}
	}
	return ret_value;
}

int ROB::commit_and_return(int t_cycle, int _st_update, float _st_update_value){

	int c_pos=-1;
	if (rob_vect[commit_pos].i_type == ST){

		if (rob_vect[commit_pos].finished){

			if ((rob_vect[commit_pos].finish_cycle)==(t_cycle-5)){  // 1 extra cycle to go from WB to commit stage
                		c_pos=commit_pos;
        			D_MEM[_st_update] = _st_update_value;
                		rob_vect[commit_pos].committed=true;
                		commit_pos++;
			}
		}
	}

	else{
		if (rob_vect[commit_pos].finished && (t_cycle > rob_vect[commit_pos].finish_cycle)){
			c_pos=commit_pos;
			rob_vect[commit_pos].committed=true;
			commit_pos++;
		}
	}

	if (commit_pos==rob_capacity)
	{commit_pos=0;}

	return c_pos;
}

float ROB::get_result(int r_index){

	return (rob_vect[r_index].result);

}

bool ROB::get_finished(int r_index){
    return (rob_vect[r_index].finished);
}

int ROB::get_inst_num(int r_index){
	return (rob_vect[r_index].instr_num);
}


ROBType ROB::get_rob_type(int r_index){
	return (rob_vect[r_index].rob_type);
}

void ROB::print(){

for (int i=0;i<entry_count; i++){

	std::cout<<"ROB Index:"<<rob_vect[i].rob_entry_num<<"	ROB Type:"
        <<rob_vect[i].rob_type<<"	Instruction type:"<<rob_vect[i].i_type
        <<"	ARF Destination:"<<rob_vect[i].dst<<"	ROB result:"<<rob_vect[i].result
        <<"	ROB finished bit:"<<rob_vect[i].finished<<"	ROB committed bit:"
        <<rob_vect[i].committed<<"	ROB finish cycle:"<<rob_vect[i].finish_cycle<< std::endl;
}
}

