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

string strtolower(string istring)
{
	int str_size = istring.length();
	for(int i = 0; i < str_size; i++)
	{
		istring[i] = chartolower(istring[i]);
	}
	return istring;
}

char chartolower(const char c)
{
	switch(c)
	{
		case 'A': return 'a';
		case 'B': return 'b';
		case 'C': return 'c';
		case 'D': return 'd';
		case 'E': return 'e';
		case 'F': return 'f';
		case 'G': return 'g';
		case 'H': return 'h';
		case 'I': return 'i';
		case 'J': return 'j';
		case 'K': return 'k';
		case 'L': return 'l';
		case 'M': return 'm';
		case 'N': return 'n';
		case 'O': return 'o';
		case 'P': return 'p';
		case 'Q': return 'q';
		case 'R': return 'r';
		case 'S': return 's';
		case 'T': return 't';
		case 'U': return 'u';
		case 'V': return 'v';
		case 'W': return 'w';
		case 'X': return 'x';
		case 'Y': return 'y';
		case 'Z': return 'z';
		default: return c;
	}
}

string strtoupper(string istring)
{
	int str_size = istring.length();
	for(int i = 0; i < str_size; i++)
	{
		istring[i] = chartoupper(istring[i]);
	}
	return istring;
}

char chartoupper(const char c)
{
	switch(c)
	{
		case 'a': return 'A';
		case 'b': return 'B';
		case 'c': return 'C';
		case 'd': return 'D';
		case 'e': return 'E';
		case 'f': return 'F';
		case 'g': return 'G';
		case 'h': return 'H';
		case 'i': return 'I';
		case 'j': return 'J';
		case 'k': return 'K';
		case 'l': return 'L';
		case 'm': return 'M';
		case 'n': return 'N';
		case 'o': return 'O';
		case 'p': return 'P';
		case 'q': return 'Q';
		case 'r': return 'R';
		case 's': return 'S';
		case 't': return 'T';
		case 'u': return 'U';
		case 'v': return 'V';
		case 'w': return 'W';
		case 'x': return 'X';
		case 'y': return 'Y';
		case 'z': return 'Z';
		default: return c;
	}
}

/* Didn't find any usable function for this, so here is my version :) */
string dechex(const int i)
{
	stringstream sstream;
	sstream << hex << i;
	return sstream.str();
}

double hexdec(string istring)
{
	double ret = 0;
	istring = trim(istring);
	for(int i = istring.length()-1; i >= 0; --i)
	{
		int num = 0;
		switch(tolower(istring[i]))
		{
			case 'a': num = 10; break;
			case 'b': num = 11; break;
			case 'c': num = 12; break;
			case 'd': num = 13; break;
			case 'e': num = 14; break;
			case 'f': num = 15; break;
			case '1': num = 1; break;
			case '2': num = 2; break;
			case '3': num = 3; break;
			case '4': num = 4; break;
			case '5': num = 5; break;
			case '6': num = 6; break;
			case '7': num = 7; break;
			case '8': num = 8; break;
			case '9': num = 9; break;
			case '0': num = 0; break;
		}
		ret += num*pow((double) 16, (double) istring.length()-i-1);
	}
	return ret;
}

string f2str(const float f)
{
	stringstream sstream;
	sstream.flags (ios::fixed);
	sstream << f;
	string converted = sstream.str();
	// now strip remaining zeros at the end
	std::string::size_type last = converted.find_last_not_of("0"); /// must succeed
	converted = converted.substr( 0, last + 1);
	// and the dot if no zeros remain
	last = converted.find_last_not_of("."); /// must succeed
	converted = converted.substr( 0, last + 1);
	return converted;
}

float str2f(const string istring)
{
	return atof(istring.c_str());
}

string char2str(const char c)
{
	string ret = "";
	ret += c;
	return ret;
}

string char2str(const char *c)
{
	stringstream sstream;
	sstream << c;
	return sstream.str();
}

