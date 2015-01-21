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


#include "RS.h"

RS::RS (int cap){

	rs_capacity=cap;
	rs_vector.resize(cap);
	entry_count=0;
	push_pos=-1;
}

bool RS::check_clear(){

	bool ret_value=false;

	if (entry_count <= rs_capacity)
		ret_value=true;
	return ret_value;
}

void RS::add_entry (int_vec_t RAT, int_vec_t ARF, INST i_entry, int curr_cycle, int i_num){ // function takes ARF, RAT and the instruction to be inserted as input

	int actual_dest=i_entry.dest_reg;
	int actual_source1=i_entry.source_reg_left;

	int actual_source2=i_entry.source_reg_right;
	std::string name=i_entry.inst_name;

	rs_entry temp;
	temp.inst_name=name;
	temp.inst_num=i_num;
	temp.val_ready_cycle=-1;
	temp.tag1=RAT[actual_source1];
	temp.tag2=RAT[actual_source2];
	temp.rob_dest=actual_dest;//RAT[actual_dest];


	if (RAT[actual_source1]==-1){
		temp.value1=ARF[actual_source1];
		temp.val_ready_cycle=curr_cycle;
	}

	else
		temp.value1=0;


	if (i_entry.inst_type == I_ADD){
		temp.value2=actual_source2;
		temp.tag2=-1;
		temp.val_ready_cycle=curr_cycle;
	}
	else{

	if (RAT[actual_source2]==-1){
		temp.value2=ARF[actual_source2];
		temp.val_ready_cycle=curr_cycle;
	}
	else
		temp.value2=0;
	}

	rs_vector[entry_count]=temp;
	entry_count++;
}

void RS::add_entry (int_vec_t RAT, float_vec_t ARF, INST i_entry, int curr_cycle, int i_num){ // function takes ARF, RAT and the instruction to be inserted as input

	int actual_dest=i_entry.dest_reg;
	int actual_source1=i_entry.source_reg_left;
	int actual_source2=i_entry.source_reg_right;
	std::string name=i_entry.inst_name;

	rs_entry temp;
	temp.inst_name=name;
	temp.val_ready_cycle=-1;
	temp.inst_num = i_num;
	temp.tag1=RAT[actual_source1];
	temp.tag2=RAT[actual_source2];
	temp.rob_dest=actual_dest;//RAT[actual_dest];

	if (RAT[actual_source1]==-1){
		temp.value1=ARF[actual_source1];
		temp.val_ready_cycle=curr_cycle;
	}
	else
		temp.value1=0;

	if (RAT[actual_source2]==-1){
		temp.value2=ARF[actual_source2];
		temp.val_ready_cycle=curr_cycle;
	}
	else
		temp.value2=0;

	rs_vector[entry_count]=temp;
	entry_count++;
}


void RS::update_dest(int_vec_t RAT){

	rs_vector[entry_count-1].rob_dest=RAT[rs_vector[entry_count-1].rob_dest];
}



bool RS::push_ready(int curr_cycle){    // this function returns if there is any entry ready to be pushed to FU

	bool ret_value=false;
	for (int i=0; i<entry_count; i++){
		if ((rs_vector[i].tag1)==-1 && (rs_vector[i].tag2)==-1 && (rs_vector[i].val_ready_cycle < curr_cycle)){
			push_pos=i;  // sets the position in the RS to be pushed to CDB
			ret_value=true;
			break;
		}
	}
    return ret_value;
}

// five functions to send the required fields to the FUs. Use this only if push_ready=true
int RS::get_inst_num(){
	return (rs_vector[push_pos].inst_num);
}

std::string RS::get_inst_name(){
	return (rs_vector[push_pos].inst_name);
}
int RS::get_inst_robdest(){
	return (rs_vector[push_pos].rob_dest);
}
float RS::get_value1(){
	return (rs_vector[push_pos].value1);
}
float RS::get_value2(){
	return (rs_vector[push_pos].value2);
}

void RS::del_entry_pushed(){

	rs_vector.erase(rs_vector.begin()+push_pos);
	push_pos=-1;
	entry_count--;
	rs_vector.resize(rs_capacity);

}


// five functions declared above for sending required fields to FUs

void RS::update(int rob_index, float result, int curr_cycle){   // function to update the RS tag fields with values

	for (int i=0; i<entry_count; i++){

		if ((rs_vector[i].tag1) == rob_index){
			rs_vector[i].value1 =result;
			rs_vector[i].tag1 = -1;
			rs_vector[i].val_ready_cycle=curr_cycle;
		}

		if ((rs_vector[i].tag2) == rob_index){
			rs_vector[i].value2 =result;
			rs_vector[i].tag2 = -1;
			rs_vector[i].val_ready_cycle=curr_cycle;
		}
	}
}

























