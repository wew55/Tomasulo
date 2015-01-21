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


#ifndef RS_H
#define RS_H

#include<iostream>
#include<vector>
#include<string>

#include"global.h"

class RS{

	public:
    	RS (int cap);
    	bool check_clear();  // returns true if RS is clear for another insertion
    	void add_entry (int_vec_t RAT, int_vec_t ARF, INST i_entry, int, int inst_num);  // function to add entry in RS
    	void add_entry (int_vec_t RAT, float_vec_t ARF, INST i_entry, int, int inst_num);  // overloaded function to add entry in RS for floating point FUs
    	bool push_ready(int); // return true is there is any instruction ready to be pushed to FU

    	//use the following 5 functions only when push_ready returns true
    	int get_inst_num();
    	std::string get_inst_name(); // returns the name of the instruction to be pushed
        int get_inst_robdest();	     //  returns the destination in ROB, for the instruction to be executed
    	float get_value1();	// returns the first operand value of the instruction
    	float get_value2(); 	// returns second operand value for instruction
    	void del_entry_pushed(); // deletes the pushed entry from the RS
    	void update_dest(int_vec_t RAT); // updates the destination in RS

    	// description of the 5 instructions finished

    	void update(int rob_index, float result, int);  // updates the RS, replacing the tags with operand values when available

   	 private:
    	    struct rs_entry{
    		    std::string inst_name;
    		    int inst_num;
    		    int rob_dest;
    		    int tag1;
    		    int tag2;
    		    float value1;
    		    float value2;
    		    int val_ready_cycle;
    	    };

    	    std::vector<rs_entry> rs_vector;

            int rs_capacity;
        	int entry_count;
        	int push_pos;
       };

#endif
