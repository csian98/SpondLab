#include <iostream>
#include <string>
#include <string_view>

#include "sessa.hpp"

/* Copyright (C) 
 * 2023 - sian.Choi
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */
 
/**
 * @file main.cpp
 * @brief //
 * Build random composition Atoms' XPS with sessa.exe(NIST)
 * and Training deep neural models with those data.
 *
 * KHU SpondLab
 * @author sian.Choi
 * @version 3.0.0
 * @date 2023-01-26
 */

int main(void){
  signal(SIGUSR1, Sessa::writeStatus);

  // Build Instance of Sessa to initialize
  Sessa sessa;
  
  // each sequence of 1) data Build and 2) deep neural training
  // progress with functor( Sessa::operator()  ) or method( Sessa::dataWrite() )
  while( sessa() ){}
  
  return 0;

  // process use diverse resources that require to be free
  // In case the process stops abnormally,
  // in particular, the shared memory must be freed directly.
}
