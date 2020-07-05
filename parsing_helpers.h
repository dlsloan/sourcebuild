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

#include <string>
#include <time.h>

std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string trim(std::string s);
bool startsWith(std::string const& str, std::string const& start);
bool isQuoteInclude(std::string s);
std::string getIncludeFile(std::string s);
bool fileExists(const std::string& name);
int lastIndexOf(std::string const& str, char ch);
int indexOf(std::string const& str, char ch);
std::string trimExtension(std::string s);
bool getFileTime(std::string name, time_t& time);
