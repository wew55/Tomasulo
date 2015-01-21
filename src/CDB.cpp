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


#include "CDB.h"

void CDB::push_to_cdb(int rob_index, float fu_result, int i_num){

	CDB_queue temp;
	temp.rob_pos=rob_index;
	temp.result=fu_result;
	temp.inst_num=i_num;

	cdb_queue.push(temp);
}

bool CDB::cdb_empty(){

	return (cdb_queue.empty());
}


int CDB::get_cdb_robpos(){

	return (cdb_queue.front().rob_pos);
}

float CDB::get_cdb_result(){
	return (cdb_queue.front().result);
}

int CDB::get_inst_num(){
	return (cdb_queue.front().inst_num);
}

void CDB::cdb_pop(){
	cdb_queue.pop();

}


