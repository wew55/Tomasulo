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


#ifndef ROB_H
#define ROB_H

#include<iostream>
#include<vector>
#include<string>

#include"global.h"

class ROB{
    public:
        ROB(int cap);	// constructor, takes the ROB capacity as input
        void add_entry(int PC, int inst_num); // adds an entry to the ROB
        int modify_dst(); // returns the rob_entry number for register renaming
        //void delete_entry();
        void modify_value(int pos, float value, int t_cycle);  // updates the result field of the entry at 'pos', with 'value
        //void modify_finished();
        bool rob_pos_clear(int pos); // returns whether a particular entry in ROB is filled or can be written
	    bool clear_check(); // return true if the ROB is not completely full
	    int commit_and_return(int t_cycle, int, float); // returns the ROB index of the ROB entry getting commited, if there is any commit. ELSE return -1.
	    float get_result(int r_index); // returns the calculated result from a given rob_index
	    int get_inst_num(int r_index);
	    ROBType get_rob_type(int r_index);	// returns the rob_type (Integer or floating point) of the instruction at index=r_index
	    void print();
        int ret_entry_num();
        bool get_finished(int);
    private:
	    struct rob_entry{		// structure for each ROB entry
		    int instr_num;
		    int rob_entry_num; // rob_entry_number, used for register-renaming in RAT
		    ROBType rob_type;  // tells whether instruction is Integer, Float or Non-Arithmetic type
                INSType i_type;
		    int dst;	   // actual destination in ARF
		    float result;	   // stores result of the instruction
		    bool finished;     // tells whether the instruction is finished and ready to be committed
		    bool committed;	   // tells whether instruction is committed
		    int finish_cycle;  // records the time when the instruction is finished, used for adding the 4 cycle delay in commit for STORE

	};

	    std::vector<rob_entry> rob_vect; // vector or ROB entry structures
        int rob_capacity;  // stores the total ROB capacity
	    bool complete_fill_once; // indicates whether the ROB has been completely filled once or nor
	    int entry_count;  // keeping track of which position in ROB to enter the next entry
    	int commit_pos; // keeps the index of last committed position


};

#endif
