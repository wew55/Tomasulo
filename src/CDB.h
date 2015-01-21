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


#ifndef CDB_H
#define CDB_H

#include<iostream>
#include<string>
#include<queue>

#include"global.h"


class CDB{
    private:
	    struct CDB_queue{

		    int rob_pos;  // stores ROB destination for each entry
		    float result; // stores result for each entry
		    int inst_num;

	    };

	    std::queue<CDB_queue> cdb_queue;

    public:
	    void push_to_cdb(int rob_index, float fu_result, int i_num); // pushing an entry of CDB
	    bool cdb_empty() ; //returns true if CDB is empty

	    //the following 2 functions should be used only if CDB is not empty (i.e., cdb_empty() returns false.
	    int get_cdb_robpos();  // getting the ROB destination postion of the first entry
	    float get_cdb_result();	    // getting the result of the first entry
	    int get_inst_num();
	    //description of the 2 functions over

	    void cdb_pop(); // popping the CDB entry after ROB and RS has been updated;

};

#endif
