#!/bin/bash

# Copyright (C) 2020 David Sloan
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, see <http://www.gnu.org/licenses/>.

set -e

if [[ -z "$1" ]]
then
  echo "Building"
  CLEAN="0"
else
  echo "Cleaning"
  CLEAN="1"
fi

ARGS="-g -std=c++11 -Wall"

GC="g++ $ARGS -I ../libs -I ../"

function make_o {
  if [[ -z ${OBJS+x} ]]
  then
    OBJS=$1/.obj/$2.o
  else
    OBJS="$OBJS $1/.obj/$2.o"
  fi
  echo "$1/$2.cpp"
  if [[ $CLEAN = "0" ]]
  then
    mkdir -p $1/.obj  
    $GC -o $1/.obj/$2.o -c $1/$2.cpp
  else
    rm -rf $1/.obj
  fi
}

make_o ../libs/Base String
make_o ../libs/Base Exception
make_o ../libs/FileSystem Path

make_o . ut_string
make_o . ut_list
make_o . ut_dictionary
make_o . ut_queue
make_o . ut_path

if [[ "$CLEAN" = "0" ]]
then
  $GC -o ut $OBJS ../libs/UT/UT.cpp
  valgrind -v --leak-check=full ./ut
else
  rm -f sourcebuild
fi