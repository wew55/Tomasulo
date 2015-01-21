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
#include<string>
#include<queue>
#include "FU.h"

FU::FU(FUType _type){
    if(_type==INT_ADDER){
        type = INT_ADDER;
    }
    if(_type==FP_ADDER){
        type = FP_ADDER;
    }
    if(_type==FP_MULTPLIER){
        type = FP_MULTPLIER;
    }
    wait_entries = 0;
    wb_rq = false;
    wb_int_result =-1;
    wb_inst_index =-1;
    wb_fp_result =-1;
}

void FU::add_int_entry(std::string _op, int _rob_index,
        int _val_left, int _val_right, int _current_cycle, int _inst_index, Config _config){

    FUentry new_entry;
    new_entry.op = _op;
    new_entry.inst_index = _inst_index;
    new_entry.rob_index = _rob_index;
    new_entry.int_val_left = _val_left;
    new_entry.int_val_right = _val_right;
    new_entry.latency = _config.ex_cycle_of_int_adder;
    new_entry.creation_cycle = _current_cycle;

    new_entry.fp_val_left = -1;
    new_entry.fp_val_right = -1;

    new_entry.int_result = -1;
    new_entry.fp_result = -1;

    entry_queue.push(new_entry);
    wait_entries++;

}

void FU::add_fp_entry(std::string _op, int _rob_index,
        float _val_left, float _val_right, int _current_cycle, int _inst_index, Config _config){

    FUentry new_entry;
    new_entry.op = _op;
    new_entry.rob_index = _rob_index;
    new_entry.inst_index = _inst_index;
    new_entry.fp_val_left = _val_left;
    new_entry.fp_val_right = _val_right;

    if(new_entry.op == "Add.d" || new_entry.op == "Sub.d"){
        new_entry.latency = _config.ex_cycle_of_fp_adder;
	std::cout<<"Add.d latency:"<<new_entry.latency<<std::endl;
    }
    if(new_entry.op == "Mult.d"){
        new_entry.latency = _config.ex_cycle_of_fp_mul;
    }
    new_entry.creation_cycle = _current_cycle;

    new_entry.int_val_left = -1;
    new_entry.int_val_right = -1;

    new_entry.int_result = -1;
    new_entry.fp_result = -1;

    entry_queue.push(new_entry);
    wait_entries++;

}

void FU::int_compute(int _current_cycle){


    //update pipeline
    if(wait_entries){
    //FUentry entry_queue.front() = entry_queue.front();
        if(entry_queue.front().latency == (_current_cycle - entry_queue.front().creation_cycle)){
            //execute
            if(entry_queue.front().op == "Add")
            {
            entry_queue.front().int_result = ( entry_queue.front().int_val_left + entry_queue.front().int_val_right);
            }
            if(entry_queue.front().op == "Sub")
            {
            entry_queue.front().int_result = ( entry_queue.front().int_val_left - entry_queue.front().int_val_right);
            }
            if(entry_queue.front().op == "Addi")
            {
            entry_queue.front().int_result = ( entry_queue.front().int_val_left + entry_queue.front().int_val_right);
            }


            wb_rq = true;
            wb_rob_index = entry_queue.front().rob_index;
            wb_int_result = entry_queue.front().int_result;
            wb_inst_index = entry_queue.front().inst_index;
            //entry_queue.pop();
            wait_entries--;
        }
        else{
            wb_rq = false;
        }

    }
    else{
        wb_rq = false;
    }

}

void FU::fp_compute(int _current_cycle){


    //update pipeline
    if(wait_entries){
    //FUentry entry_queue.front() = entry_queue.front();
        if(entry_queue.front().latency == (_current_cycle - entry_queue.front().creation_cycle)){
            //execute
            if(entry_queue.front().op == "Add.d")
            {
            entry_queue.front().fp_result = ( entry_queue.front().fp_val_left + entry_queue.front().fp_val_right);
            }
            if(entry_queue.front().op == "Sub.d")
            {
            entry_queue.front().fp_result = ( entry_queue.front().fp_val_left - entry_queue.front().fp_val_right);
            }
            if(entry_queue.front().op == "Mult.d")
            {
            entry_queue.front().fp_result = ( entry_queue.front().fp_val_left * entry_queue.front().fp_val_right);
            }

            wb_rq = true;
            wb_rob_index = entry_queue.front().rob_index;
            wb_fp_result = entry_queue.front().fp_result;
            wb_inst_index = entry_queue.front().inst_index;

            //entry_queue.pop();
            wait_entries--;
        }
        else{
            wb_rq = false;
        }

    }
    else{
        wb_rq = false;
    }

}

bool FU::check_wb(){
	std::cout<<"Checking WB of FU:	" << wb_rq<<std::endl;
    return wb_rq;
}

int FU::wb_int_rq(){
    return wb_int_result;
}

float FU::wb_fp_rq(){
    return wb_fp_result;
}

int FU::wb_rob_index_rq(){
    return wb_rob_index;
}

int FU::get_inst_index(){
    return wb_inst_index;
}

void FU::fu_pop(){
    entry_queue.pop();

}

//fu_print is not used.
void FU::fu_print(){

    std::cout<<"FUType:"<<type<<std::endl;
    std::cout<<"Waiting entries:"<<wait_entries<<std::endl;
    std::cout<<"If_Write_Back_Rq:"<<wb_rq<<std::endl;
    std::cout<<"Write_Back_ROB#:"<<wb_rob_index<<std::endl;
    std::cout<<"Write_Back_Value:"<<wb_int_result<<std::endl;
    std::cout<<"Write_Back_Value:"<<wb_fp_result<<std::endl;

    std::cout<<"Write_Back_Value:"<<wb_fp_result<<std::endl;

}
