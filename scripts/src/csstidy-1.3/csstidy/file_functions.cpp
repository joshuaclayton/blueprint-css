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

#include "csspp_globals.hpp"
using namespace std;

string file_get_contents(const string filename)
{
	ifstream file_input(filename.c_str(),ios::binary);
    string line, file_contents = "";
    
    if(file_input.bad())
    {
		return "";
	}
	else
	{
	    while(file_input.good())
		{
			getline(file_input,line);
			file_contents += (line + "\n");
		}	    
	}
	file_input.close();

	return file_contents;
}

bool file_exists(const char *filename)
{
	ifstream file_input(filename);

	if(file_input.is_open())
	{
		file_input.close();
		return true;
	}
	
	file_input.close();
	return false;
}
