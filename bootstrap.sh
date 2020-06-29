#!/bin/bash

set -e

ARGS="-g -std=c++11 -Wall"

GC="g++ $ARGS -I libs"

function make_o {
  if [[ -z ${OBJS+x} ]]
  then
    OBJS=$1/.obj/$2.o
  else
    OBJS="$OBJS $1/.obj/$2.o"
  fi
  mkdir -p $1/.obj
  echo "$1/$2.cpp"
  $GC -o $1/.obj/$2.o -c $1/$2.cpp
}

make_o libs/Base Char
make_o libs/Base Exception
make_o libs/Base String

make_o libs/FileSystem FileStream
make_o libs/FileSystem Path

make_o libs/IO BaseStream
make_o libs/IO ReadStream
make_o libs/IO StdIO
make_o libs/IO StreamException
make_o libs/IO TextStream
make_o libs/IO WriteStream

make_o . parsing_helpers
make_o . Source
make_o . SourceHeader
make_o . SourceProject

$GC -o sourcebuild $OBJS SourceBuild.cpp

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
