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






#include<iostream>
#include<vector>

#include "ARF.h"

ARF::ARF(Config config)
{
    int_reg.resize(config.init_int_regs.size());
    fp_reg.resize(config.init_fp_regs.size());
    std::fill(int_reg.begin(), int_reg.end(),0);
    std::fill(fp_reg.begin(), fp_reg.end(),0);
}

void ARF::ini(Config config)
{
    int_reg = config.init_int_regs;
    fp_reg = config.init_fp_regs;
}

void ARF::commit(float result, int dst, ROBType rob_type)
{
        switch (rob_type){
            case RAT_I:
                int_reg.at(dst) = (int)result;
                break;
            case RAT_F:
                fp_reg.at(dst) = result;
                break;
            case NAR:
                fp_reg.at(dst) = result;
                break;
        }
}

int_vec_t ARF::get_int_reg(){
    return int_reg;
}

float_vec_t ARF::get_fp_reg(){
    return fp_reg;
}

int ARF::get_int_reg_value(int _int_reg_index){
    return int_reg.at(_int_reg_index);
}

float ARF::get_fp_reg_value(int _fp_reg_index){
    return fp_reg.at(_fp_reg_index);
}

void ARF::print_out(){

	for (int i=0; i<32; i++){
        if (int_reg[i]!=0)
		std::cout<<"R["<<i<<"]"<<int_reg[i]<<"\t";
	}

	std::cout<<"\n\n\n";

	for (int i=0; i<32; i++){
        if (fp_reg[i]!=0)
		std::cout<<"F["<<i<<"]"<<fp_reg[i]<<"\t";
	}
}


