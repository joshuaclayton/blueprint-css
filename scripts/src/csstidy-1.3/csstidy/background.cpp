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
extern map<string,string> background_prop_default;

map<string,string> dissolve_short_bg(string istring)
{
	vector<string> repeat,attachment,clip,origin,pos,str_values;
	
	repeat.push_back("repeat"); repeat.push_back("repeat-x"); repeat.push_back("repeat-y");
	repeat.push_back("no-repeat"); repeat.push_back("space");
	attachment.push_back("scroll"); attachment.push_back("fixed"); attachment.push_back("local");
	clip.push_back("border"); clip.push_back("padding");
	origin.push_back("border"); origin.push_back("padding"); origin.push_back("content");
	pos.push_back("top"); pos.push_back("center"); pos.push_back("bottom"); pos.push_back("left"); pos.push_back("right");
	string important = "";
	
	map<string,string> ret;
	map<string,bool> have;
	ret["background-image"] = "";
	ret["background-size"] = "";
	ret["background-repeat"] = "";
	ret["background-position"] = "";
	ret["background-attachment"] = "";
	ret["background-clip"] = "";
	ret["background-origin"] = "";
	ret["background-color"] = "";
	
	if(is_important(istring))
	{
		important = "!important";
		istring = gvw_important(istring);
	}
	
	str_values = explode_ws(',',istring);
	for(int i = 0; i < str_values.size(); i++)
	{
		have["clip"] = false; have["pos"] = false;
		have["color"] = false; have["bg"] = false;
		
		vector<string> temp_values = explode_ws(' ',trim(str_values[i]));
	
		for(int j = 0; j < temp_values.size(); j++)
		{
			if(have["bg"] == false && ((temp_values[j]).substr(0,4) == "url(" || temp_values[j] == "none"))
			{
				ret["background-image"] += temp_values[j];
				ret["background-image"] += ",";
				have["bg"] = true;
			}
			else if(in_str_array(repeat,temp_values[j]))
			{
				ret["background-repeat"] += temp_values[j];
				ret["background-repeat"] += ",";
			}
			else if(in_str_array(attachment,temp_values[j]))
			{
				ret["background-attachment"] += temp_values[j];
				ret["background-attachment"] += ",";
			}
			else if(in_str_array(clip,temp_values[j]) && !have["clip"])
			{
				ret["background-clip"] += temp_values[j];
				ret["background-clip"] += ",";
				have["clip"] = true;
			}
			else if(in_str_array(origin,temp_values[j]))
			{
				ret["background-origin"] += temp_values[j];
				ret["background-origin"] += ",";
			}
			else if(temp_values[j][0] == '(')
			{
				ret["background-size"] += (temp_values[j]).substr(1,temp_values[j].length()-2);
				ret["background-size"] += ",";
			}
			else if(in_str_array(pos,temp_values[j]) || isdigit(temp_values[j][0]) || temp_values[j][0] == 0)
			{
				ret["background-position"] += temp_values[j];
				if(!have["pos"]) ret["background-position"] += " "; else ret["background-position"] += ",";
				have["pos"] = true;
			}
			else if(!have["color"])
			{
				ret["background-color"] += temp_values[j];
				ret["background-color"] += ",";
				have["color"] = true;
			}
		}
	}
	
	for(map<string,string>::iterator it = background_prop_default.begin(); it != background_prop_default.end(); it++ )
	{
		if(ret[it->first] != "")
		{
			ret[it->first] = (ret[it->first]).substr(0,ret[it->first].length()-1);
			ret[it->first] += important;
		}
		else
		{
			ret[it->first] = it->second;
			ret[it->first] += important;
		}
	}
	
	return ret;	
}

vector<string> explode_ws(char sep,string istring)
{
	// 1 = st // 2 = str
	int status = 1;
	char to;
	
	vector<string> output;
	output.push_back("");
	int num = 0;
	int len = istring.length();
	for(int i = 0;i < len; i++)
	{
		switch(status)
		{
			case 1:
			if(istring[i] == sep && !escaped(istring,i))
			{
				++num;
				output.push_back("");
			}
			else if(istring[i] == '"' || istring[i] == '\'' || istring[i] == '(' && !escaped(istring,i))
			{
				status = 2;
				to = (istring[i] == '(') ? ')' : istring[i];
				output[num] += istring[i];
			}
			else
			{
				output[num] += istring[i];
			}
			break;
			
			case 2:
			if(istring[i] == to && !escaped(istring,i))
			{
				status = 1;
			}
			output[num] += istring[i];
			break;
		}
	}
	
	return output;
}

void merge_bg(umap<string,string>& css_input)
{
	// Max number of background images. CSS3 not yet fully implemented
	int number_of_values = max((explode_ws(',',css_input["background-image"]).size()),(explode_ws(',',css_input["background-color"])).size());
	// Array with background images to check if BG image exists
	vector<string> bg_img_array = explode_ws(',',gvw_important(css_input["background-image"]));
	string new_bg_value,important = "";
	
	for(int i = 0; i < number_of_values; i++)
	{
		for(map<string,string>::iterator it = background_prop_default.begin(); it != background_prop_default.end(); it++ )
		{			
			// Skip if property does not exist
			if(!css_input.has(it->first))
			{
				continue;
			}
			
			string cur_value = css_input[it->first];
			
			// Skip some properties if there is no background image
			if((bg_img_array.size() <= i || bg_img_array[i] == "none")
				&& (it->first == "background-size" || it->first == "background-position"
				|| it->first == "background-attachment" || it->first == "background-repeat"))
			{
				continue;
			}
			
			// Remove !important
			if(is_important(cur_value))
			{
				important = "!important";
				cur_value = gvw_important(cur_value);
			}
			
			// Do not add default values
			if(cur_value == it->second)
			{
				continue;
			}
			
			vector<string> temp = explode_ws(',',cur_value);

			if(temp.size() > i)
			{					
				if(it->first == "background-size")
				{
					new_bg_value += "(";
					new_bg_value += temp[i];
					new_bg_value += ") ";
				}
				else
				{
					new_bg_value += temp[i];
					new_bg_value += " ";
				}
			}			
		}
		
		new_bg_value = trim(new_bg_value);
		if(i != number_of_values-1) new_bg_value += ",";
	}
		
	// Delete all background-properties
	for(map<string,string>::iterator it = background_prop_default.begin(); it != background_prop_default.end(); it++ )
	{
		css_input.erase(it->first);
	}
	
	// Add new background property
	if(new_bg_value != "")
	{
		css_input["background"] = new_bg_value + important;
	}
}
