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

extern vector<string> unit_values,color_values;
extern map<string,string> replace_colors,all_properties;

string shorthand(string value)
{
	string important = "";
	
	if(is_important(value))
	{
		value = gvw_important(value);
		important = "!important";
	}
	
	vector<string> values = explode(" ",value);
	switch(values.size())
	{
		case 4:
		if(values[0] == values[1] && values[0] == values[2] && values[0] == values[3])
		{
			return values[0] + important;
		}
		else if(values[1] == values[3] && values[0] == values[2])
		{
			return values[0] + " " + values[1] + important;
		}
		else if(values[1] == values[3])
		{
			return values[0] + " " + values[1] + " " + values[2] + important;
		}
		else return value + important;
		break;
		
		case 3:
		if(values[0] == values[1] && values[0] == values[2])
		{
			return values[0] + important;
		}
		else if(values[0] == values[2])
		{
			return values[0] + " " + values[1] + important;
		}
		else return value + important;
		break;
		
		case 2:
		if(values[0] == values[1])
		{
			return values[0] + important;
		}
		else return value + important;
		break;
		
		default:
		return value + important;
	}
}

string compress_numbers(string subvalue, string property)
{
	string units[] = {"in", "cm", "mm", "pt", "pc", "px", "rem", "%", "ex", "gd", "em", "vw", "vh",
	                  "vm", "deg", "grad", "rad", "ms", "s", "khz", "hz" }; // sync  for loop
	           
	vector<string> temp;
	if(property == "font")
	{
		temp = explode("/",subvalue);
	}
	else
	{
		temp.push_back(subvalue);
	}
		
	for (int i = 0; i < temp.size(); ++i)
	{
		if(!(temp[i].length() > 0 && (ctype_digit(temp[i][0]) || temp[i][0] == '+' || temp[i][0] == '-' ) ))
		{
			continue;
		}
		
		if(in_str_array(color_values,property))
		{
			temp[i] = "#" + temp[i];
		}
	
		if(str2f(temp[i]) == 0)
		{
			temp[i] = "0";
		}
		else
		{
			bool unit_found = false;
			temp[i] = strtolower(temp[i]);
			for(int j = 0; j < 21; ++j )
			{
				if(temp[i].find(units[j]) != string::npos)
				{
					temp[i] = f2str(str2f(temp[i])) + units[j];
					unit_found = true;
					break;
				}
			}
			if(!unit_found && in_str_array(unit_values,property))
			{
				temp[i] = f2str(str2f(temp[i]));
				temp[i] += "px";
			}
			else if(!unit_found)
			{
				temp[i] = f2str(str2f(temp[i]));
			}
			// Remove leading zero
            if (abs( (int) str2f(temp[i])) < 1) {
                if (str2f(temp[i]) < 0) {
                    temp[i] = '-' + temp[i].substr(2);
                } else {
                    temp[i] = temp[i].substr(1);
                }
            }     
		}
	}	
		
	return (temp.size() > 1) ? temp[0] + "/" + temp[1] : ((temp.size() > 0) ? temp[0] : "");
}

bool property_is_next(string istring, int pos)
{
	istring = istring.substr(pos,istring.length()-pos);
	pos = istring.find_first_of(':',0);
	if(pos == string::npos)
	{
		return false;
	}
	istring = strtolower(trim(istring.substr(0,pos)));
	return (all_properties.count(istring) > 0);
}

string cut_color(string color)
{
	if(strtolower(color.substr(0,4)) == "rgb(")
	{
		vector<string> color_tmp = explode(",",color.substr(4,color.length()-5));

		for (int i = 0; i < color_tmp.size(); ++i)
		{
			color_tmp[i] = trim(color_tmp[i]);
			if(color_tmp[i].at(color_tmp[i].length()-1) == '%')
			{
				color_tmp[i] = f2str(round(255 * atoi(color_tmp[i].c_str())/100,0));
			}
			if(atoi(color_tmp[i].c_str()) > 255) color_tmp[i] = 255;
		}
		
		color = "#";
		for (int i = 0; i < color_tmp.size(); ++i)
		{
			if(atoi(color_tmp[i].c_str()) < 16)
			{
				color += "0" + dechex(atoi(color_tmp[i].c_str()));
			}
			else
			{
				color += dechex(atoi(color_tmp[i].c_str()));
			}
		}
	}

	// Fix bad color names
	if(replace_colors.count(strtolower(color)) > 0)
	{
		color = replace_colors[strtolower(color)];
	}

	if(color.length() == 7)
	{
		string color_temp = strtoupper(color);

		if(color_temp[0] == '#' && color_temp[1] == color_temp[2] && color_temp[3] == color_temp[4] && color_temp[5] == color_temp[6])
		{
			color = "#";
			color += color_temp[2];
			color += color_temp[3];
			color += color_temp[5];
		}
	}

	string temp = strtolower(color);
	/* color name -> hex code */
	if(temp == "black")		return "#000";
	if(temp == "fuchsia")	return "#F0F";
	if(temp == "white")		return "#FFF";
	if(temp == "yellow")	return "#FF0";		
	/* hex code -> color name */
	if(temp == "#800000")	return "maroon";
	if(temp == "#ffa500")	return "orange";
	if(temp == "#808000")	return "olive";
	if(temp == "#800080")	return "purple";
	if(temp == "#008000")	return "green";
	if(temp == "#000080")	return "navy";
	if(temp == "#008080")	return "teal";
	if(temp == "#c0c0c0")	return "silver";
	if(temp == "#808080")	return "gray";
	if(temp == "#f00")		return "red";	

	return color;
}

int c_font_weight(string& value)
{
	string important = "";
	if(is_important(value))
	{
		important = "!important";
		value = gvw_important(value);
	}
	if(value == "bold")
	{
		value = "700"+important;
		return 700;
	}
	else if(value == "normal")
	{
		value = "400"+important;
		return 400;
	}
	return 0;
}


void merge_selectors(sstore& input)
{
	for(sstore::iterator i = input.begin(), e = input.end(); i != e;)
	{
		string newsel = "";
	
		// Check if properties also exist in another selector
		vector<string> keys;
		for(sstore::iterator j = input.begin(); j != input.end(); j++ )
		{
			if(j->first == i->first)
			{
				continue;
			}
			
			if(input[j->first] == input[i->first])
			{
				keys.push_back(j->first);
			}
		}

		if(keys.size() > 0)
		{
			newsel = i->first;

			for(int k = 0; k < keys.size(); ++k)
			{
				input.erase(keys[k]);
				newsel += "," + keys[k];
			}

			input[newsel] = i->second;
			
			input.erase(i);
			e = input.end();
		} else {
			i++;
		}
	}
}
