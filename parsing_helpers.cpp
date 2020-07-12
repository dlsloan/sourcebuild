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

#include "parsing_helpers.h"

#include "Base/adhoc_stdint.h"

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <sys/stat.h>

using namespace std;
using namespace Base;

bool isQuoteInclude(String s)
{
  s = s.trim();
  if (!s.startsWith("#include"))
    return false;
  s = s.substring(8, s.length() - 8).trim();
  if (s.length() < 2)
    return false;
  return s[0] == '\"' && s[-1] == '\"';
}

String getIncludeFile(String s)
{
  s = s.trim();
  assert(s.startsWith("#include"));
  s = s.substring(8, s.length() - 8).trim();
  assert(s.length() > 2);
  return s.substring(1, s.length() - 2);
}

bool fileExists(const string& name) {
    ifstream f(name.c_str());
    return f.good();
}

int lastIndexOf(string const& str, char ch)
{
    for (int i = str.length() - 1; i >= 0; --i) {
      if (str.at(i) == ch) {
        return static_cast<int>(i);
      }
    }
    return -1;
}

int indexOf(string const& str, char ch)
{
    for (size_t i = 0; i < str.length(); ++i) {
      if (str.at(i) == ch) {
        return static_cast<int>(i);
      }
    }
    return -1;
}

string trimExtension(string s) {
    int index = lastIndexOf(s, '.');
    if (index == -1)
      return s;
    else 
      return s.substr(0, index);
}

bool getFileTime(string name, time_t& time)
{
    struct stat f_stat;
    if (stat(name.c_str(), &f_stat) != 0)
      return false;
    time = f_stat.st_mtime;
    return true;
}
