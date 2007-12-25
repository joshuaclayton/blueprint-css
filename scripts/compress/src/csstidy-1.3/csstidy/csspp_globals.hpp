/*
 * This file is part of CSSTidy.
 *
 * CSSTidy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CSSTidy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CSSTidy; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
#define CSSTIDY_VERSION "1.3"
#include <cstdlib>
#include <string> 
#include <iterator>
#include <vector>
#include <math.h>
#include <time.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

#include "umap.hpp"
#include "datastruct.hpp"

#include "trim.hpp"
#include "conversions.hpp"
#include "misc.hpp"
#include "important.hpp"
#include "file_functions.hpp"

#include "cssopt.hpp"
#include "csstidy.hpp"

#include "parse_css.hpp"
#include "background.hpp"
