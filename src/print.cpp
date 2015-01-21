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


#include "print.h"

print::print(){
	entry_count=0;
	print_vect.resize(1000);
}

void print::issue(INST i_entry, int curr_cycle){

	print_entry temp;
	temp.pos = entry_count;
	temp.inst = i_entry;
	temp.issue = curr_cycle;
	temp.beg_exec = 0;
	temp.end_exec = 0;
	temp.mem_beg=0;
	temp.mem_end=0;
	temp.wb = 0;
	temp.commit = 0;

	print_vect[entry_count]=temp;
	entry_count++;

}

void print::exec_begin(int print_pos, int curr_cycle){

	if (print_vect[print_pos].beg_exec == 0 ){
	std::cout<<"Exec pos:"<<print_pos<< " Initial value:"<<print_vect[print_pos].beg_exec<<std::endl;
	print_vect[print_pos].beg_exec=curr_cycle;
	if (print_vect[print_pos].inst.inst_type == ST || print_vect[print_pos].inst.inst_type == LD || print_vect[print_pos].inst.inst_type == BR ){
		print_vect[print_pos].end_exec= curr_cycle;
	}
}

}

void print::exec_end(int print_pos, int curr_cycle){

	print_vect[print_pos].end_exec= (curr_cycle-1);
}

void print::mem(int print_pos, int curr_cycle, bool fwd_match, int mem_latency){

	if (print_vect[print_pos].inst.inst_type == LD){
		print_vect[print_pos].mem_end = (curr_cycle -1 );
		if (fwd_match == true){
			std::cout<<"FWD match check:"<<fwd_match<<std::endl;
			print_vect[print_pos].mem_beg=  (print_vect[print_pos].mem_end);
		}
		else
			print_vect[print_pos].mem_beg=  (print_vect[print_pos].mem_end)-(mem_latency-1);
	}
}


void print::wb(int print_pos, int curr_cycle){

	if (print_vect[print_pos].inst.inst_type != ST)
		print_vect[print_pos].wb= curr_cycle;
}

void print::commit(int print_pos, int curr_cycle){
	print_vect[print_pos].commit= curr_cycle;
}
void print::print_out(){

	std::cout<<"INSTRUCTIONS       ISSUE        EXEC(BEG)     EXEC(END)     MEM(BEG)     MEM(END)      WB    COMMIT"<<std::endl;

	for (int i=0; i<entry_count; i++){

		// printing the instructions first
		std::string name=print_vect[i].inst.inst_name;

		if (name == "Beq" || name == "Bne" || name == "Addi")
std::cout<< name <<" R"<< print_vect[i].inst.dest_reg <<", R"<< print_vect[i].inst.source_reg_left << ", "<<print_vect[i].inst.source_reg_right<<"       ";
		if (name == "Add" || name == "Sub")
     std::cout<< name <<" R"<< print_vect[i].inst.dest_reg <<", R"<< print_vect[i].inst.source_reg_left << ", R"<< print_vect[i].inst.source_reg_right<<"       ";
		if (name == "Add.d" || name == "Sub.d" || name == "Mult.d")
     std::cout<< name <<" F"<< print_vect[i].inst.dest_reg <<", F"<< print_vect[i].inst.source_reg_left << ", F"<< print_vect[i].inst.source_reg_right<<"     ";
		if (name == "Ld" || name == "Sd")
     std::cout<< name <<" F"<< print_vect[i].inst.dest_reg <<", "<< print_vect[i].inst.source_reg_left << " (R"<< print_vect[i].inst.source_reg_right<<")        ";


		//printing out cycles
		std::cout<<print_vect[i].issue;
		std::cout<< "      	     "<<print_vect[i].beg_exec;
		std::cout<< "            "<<print_vect[i].end_exec;

		if (print_vect[i].mem_beg == 0)
			std::cout<< "      	--" ;
		else
			std::cout<< "      	"<<print_vect[i].mem_beg;

		if (print_vect[i].mem_end == 0)
			std::cout<< "           --" ;
		else
			std::cout<< "            "<<print_vect[i].mem_end;


		if (print_vect[i].wb == 0)
			std::cout<< "      	--" ;
		else
			std::cout<< "      	"<<print_vect[i].wb;


		std::cout<< "       "<<print_vect[i].commit<<std::endl;
	}
}


