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
#include<fstream>
#include<vector>
#include<string>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>

#include"parser.h"


void Parser::parse_inputfile(std::ifstream& file){

    int instruction_id = 0;
    char line[1024];
    bool alu_config_parsing = 0;
    bool mem_ini_parsing = 0;
    bool assembly_parsing = 0;

    int parsing_line_status=0;


    std::cout<<"############### Parsing Start #################"<<std::endl;

    while (file.getline(line,1024)){


        std::cout<<"Parsing line #: "<<parsing_line_status<<std::endl;
        parsing_line_status++;

        if (line == NULL || line[0] == ' ')
            continue;

        if(!strcmp(line, "#alu.config")){
            alu_config_parsing = 1;
            mem_ini_parsing = 0;
            assembly_parsing = 0;

            std::cout<<"Parsing alu.config start:"<<std::endl;
            continue;
        }

        if(!strcmp(line, "#mem.ini")){
            alu_config_parsing = 0;
            mem_ini_parsing = 1;
            assembly_parsing = 0;
            std::cout<<"Parsing mem.ini start:"<<std::endl;
            continue;
        }




        if(!strcmp(line, "#assembly")){
            alu_config_parsing = 0;
            mem_ini_parsing = 0;
            assembly_parsing = 1;
            std::cout<<"Parsing assembly start:"<<std::endl;
            continue;
        }

        if(alu_config_parsing){
        //Alu_config Parsing
        char* alu_config_token;
        std::vector<char*> alu_config_line;

        alu_config_token = std::strtok(line, " .");

        while(alu_config_token != NULL){
            alu_config_line.push_back(alu_config_token);
            alu_config_token = std::strtok (NULL," .");
        }

        std::string alu_config_line_0 = std::string(alu_config_line[0]);
        std::string alu_config_line_1 = std::string(alu_config_line[1]);


        if(alu_config_line_0 == "Integer"&& alu_config_line_1=="adder"){
            configs.num_rs_of_int_adder = atoi(alu_config_line[2]);
            configs.ex_cycle_of_int_adder = atoi(alu_config_line[3]);
            configs.num_fu_of_int_adder = atoi(alu_config_line[4]);

            std::cout<<"Integer_adder's_rs_# = "<<configs.num_rs_of_int_adder<<std::endl;
            std::cout<<"Integer_adder's_ex_cycle_# = "<<configs.ex_cycle_of_int_adder<<std::endl;
            std::cout<<"Integer_adder's_fu_# = "<<configs.num_fu_of_int_adder<<std::endl;
        }

        if(alu_config_line_0 == "FP"&& alu_config_line_1=="adder"){
            configs.num_rs_of_fp_adder = atoi(alu_config_line[2]);
            configs.ex_cycle_of_fp_adder = atoi(alu_config_line[3]);
            configs.num_fu_of_fp_adder = atoi(alu_config_line[4]);

            std::cout<<"FP_adder's_rs_# = "<<configs.num_rs_of_fp_adder<<std::endl;
            std::cout<<"FP_adder's_ex_cycle # = "<<configs.ex_cycle_of_fp_adder<<std::endl;
            std::cout<<"FP_adder's_fu_#_= "<<configs.num_fu_of_fp_adder<<std::endl;
        }

        if(alu_config_line_0 == "FP"&& alu_config_line_1=="multiplier"){
            configs.num_rs_of_fp_mul = atoi(alu_config_line[2]);
            configs.ex_cycle_of_fp_mul = atoi(alu_config_line[3]);
            configs.num_fu_of_fp_mul = atoi(alu_config_line[4]);

            std::cout<<"FP_mul's_rs_# = "<<configs.num_rs_of_fp_mul<<std::endl;
            std::cout<<"FP_mul's_ex_cycle_# = "<<configs.ex_cycle_of_fp_mul<<std::endl;
            std::cout<<"FP_mul's_fu_#_= "<<configs.num_fu_of_fp_mul<<std::endl;
        }

        if(alu_config_line_0 == "Load/store"&& alu_config_line_1=="unit"){
            configs.num_rs_of_ldst = atoi(alu_config_line[2]);
            configs.ex_cycle_of_ldst = atoi(alu_config_line[3]);
            configs.mem_cycle_of_ldst = atoi(alu_config_line[4]);
            configs.num_fu_of_ldst = atoi(alu_config_line[5]);

            std::cout<<"FP_ld/st's_rs_# = "<<configs.num_rs_of_ldst<<std::endl;
            std::cout<<"FP_ld/st's_ex_cycle_# = "<<configs.ex_cycle_of_ldst<<std::endl;
            std::cout<<"FP_ld/st's_mem_cycle_# = "<<configs.mem_cycle_of_ldst<<std::endl;
            std::cout<<"FP_ld/st's_fu_# = "<<configs.num_fu_of_ldst<<std::endl;
        }

        }

        if(mem_ini_parsing){
        //Mem_ini Parsing
        //Naive parser, to be improved.
        char* mem_ini_token;
        std::vector<char*> mem_ini_line;
        mem_ini_token = std::strtok(line," ,=()[]");

        while(mem_ini_token != NULL){

            mem_ini_line.push_back(mem_ini_token);
            mem_ini_token = std::strtok (NULL," ,=()[]");
        }

        if(std::string(mem_ini_line[0])=="ROB" && std::string(mem_ini_line[1])=="entries"){
            configs.ROB_entries = atoi(mem_ini_line[2]);

            std::cout<<"ROB_entries_# = "<<configs.ROB_entries<<std::endl;
        }
        else if(std::string(mem_ini_line[0])=="Mem"){

            configs.init_mem.resize(64, 0);
            std::cout<<"Memory initialization>> "<<std::endl;

            for(std::vector<char*>::size_type i = 0; i!=mem_ini_line.size();i=i+3){
                //if(std::string(mem_ini_line[i])=="Mem")
                //    continue;

                configs.init_mem.at(atoi(mem_ini_line[i+1])) = atof(mem_ini_line[i+2]);

            std::cout<<"Memory initialization>> "<<"index:"<<atoi(mem_ini_line[i+1])<<" "<<"value:"<<atof(mem_ini_line[i+2])<<std::endl;
            }



        }
        else{

            configs.init_int_regs.resize(32, 0);
            configs.init_fp_regs.resize(32, 0);

            for(std::vector<char*>::size_type i = 0; i!=mem_ini_line.size();i=i+2){

                if(strchr(mem_ini_line[i],'R')!=NULL){

                    memmove(mem_ini_line[i],mem_ini_line[i]+1, strlen(mem_ini_line[i]));
                configs.init_int_regs.at(atoi(mem_ini_line[i])) = atoi(mem_ini_line[i+1]);

            std::cout<<"Integer Register initialization>> "<<"index:"<<atoi(mem_ini_line[i])<<" "<<"value:"<<atoi(mem_ini_line[i+1])<<std::endl;

                }
                if(strchr(mem_ini_line[i],'F')!=NULL){
                    memmove(mem_ini_line[i],mem_ini_line[i]+1, strlen(mem_ini_line[i]));
                configs.init_fp_regs.at(atoi(mem_ini_line[i])) = atof(mem_ini_line[i+1]);

            std::cout<<"FP Register initialization>> "<<"index:"<<atoi(mem_ini_line[i])<<" "<<"value:"<<atof(mem_ini_line[i+1])<<std::endl;
                }
            }

        }


        }


        if(assembly_parsing){
        //Assembly Parsing
        char* token;
        std::vector<char*> instr_line;
        INST add_inst;

;

        token = std::strtok(line," RF,()");

        while(token != NULL){

            instr_line.push_back(token);
            token = std::strtok (NULL," RF,()");
        }

        add_inst.inst_id = instruction_id;

        add_inst.inst_name = std::string(instr_line[0]);

        //Lowercase to Uppercase letters

        if(add_inst.inst_name == "add")
            add_inst.inst_name = "Add";
        if(add_inst.inst_name == "sub")
            add_inst.inst_name = "Sub";
        if(add_inst.inst_name == "add.d")
            add_inst.inst_name = "Add.d";
        if(add_inst.inst_name == "sub.d")
            add_inst.inst_name = "Sub.d";
        if(add_inst.inst_name == "addi")
            add_inst.inst_name = "Addi";
        if(add_inst.inst_name == "mult.d")
            add_inst.inst_name = "Mult.d";
        if(add_inst.inst_name == "sd")
            add_inst.inst_name = "Sd";
        if(add_inst.inst_name == "ld")
            add_inst.inst_name = "Ld";
        if(add_inst.inst_name == "beq")
            add_inst.inst_name = "Beq";
        if(add_inst.inst_name == "bne")
            add_inst.inst_name = "Bne";


        //ALU Instructions Parsing

        if(add_inst.inst_name == "Add"|| add_inst.inst_name == "Sub"){
            add_inst.dest_reg = atoi(instr_line[1]);
            add_inst.source_reg_left = atoi(instr_line[2]);
            add_inst.source_reg_right = atoi(instr_line[3]);
            add_inst.inst_type = INT_ADD;


    }

        if(add_inst.inst_name == "Add.d"|| add_inst.inst_name == "Sub.d"){
            add_inst.dest_reg = atoi(instr_line[1]);
            add_inst.source_reg_left = atoi(instr_line[2]);
            add_inst.source_reg_right = atoi(instr_line[3]);
            add_inst.inst_type = FLOAT_ADD;

    }

        if(add_inst.inst_name == "Addi"){
            add_inst.dest_reg = atoi(instr_line[1]);
            add_inst.source_reg_left = atoi(instr_line[2]);
            add_inst.source_reg_right = atoi(instr_line[3]);
            add_inst.inst_type = I_ADD;
        }
        if(add_inst.inst_name == "Mult.d"){
            add_inst.dest_reg = atoi(instr_line[1]);
            add_inst.source_reg_left = atoi(instr_line[2]);
            add_inst.source_reg_right = atoi(instr_line[3]);
            add_inst.inst_type = FLOAT_MUL;
        }
        if(add_inst.inst_name == "Ld"){
            add_inst.dest_reg = atoi(instr_line[1]);
            add_inst.source_reg_left = atoi(instr_line[2]);
            add_inst.source_reg_right = atoi(instr_line[3]);
            add_inst.inst_type = LD;
        }

        if(add_inst.inst_name == "Sd"){
            add_inst.dest_reg = atoi(instr_line[1]);
            add_inst.source_reg_left = atoi(instr_line[2]);
            add_inst.source_reg_right = atoi(instr_line[3]);
            add_inst.inst_type = ST;
        }

        if(add_inst.inst_name == "Beq"|| add_inst.inst_name == "Bne"){
            add_inst.dest_reg = atoi(instr_line[1]);
            add_inst.source_reg_left = atoi(instr_line[2]);
            add_inst.source_reg_right = atoi(instr_line[3]);
            add_inst.inst_type = BR;

        }
        std::cout<<"Read assembly instructions>> "<<"id:"<<instruction_id<<" "<<"instr_name:"<<add_inst.inst_name<<" ";
        std::cout<<"Dest_reg: "<<add_inst.dest_reg<<" ";
        std::cout<<"Source_reg_left: "<<add_inst.source_reg_left<<" ";
        std::cout<<"Source_reg_right: "<<add_inst.source_reg_right<<std::endl;


        instr.push_back(add_inst);
        instruction_id++;
        }
    }

    std::cout<<"############### Parsing Complete #################"<<std::endl;

  }

Config Parser::get_configs(){
    return configs;
}

inst_vec_t Parser::get_instr(){
    return instr;

}
