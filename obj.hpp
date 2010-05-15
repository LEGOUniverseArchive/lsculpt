/*	libobj: A modern C++ library for parsing the Wavefront OBJ file format.

  Copyright (C) 2007 Ares Lagae <ares.lagae@cs.kuleuven.be>

  http://www.cs.kuleuven.ac.be/~ares/libobj/

  This file (obj.hpp) is part of libobj.

  libobj is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  libobj is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/  */

#ifndef OBJ_OBJ_HPP_INCLUDED
#define OBJ_OBJ_HPP_INCLUDED

#include <cstddef>
#include <tr1/tuple>

namespace obj {

typedef std::size_t size_type;
typedef std::ptrdiff_t index_type;
typedef double float_type;
typedef std::tr1::tuple<index_type, index_type> index_2_tuple_type;
typedef std::tr1::tuple<index_type, index_type, index_type> index_3_tuple_type;

} // namespace obj

#endif // OBJ_OBJ_HPP_INCLUDED
