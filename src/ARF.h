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



#ifndef ARF_H
#define ARF_H

#include<iostream>
#include<vector>
#include<string>

#include"global.h"


class ARF{
    private:
        float_vec_t fp_reg;
        int_vec_t int_reg;
    public:
        ARF(Config config);
        //~ARF();
        void ini(Config config);
        void commit(float result, int dst, ROBType rob_type);
        int_vec_t get_int_reg();
        float_vec_t get_fp_reg();

        int get_int_reg_value(int);
        float get_fp_reg_value(int);
	void print_out();

};


#endif
