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
#include<iosfwd>
#include<vector>
#include<string>
#include<sstream>
#include<cmath>
#include"global.h"
#include"parser.h"
#include "LSQ.h"
#include"RAT.h"
#include"ROB.h"
#include"FU.h"
#include"RS.h"
#include"ARF.h"
#include"CDB.h"
#include"print.h"
#include"BRN.h"

inst_vec_t I_MEM;
float_vec_t D_MEM;

int main(int argc, char* argv[])
{



    std::string ifile;
    ifile = argv[1];

	// opens the input file
    std::ifstream file;
    file.open(ifile.c_str(), std::ios::in);

    //Check if data file is open and reports
    if(file.is_open())
      std::cout << "Inputfile ./" << ifile << " is open.\n";
      else
          std::cout << "Error opening " << ifile << ".\n";


	//start creating object for parser and all the hardware classes
    Parser parser;

    parser.parse_inputfile(file);

    Config configuration = parser.get_configs();

    I_MEM = parser.get_instr();
    D_MEM = configuration.init_mem;

    ROB rob(configuration.ROB_entries);
    RS int_add_rs(configuration.num_rs_of_int_adder);
    RS fp_add_rs(configuration.num_rs_of_fp_adder);
    RS fp_mul_rs(configuration.num_rs_of_fp_mul);

    RAT int_rat;
    RAT fp_rat;

    FU int_add_fu(INT_ADDER);
    FU fp_add_fu(FP_ADDER);
    FU fp_mul_fu(FP_MULTPLIER);

    ARF arf(configuration);
    arf.ini(configuration);

    LD_ST_QUEUE lsq(configuration.num_rs_of_ldst);
    CDB cdb;
    BRN br;
    print pr;

    int current_cycle = 0; //initial current cycle = 0;
    int PC = -1;//initial PC = 0;
    int num_commits = 0;
    int tot_inst = I_MEM.size();
    int left_inst = I_MEM.size();
    bool prog_end=false;
    bool br_flag=false;
    int i_num=-1;

    //single-cycle pipeline, each iteration for one cycle.

     while (prog_end == false&& current_cycle <200) {  //all the instructions commit, get from certain function of ROB to see if all the entries


        //in ROB have committed.

			std::cout<<std::endl;
			std::cout<<"--------------------------"<<"Cycle:"<<current_cycle<<"---------------------------------"<<std::endl;
			std::cout<<"Total Instructions:"<<tot_inst<<std::endl;
			std::cout<<"Left Instructions:"<<left_inst<<std::endl;



        //issue begins here:
        //BR control

if (rob.clear_check() && (((I_MEM[PC+1].inst_type == INT_ADD)&& int_add_rs.check_clear()) || ((I_MEM[PC+1].inst_type == I_ADD)&& int_add_rs.check_clear())
|| ((I_MEM[PC+1].inst_type == FLOAT_ADD)&& fp_add_rs.check_clear()) || ((I_MEM[PC+1].inst_type == FLOAT_MUL) && fp_mul_rs.check_clear()) ||
((I_MEM[PC+1].inst_type == LD)&& lsq.check_clear()) || ((I_MEM[PC+1].inst_type == ST)&& lsq.check_clear()) || ((I_MEM[PC+1].inst_type ==BR))) && br_flag == false && left_inst) {


		   PC=PC+1;
                   left_inst--;
		   i_num++;

	 //RS and LD_ST_QUEUE updates, by entering instructions

	pr.issue(I_MEM[PC], current_cycle);

	if (I_MEM[PC].inst_type == BR){

		br.entry(I_MEM[PC], int_rat.rat_copy(), arf.get_int_reg(), current_cycle, i_num);
		br_flag=true;
	}


       	if((I_MEM.at(PC).inst_type!=LD) && (I_MEM.at(PC).inst_type!=ST) && I_MEM[PC].inst_type != BR){
            if((I_MEM.at(PC).inst_type==INT_ADD) || (I_MEM.at(PC).inst_type==I_ADD)) {

                int_add_rs.add_entry(int_rat.rat_copy(), arf.get_int_reg(), I_MEM.at(PC), current_cycle, i_num);//:
       		}
            else if(I_MEM.at(PC).inst_type==FLOAT_ADD){

                fp_add_rs.add_entry(fp_rat.rat_copy(), arf.get_fp_reg(), I_MEM.at(PC), current_cycle, i_num);//:
        	}
            else{
	        fp_mul_rs.add_entry(fp_rat.rat_copy(), arf.get_fp_reg(), I_MEM.at(PC), current_cycle, i_num);
		}
        }


        else{
            lsq.add_entry(int_rat.rat_copy(), fp_rat.rat_copy(), arf.get_int_reg(), arf.get_fp_reg(), I_MEM.at(PC), current_cycle, i_num, configuration);
        }



	 //ROB entry made


        rob.add_entry(PC, i_num);


        //RAT updates, register renmaing taking places with rob index

       if((I_MEM.at(PC).inst_type==INT_ADD) || (I_MEM.at(PC).inst_type==I_ADD)){
            int_rat.rat_update(I_MEM.at(PC).dest_reg, rob.modify_dst());


        }
        else{
		if (I_MEM.at(PC).inst_type != ST && I_MEM.at(PC).inst_type != BR)   // If it is a store , there is no register renaming
         	fp_rat.rat_update(I_MEM.at(PC).dest_reg, rob.modify_dst());}

        }

      	// RS destination updated with the updates RAT

		if((I_MEM.at(PC).inst_type==INT_ADD) || (I_MEM.at(PC).inst_type==I_ADD))
			int_add_rs.update_dest(int_rat.rat_copy());

		if((I_MEM.at(PC).inst_type==FLOAT_ADD))
			fp_add_rs.update_dest(fp_rat.rat_copy());

		if((I_MEM.at(PC).inst_type==FLOAT_MUL))
			fp_mul_rs.update_dest(fp_rat.rat_copy());

		if((I_MEM.at(PC).inst_type == LD)||(I_MEM.at(PC).inst_type == ST))
			lsq.update_dest(fp_rat.rat_copy(), rob.modify_dst() );

		if (I_MEM.at(PC).inst_type == BR)
			br.get_rob_pos(rob.modify_dst());

	}



        //execute begins here:
			if (br.ready(current_cycle) && br_flag == true ){
			pr.exec_begin (br.get_inst_num(), current_cycle);
			if (br.execute() != 0){
			PC=PC+((br.execute())-1);
			left_inst += std::abs(br.execute()) + 1;
			tot_inst += std::abs(br.execute()) + 1;
			}
			rob.modify_value(br.return_rob(), -1, current_cycle);
			br_flag=false;
			}




        	//push an entry into FUs
           if(int_add_rs.push_ready(current_cycle)){
                int_add_fu.add_int_entry(int_add_rs.get_inst_name(), int_add_rs.get_inst_robdest(),
                        (int) int_add_rs.get_value1(), (int)  int_add_rs.get_value2(), (current_cycle), int_add_rs.get_inst_num(), configuration);

		        pr.exec_begin(int_add_rs.get_inst_num(), current_cycle);
                	int_add_rs.del_entry_pushed();

            }
            if(fp_add_rs.push_ready(current_cycle)){
                fp_add_fu.add_fp_entry(fp_add_rs.get_inst_name(), fp_add_rs.get_inst_robdest(),
                        fp_add_rs.get_value1(),  fp_add_rs.get_value2(), (current_cycle), fp_add_rs.get_inst_num() , configuration);

		        pr.exec_begin(fp_add_rs.get_inst_num(), current_cycle);
                	fp_add_rs.del_entry_pushed();

            }
            if(fp_mul_rs.push_ready(current_cycle)){
                fp_mul_fu.add_fp_entry(fp_mul_rs.get_inst_name(), fp_mul_rs.get_inst_robdest(),
                        fp_mul_rs.get_value1(),fp_mul_rs.get_value2(), (current_cycle), fp_mul_rs.get_inst_num() , configuration);

		        pr.exec_begin(fp_mul_rs.get_inst_num(), current_cycle);
		        fp_mul_rs.del_entry_pushed();

            }


            int add_pos=lsq.execute(current_cycle);   // address calculation for LS-QUEUE
		    if (add_pos !=-1){
		    	pr.exec_begin(add_pos, current_cycle);// print execute cycle for LOAD
		    }

            int_add_fu.int_compute(current_cycle);
            fp_add_fu.fp_compute(current_cycle);
            fp_mul_fu.fp_compute(current_cycle);

        //mem begins here:

        	lsq.mem(current_cycle);//TODO: print mem cycle for LOAD

	    // mem stage ends

        //write back begins here:

            if(lsq.lsq_wb_check()){
                cdb.push_to_cdb(lsq.write_back_rob(), lsq.write_back_result(), lsq.get_inst_index());
		pr.mem(lsq.get_inst_index(), current_cycle, lsq.get_fwd_match(), configuration.mem_cycle_of_ldst);
		lsq.delete_entry();
            }

            if(int_add_fu.check_wb()){
                cdb.push_to_cdb(int_add_fu.wb_rob_index_rq(), (float)int_add_fu.wb_int_rq(), int_add_fu.get_inst_index());
		pr.exec_end(int_add_fu.get_inst_index(), current_cycle);
		int_add_fu.fu_pop();
            }
            if(fp_add_fu.check_wb()){
                cdb.push_to_cdb(fp_add_fu.wb_rob_index_rq(), fp_add_fu.wb_fp_rq(), fp_add_fu.get_inst_index());
		pr.exec_end(fp_add_fu.get_inst_index(), current_cycle);
		fp_add_fu.fu_pop();
            }
            if(fp_mul_fu.check_wb()){
                cdb.push_to_cdb(fp_mul_fu.wb_rob_index_rq(), fp_mul_fu.wb_fp_rq(), fp_mul_fu.get_inst_index());
		pr.exec_end(fp_mul_fu.get_inst_index(), current_cycle);
		fp_mul_fu.fu_pop();
            }



		 for (int k=0; k<rob.ret_entry_num(); k++){

                    if (rob.get_finished(k)== true){

		        br.update(k, rob.get_result(k), current_cycle);
                	int_add_rs.update(k, rob.get_result(k), current_cycle);
                	fp_add_rs.update(k, rob.get_result(k), current_cycle);
                	fp_mul_rs.update(k, rob.get_result(k), current_cycle);
                	lsq.update(k, rob.get_result(k), current_cycle);
                    }
                }

	    if(!cdb.cdb_empty()){

		    pr.wb(cdb.get_inst_num(), current_cycle);

            rob.modify_value(cdb.get_cdb_robpos(),cdb.get_cdb_result(), current_cycle);

		for (int k=0; k<rob.ret_entry_num(); k++){

                    if (rob.get_finished(k)== true){

		        br.update(k, rob.get_result(k), current_cycle);
                	int_add_rs.update(k, rob.get_result(k), current_cycle);
                	fp_add_rs.update(k, rob.get_result(k), current_cycle);
                	fp_mul_rs.update(k, rob.get_result(k), current_cycle);
	                lsq.update(k, rob.get_result(k), current_cycle);
                    }
                }


                cdb.cdb_pop();

            }

       //commit begins here:

            int rob_commit_pos = rob.commit_and_return(current_cycle,lsq.get_st_update(), lsq.get_st_update_value());

	        if(rob_commit_pos!=-1){

	        //  br.update(rob_commit_pos, rob.get_result(rob_commit_pos), current_cycle);
	        //  int_add_rs.update(rob_commit_pos, rob.get_result(rob_commit_pos), current_cycle);
            //  fp_add_rs.update(rob_commit_pos, rob.get_result(rob_commit_pos), current_cycle);
            //  fp_mul_rs.update(rob_commit_pos, rob.get_result(rob_commit_pos), current_cycle);
            //  lsq.update(rob_commit_pos, rob.get_result(rob_commit_pos), current_cycle);

	        pr.commit(rob.get_inst_num(rob_commit_pos), current_cycle);
            	int int_true_dest = int_rat.rat_release(rob_commit_pos);
            	int fp_true_dest = fp_rat.rat_release(rob_commit_pos);

            	if(int_true_dest!=-1)
                	arf.commit(rob.get_result(rob_commit_pos),int_true_dest, rob.get_rob_type(rob_commit_pos));
            	if(fp_true_dest!=-1)
                	arf.commit(rob.get_result(rob_commit_pos),fp_true_dest, rob.get_rob_type(rob_commit_pos));

            	num_commits++;

	        if (num_commits == tot_inst)
		        prog_end=true;


            }
		 	rob.print(); // ROB printing for debugging.
			current_cycle++;
        }
  	for (int k=0; k<5; k++)
	     std::cout<<"\n"<<std::endl;

     std::cout<<"	     ----------------------- Timing Table -----------------------------"<<std::endl;
     	pr.print_out();

	for (int k=0; k<5; k++)
	     std::cout<<"\n"<<std::endl;

	 std::cout<<"	     ----------------------- Registers -----------------------------"<<std::endl;
			arf.print_out();

	for (int k=0; k<5; k++)
	     std::cout<<"\n"<<std::endl;
 std::cout<<"	     ----------------------- Data Mem -----------------------------"<<std::endl;

		for (int i=0; i<64; i++){
			if (D_MEM[i] != 0)
		std::cout<<"D_MEM["<<(i*4)<<"]"<<D_MEM[i]<<"\t";
	}

	file.close();


    return 0;
}
