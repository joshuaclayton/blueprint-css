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
 
#ifndef HEADER_CSS_MISC
#define HEADER_CSS_MISC 

// Checks if a charcter is escaped
bool escaped(const string &istring, int pos);

// Returns a char of a string at pos but checks the string-length before
char s_at(const string &istring, int pos);

// Splits a string at e
vector<string> explode(const string e, string s, const bool check = false);

// Implodes a string at e
std::string implode(const string e, const vector<string> s);

// Replaces <find> with <replace> in <str>
string str_replace(const string find, const string replace, string str);

// Replaces all values of <find> with <replace> in <str>
string str_replace(const vector<string>& find, const string replace, string str);

// Checks if a string exists in a string-array
bool in_char_arr(const char* haystack, const char needle);
bool in_str_array(const string& haystack, const char needle);
bool in_str_array(const vector<string>& haystack, const string needle);

// Replaces certain chars with their entities
string htmlspecialchars(string istring, int quotes = 0);

// Rounds a float value
float round(const float &number, const int num_digits);

// Replacement for max (so that I don't have to include unnecessary things)
int max(const int i1, const int i2);

/* isspace() and isdigit() do not work correctly with UTF-8 strings */
bool ctype_space(const char c);
bool ctype_digit(const char c);
bool ctype_xdigit(char c);
bool ctype_alpha(char c);

/* Unserialise string arrays */
vector<string> unserialise_sa(const string istring);

/* Serialise a string */
string serialise_sa(const string istring);

#endif // HEADER_CSS_MISC
