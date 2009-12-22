/*
    Copyright (c) 2009 Sascha Peilicke <sasch.pe@gmx.de>

    This application is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This application is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this application; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <vector>

bool log_init(const std::string &dir);
void log(const std::string &data_traits, const std::string &name, int size, unsigned int time_msecs, unsigned int comparison_count = -1);

template <typename T>
void print_vector(const std::vector<T> &before, const std::vector<T> &after, const std::string &line_prefix = "");

#endif // UTILITY_H
