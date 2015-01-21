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


#ifndef BR_H
#define BR_H

#include "global.h"
#include <string>
#include <iostream>
#include <vector>

class BRN{

	private:
	int inst_num;
	int rob_index;
	int offset;
	int tag1;
	int tag2;
	int val1;
	int val2;
	int ready_cycle;
	std::string name;

	public:
	void entry(INST i_entry, int_vec_t RAT, int_vec_t ARF, int curr_cycle, int i_num);
	void update(int rob_pos, int value, int curr_cycle);
	bool ready(int curr_cycle);
	int execute();
	void get_rob_pos(int);
	int return_rob();
	int get_inst_num();
};





#endif
