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

#include "Source.h"

#include <fstream>
#include <iostream>

#include "parsing_helpers.h"

using namespace std;
using namespace FileSystem;
using namespace Base;

Source::Source(Path const& file) :
  objPath_(""),
  objFile_("")
{
  Path filepath = file;
  cout << "scanning: " << file.toString().c_str() << endl;
  if (!filepath.fileExists())
  {
    cout << "ERROR: could not find file \"" << file.toString().c_str() << endl;
    assert(filepath.fileExists());
  }
  file_ = filepath;
  ifstream stream(filepath.toString().c_str(), ifstream::in);
  while (!stream.eof())
  {
    string line_str;
    getline(stream, line_str);
    String line(line_str.c_str());
    line = line.trim();
    if (isQuoteInclude(line)) {
      String incFile = getIncludeFile(line);
      Path incPath(incFile);
      if (!incPath.fileExists())
        incPath = "libs" / incPath;
      if (headers_.containsKey(incFile))
        continue;
      headers_.add(incFile, incPath);
    } else if (line.startsWith("#pragma")) {
      line = line.substring(7).trim();
      if (line.startsWith("git"))
      {
        line = line.substring(3).trim();
        GitRepo repo(line);
        repos_.add(repo);
      }
    }
  }
  if (file_.length() == 1)
  {
    objPath_ = Path(".obj");
    objFile_ = objPath_ / (file_ + 
#ifdef _MSC_VER
      ".obj");
#else
      ".o");
#endif
  }
  else
  {
    objPath_ = file_.subpath(0, file_.length() - 1) / ".obj/";
    objFile_ = objPath_ / (file_[-1] +
#ifdef _MSC_VER
    ".obj");
#else
    ".o");
#endif
  }
}

bool Source::objTime(time_t& time)
{
    struct stat f_stat;
    if (stat(objFile_.toString().c_str(), &f_stat) != 0)
      return false;
    time = f_stat.st_mtime;
    return true;
}

bool Source::cppTime(time_t& time)
{
    struct stat f_stat;
    if (stat(file_.toString().c_str(), &f_stat) != 0)
      return false;
    time = f_stat.st_mtime;
    return true;
}
