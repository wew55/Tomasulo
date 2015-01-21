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


#ifndef PARSER_H
#define PARSER_H


#include<iostream>
#include<vector>
#include<string>

#include"global.h"


class Parser{
    public:
        Parser() :configs(),instr(){};
        ~Parser(){};
        Config get_configs();
        inst_vec_t get_instr();
        void parse_inputfile(std::ifstream&);

        //std::vector<INST>* get_instr(return);
    private:
        std::vector<INST> instr;
        Config configs;
};


#endif
