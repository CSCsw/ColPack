/************************************************************************************
    Copyright (C) 2005-2008 Assefaw H. Gebremedhin, Arijit Tarafdar, Duc Nguyen,
    Alex Pothen

    This file is part of ColPack.

    ColPack is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.GraphColoring::GraphColoring()

    ColPack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with ColPack.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************************/

define([COLPACK_VER], [1])
define([COLPACK_SUB], [0])
define([COLPACK_LVL], [8])


AC_INIT(ColPack, COLPACK_VER.COLPACK_SUB.COLPACK_LVL, agebreme@purdue.edu, ColPack, http://www.cscapes.org/coloringpage/)
#LT_INIT
AC_PREREQ(2.57)
AM_INIT_AUTOMAKE([1.10 no-define])
AC_CONFIG_HEADERS(config.h)
AC_PROG_CXX
AC_CONFIG_FILES(Makefile)
AC_PROG_LIBTOOL

AC_MSG_CHECKING(Build examples)
AC_ARG_ENABLE([examples],
     [  --enable-examples    Build examples],
     [case "${enableval}" in
       yes) examples=true ;;
       no)  examples=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-examples]) ;;
     esac],[examples=false])
     AM_CONDITIONAL([EXAMPLES], [test x$examples = xtrue])

AC_MSG_CHECKING(OpenMP)
AC_ARG_ENABLE([openmp],
     [  --enable-openmp    Enable OpenMP],
     [case "${enableval}" in
       yes) openmp=true ;;
       no)  openmp=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-openmp]) ;;
     esac],[openmp=false])
     AM_CONDITIONAL([ENABLE_OPENMP], [test x$openmp = xtrue])

AC_OUTPUT

# echo configuration
echo \
"
-----------------------------------------------------------------------------
Configuration:

  C compiler:                       ${CC}
  C++ compiler:                     ${CXX}
  Linker:                           ${LD}
  Source code location:             `pwd`
  Install path:                     ${prefix}
  
  CFLAGS:                           ${ac_colpack_cflags}
  CXXFLAGS:                         ${ac_colpack_cxxflags}

  Use OpenMP:                       ${openmp}
  Build examples:                   ${examples}
"

