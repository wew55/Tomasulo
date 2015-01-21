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


#ifndef FU_H
#define FU_H

#include<iostream>
#include<vector>
#include<queue>
#include<string>

#include"global.h"

class FU{
    private:
    struct FUentry{
        std::string op;
        int rob_index;
        int inst_index;
        float fp_val_left;
        float fp_val_right;
        float fp_result;

        int int_val_left;
        int int_val_right;
        int int_result;

        int latency;
        int creation_cycle;
    };


        FUType type;
        int wait_entries;
        std::queue<FUentry> entry_queue;
        bool wb_rq;
        int wb_rob_index;
        int wb_inst_index;
        int wb_int_result;
        float wb_fp_result;


    public:
        FU(FUType);
       // ~FU();
        void add_int_entry(std::string, int, int, int, int, int, Config);
        void add_fp_entry(std::string, int, float, float, int, int, Config);

        void int_compute(int);
        void fp_compute(int);
        bool check_wb();
        int wb_int_rq();
        float wb_fp_rq();
        int wb_rob_index_rq();
	void fu_pop();
        void fu_print();//For debugging
        int get_inst_index();

};

#endif
