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


#include "BRN.h"

void BRN::entry(INST i_entry, int_vec_t RAT, int_vec_t ARF, int curr_cycle, int i_num){

	inst_num=i_num;
	tag1= RAT[i_entry.dest_reg];
	tag2= RAT[i_entry.source_reg_left];
	offset= i_entry.source_reg_right;
	name = i_entry.inst_name;
	rob_index= -1;

	if (tag1== -1)
		val1= ARF[i_entry.dest_reg];
	if (tag2== -1)
		val2= ARF[i_entry.source_reg_left];

	if (tag1 == -1 && tag2 == -1)
		ready_cycle=curr_cycle;


}

void BRN::get_rob_pos(int rob_pos){

	rob_index= rob_pos;
}

int BRN::return_rob(){

	return rob_index;
}

int BRN::get_inst_num(){
	return inst_num;
}

 void BRN::update(int rob_pos, int value, int curr_cycle){


	std::cout<<"Branch updated: ROB POS:"<<rob_pos<<"	Value:"<<value<<std::endl;
	if (tag1 == rob_pos){
		val1= value;
		tag1 = -1;
		ready_cycle = curr_cycle;
	}

	if (tag2 == rob_pos){
		val2 = value;
		tag2 = -1;
		ready_cycle=curr_cycle;
	}
}

bool BRN::ready(int curr_cycle){

	bool ret_value=false;

	if (tag1 == -1 && tag2 == -1 && ready_cycle < curr_cycle)
		ret_value=true;

	return ret_value;
}

int BRN::execute(){

	if (name=="Beq"){

		if (val1 != val2)
			offset=0;
	}

	if (name == "Bne"){
		if (val1 == val2)
			offset=0;
	}

	return offset;
}





