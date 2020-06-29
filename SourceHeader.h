/* Copyright (C) 2020 David Sloan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SourceHeader_h
#define __SourceHeader_h

#include <map>
#include <sys/stat.h>

#include "SourceProject.h"

class SourceProject;

class SourceHeader {
public:
  SourceHeader(const SourceHeader&);  
  SourceHeader& operator=(const SourceHeader&); 

  bool getHTime(time_t& time);

  ~SourceHeader() {}
private:
  friend class SourceProject;

  std::set<std::string> headers_;
  std::string filename_;

  SourceHeader(std::string header);
};

#endif//SourceHeader