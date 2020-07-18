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

#include "SourceHeader.h"

#include "Base/String.h"

#include <assert.h>
#include <fstream>
#include <iostream>

#include "parsing_helpers.h"

using namespace std;
using namespace Base;
using namespace FileSystem;

SourceHeader::SourceHeader(FileSystem::Path filename) :
  filename_(filename)
{
  cout << "scanning: " << filename.toString().c_str() << endl;
  if (!filename.fileExists())
    filename = "libs" / filename;
  if (!filename.fileExists())
  {
    cout << "ERROR: could not find file \"" << filename.toString().c_str() << endl;
    assert(filename.fileExists());
  }
  ifstream stream(filename.toString().c_str(), ifstream::in);
  while (!stream.eof())
  {
    string line_str;
    getline(stream, line_str);
    String line(line_str.c_str());
    if (!isQuoteInclude(line))
      continue;
    String file = getIncludeFile(line);
    if (headers_.containsKey(file))
      continue;
    headers_.add(file, Path(file));
  }
}

bool SourceHeader::getHTime(time_t& time)
{
    struct stat f_stat;
    if (stat(filename_.toString().c_str(), &f_stat) != 0)
      return false;
    time = f_stat.st_mtime;
    return true;
}