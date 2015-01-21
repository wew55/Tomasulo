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


#ifndef PRINT_H
#define PRINT_H

#include<iostream>
#include<vector>
#include<string>

#include"global.h"

class print{

    public:

    private:
	struct print_entry{
	int pos;
	INST inst;
	int issue;
	int beg_exec;
	int end_exec;
	int mem_beg;
	int mem_end;
	int wb;
	int commit;
	};

	int entry_count;

	std::vector<print_entry> print_vect;

     public:
	print();
	void issue(INST i_entry, int curr_cycle);
	void exec_begin(int print_pos, int curr_cycle);
	void exec_end(int print_pos, int curr_cycle);
	void mem(int print_pos, int curr_cycle, bool fwd_match, int);
	void wb(int print_pos, int curr_cycle);
	void commit(int print_pos, int curr_cycle);
	void print_out();
};
#endif
