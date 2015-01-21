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


#ifndef GLOBAL_H
#define GLOBAL_H


#include<vector>
#include<string>

#define WORD 4

typedef std::vector<float> float_vec_t;
typedef std::vector<int> int_vec_t;

enum ROBType {I, F, NAR};//TODO: how many types of ROB?
enum FUType {INT_ADDER, FP_ADDER, FP_MULTPLIER};
enum RATType {RAT_I, RAT_F};
enum INSType {LD, ST, BR, INT_ADD, FLOAT_ADD, I_ADD, FLOAT_MUL};//TODO: to be added
enum LSType {L, S};


typedef struct INSTRUCTION{
    int inst_id;
    std::string inst_name;
    INSType inst_type;
    int source_reg_left;
    int source_reg_right;
    int dest_reg;

}INST;

typedef std::vector<INST> inst_vec_t;

typedef struct CONFIGURATION{

    int num_rs_of_int_adder;
    int num_rs_of_fp_adder;
    int num_rs_of_fp_mul;
    int num_rs_of_ldst;


    int ex_cycle_of_int_adder;
    int ex_cycle_of_fp_adder;
    int ex_cycle_of_fp_mul;
    int ex_cycle_of_ldst;

    int mem_cycle_of_ldst;

    int num_fu_of_int_adder;
    int num_fu_of_fp_adder;
    int num_fu_of_fp_mul;
    int num_fu_of_ldst;

    int ROB_entries;

    int_vec_t init_int_regs;
    float_vec_t init_fp_regs;
    float_vec_t init_mem;
}Config;
extern inst_vec_t I_MEM;
extern float_vec_t D_MEM;

#endif
