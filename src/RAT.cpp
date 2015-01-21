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


#include "RAT.h"

RAT :: RAT()
{
    	rat.resize(32);
	std::fill(rat.begin(), rat.end(),-1);
}

int_vec_t RAT::rat_copy(){
	return (rat);
}


void RAT::rat_update(int true_reg, int map_rob){

	rat[true_reg]=map_rob;
}

int RAT::rat_release(int rob_pos){

	for (int i=0; i<32; i++){

		if (rat[i]==rob_pos){
			rat[i]=-1;
			return i;
		}

	}
    return (-1);
}



