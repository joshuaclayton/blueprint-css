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
 
#ifndef HEADER_CSS_OPT
#define HEADER_CSS_OPT 

// Color compression function. Converts all rgb() values to #-values and uses the short-form if possible. Also replaces color names and codes.
string cut_color(string color);

// Compresses shorthand values. Example: margin:1px 1px 1px 1px -> margin:1px
string shorthand(string value);

// Compresses numbers (ie. 1.0 -> 1 or 1.100 -> 1.1 
string compress_numbers(string subvalue, string property = "");

// Checks if the next word in a string from pos is a CSS property
bool property_is_next(string istring, const int pos);

// Compress font-weight
int c_font_weight(string& value);

// Merges selectors which have the same properties
void merge_selectors(sstore& input);

#endif // HEADER_CSS_OPT
