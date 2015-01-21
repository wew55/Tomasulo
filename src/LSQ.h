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


#ifndef LSQ_H
#define LSQ_H

#include<iostream>
#include<vector>
#include<string>

#include"global.h"



class LD_ST_QUEUE{
    private:
        struct LSQ_entry{
            INSType type;
            int rob_dest;
            float result;
            int inst_index;
            int tag2;
            float value2;
            int offset;
            int addr;
            int penalty;
            int val2_ready_cycle;
            int st_rob_pos;
            bool forward_ready;// forward ready only used for "S" type;
            bool fwd_match;
            bool commit_ready;
            bool committed;
            bool addr_calc;
            bool penalty_assigned;
            bool mem_access;
      };

        int qsize;
        int entry_count;
        int pos;
        int wb_inst_index;//MEM finished inst index;
        bool mem_occupy;


        float st_update_value;
        int st_update;

        std::vector<LSQ_entry> lsq;


    public:

        LD_ST_QUEUE(int);
        bool check_clear();
        void add_entry(int_vec_t int_rat, int_vec_t fp_rat, int_vec_t int_arf, float_vec_t fp_arf, INST i_entry, int curr_cycle, int, Config);
    	void update_dest(int_vec_t RAT, int);
        void update(int, float, int curr_cyle);
        int execute(int curr_cycle);
        void mem(int);
        int write_back_rob();
        float write_back_result();
    	bool lsq_wb_check();
       	void delete_entry();
    	void commit(int rob_pos);
    	bool get_fwd_match();
    	int get_inst_index();
        int get_st_update();
        float get_st_update_value();
};

#endif
