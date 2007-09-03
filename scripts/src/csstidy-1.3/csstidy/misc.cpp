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

bool escaped(const string &istring, const int pos) 
{
	return !(s_at(istring,pos-1) != '\\' || escaped(istring,pos-1));
}

// Save replacement for .at()
char s_at(const string &istring, const int pos)
{
	if(pos > (istring.length()-1) && pos < 0)
	{
		return 0;
	}
	else
	{
		return istring[pos];
	}
}

vector<string> explode(const string e,string s, const bool check)
{
	vector<string> ret;
	int iPos = s.find(e, 0);
	int iPit = e.length();
	
	while(iPos > -1)
	{
		if(iPos != 0 || check)
		{
			ret.push_back(s.substr(0,iPos));
		}
		s.erase(0,iPos+iPit);
		iPos = s.find(e, 0);
	}
	
 	if(s != "" || check)
 	{
		ret.push_back(s);
	}
	return ret;
}

string implode(const string e,const vector<string> s)
{
	string ret;
	for(int i = 0; i < s.size(); i++)
	{
		ret += s[i];
		if(i != (s.size()-1)) ret += e;
	}
	return ret;
}

float round(const float &number, const int num_digits)
{
    float doComplete5i, doComplete5(number * powf(10.0f, (float) (num_digits + 1)));
    
    if(number < 0.0f)
        doComplete5 -= 5.0f;
    else
        doComplete5 += 5.0f;
    
    doComplete5 /= 10.0f;
    modff(doComplete5, &doComplete5i);
    
    return doComplete5i / powf(10.0f, (float) num_digits);
}


string str_replace(const string find, const string replace, string str)
{
    int len = find.length();
    int replace_len = replace.length();
    int pos = str.find(find);

    while(pos != string::npos)
	{  
        str.replace(pos, len, replace);
        pos = str.find(find, pos + replace_len);
    }
    return str;
}

string str_replace(const vector<string>& find, const string replace, string str)
{
	int replace_len = replace.length();
	
	for(int i = 0; i < find.size(); ++i)
	{
	    int len = find[i].length();
	    int pos = str.find(find[i]);
	
	    while(pos != string::npos)
		{  
	        str.replace(pos, len, replace);
	        pos = str.find(find[i], pos + replace_len);
	    }
	}
    return str;
}


bool in_char_arr(const char* haystack, const char needle)
{
	for(int i = 0; i < strlen(haystack); ++i)
	{
		if(haystack[i] == needle)
		{
			return true;
		}
	}
	return false;
}

bool in_str_array(const string& haystack, const char needle)
{
	return (haystack.find_first_of(needle,0) != string::npos);
}

bool in_str_array(const vector<string>& haystack, const string needle)
{
	for(int i = 0; i < haystack.size(); ++i)
	{
		if(haystack[i] == needle)
		{
			return true;
		}
	}
	return false;
}

string htmlspecialchars(string istring, int quotes)
{
	istring = str_replace("&","&amp;",istring);
	istring = str_replace("<","&lt;",istring);
	istring = str_replace(">","&gt;",istring);
	if(quotes > 0) istring = str_replace("\"","&quot;",istring);
	if(quotes > 1) istring = str_replace("'","&#039;",istring);
	return istring;
}

int max(const int i1, const int i2)
{
	if(i1 > i2)
	{
		return i1;
	}
	else
	{
		return i2;
	}
}

bool ctype_space(const char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == 11);
}

bool ctype_digit(const char c)
{
	return (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9');
}

vector<string> unserialise_sa(const string istring)
{
	int strlen = istring.length();
	int strpos = 0;
	vector<string> ret;
	
	while(strlen > 0)
	{
		string digit_tmp = "";
		for(int i = strpos; ctype_digit(s_at(istring,i)); i++)
		{
			digit_tmp += istring[i];
			--strlen; ++strpos;
		}
		// :
		--strlen; ++strpos;
		
		int next_length = static_cast<int>(str2f(digit_tmp));
		next_length += strpos;

		string string_tmp = "";
		for(int i = strpos; (i<istring.length() && i < next_length); i++)
		{
			string_tmp += istring[i];
			--strlen; ++strpos;
		}
		ret.push_back(string_tmp);
	}
	return ret;
}

string serialise_sa(const string istring)
{
	return f2str(istring.length()) + ":" + istring;
}

bool ctype_xdigit(char c)
{
	c = chartolower(c);
	return (ctype_digit(c) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f');
}

bool ctype_alpha(char c)
{
	c = chartolower(c);
	return (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h' || c == 'i' || c == 'j' || 
	   c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'o' || c == 'p' || c == 'q' || c == 'r' || c == 's' || c == 't' || 
	   c == 'u' || c == 'v' || c == 'w' || c == 'x' || c == 'y' || c == 'z');
}
