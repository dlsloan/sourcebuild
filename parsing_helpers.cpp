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

string ltrim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
        return !isspace(ch);
    }));
    return s;
}

string rtrim(string s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), s.end());
    return s;
}

string trim(string s) {
    s = ltrim(s);
    s = rtrim(s);
    return s;
}

bool startsWith(string const& str, string const& start)
{
  if (str.length() < start.length())
    return false;
  else
    return str.substr(0, start.length()) == start;
}

bool isQuoteInclude(string s)
{
  s = trim(s);
  if (!startsWith(s, "#include"))
    return false;
  s = trim(s.substr(8, s.length() - 8));
  if (s.length() < 2)
    return false;
  return s.at(0) == '\"' && s.at(s.length() - 1) == '\"';
}

string getIncludeFile(string s)
{
  s = trim(s);
  assert(startsWith(s, "#include"));
  s = trim(s.substr(8, s.length() - 8));
  assert(s.length() > 2);
  return s.substr(1, s.length() - 2);
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
