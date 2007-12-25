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

/* is = in selector
 * ip = in property
 * iv = in value
 * instr = in string (-> ",',( => ignore } and ; etc.)
 * ic = in comment (ignore everything)
 * at = in @-block
 */

extern map<string,string> all_properties,replace_colors;
extern map< string, vector<string> > shorthands;
extern map<string,parse_status> at_rules;

void csstidy::parse_css(string css_input)
{
	input_size = css_input.length();
	css_input = str_replace("\r\n","\n",css_input); // Replace all double-newlines
	css_input += "\n";
	parse_status status = is, from;
	cur_property = "";

	string temp_add,cur_comment,temp;

	vector<string> cur_sub_value_arr;
	char str_char;
	bool str_in_str = false;
	bool invalid_at = false;
	bool pn = false;

	int str_size = css_input.length();
	for(int i = 0; i < str_size; ++i)
	{
		if(css_input[i] == '\n' || css_input[i] == '\r')
		{
			++line;
		}

		switch(status)
		{
			/* Case in-at-block */
			case at:
			if(is_token(css_input,i))
			{
				if(css_input[i] == '/' && s_at(css_input,i+1) == '*')
				{
					status = ic; i += 2;
					from = at;
				}
				else if(css_input[i] == '{')
				{
					status = is;
					add_token(AT_START, cur_at);
				}
				else if(css_input[i] == ',')
				{
					cur_at = trim(cur_at) + ",";
				}
				else if(css_input[i] == '\\')
				{
					cur_at += unicode(css_input,i);
				}
			}
			else
			{
				int lastpos = cur_at.length()-1;
				if(lastpos == -1 || !( (ctype_space(cur_at[lastpos]) || is_token(cur_at,lastpos) && cur_at[lastpos] == ',') && ctype_space(css_input[i])))
				{
					cur_at += css_input[i];
				}
			}
			break;

			/* Case in-selector */
			case is:
			if(is_token(css_input,i))
			{
				if(css_input[i] == '/' && s_at(css_input,i+1) == '*' && trim(cur_selector) == "")
				{
					status = ic; ++i;
					from = is;
				}
				else if(css_input[i] == '@' && trim(cur_selector) == "")
				{
					// Check for at-rule
					invalid_at = true;
					for(map<string,parse_status>::iterator j = at_rules.begin(); j != at_rules.end(); ++j )
					{
						if(strtolower(css_input.substr(i+1,j->first.length())) == j->first)
						{
							(j->second == at) ? cur_at = "@" + j->first : cur_selector = "@" + j->first;
							status = j->second;
							i += j->first.length();
							invalid_at = false;
						}
					}
					if(invalid_at)
					{
						cur_selector = "@";
						string invalid_at_name = "";
						for(int j = i+1; j < str_size; ++j)
						{
							if(!ctype_alpha(css_input[j]))
							{
								break;
							}
							invalid_at_name += css_input[j];
						}
						log("Invalid @-rule: " + invalid_at_name + " (removed)",Warning);
					}
				}
				else if(css_input[i] == '"' || css_input[i] == '\'')
				{
					cur_string = css_input[i];
					status = instr;
					str_char = css_input[i];
					from = is;
				}
				else if(invalid_at && css_input[i] == ';')
				{
					invalid_at = false;
					status = is;
				}
				else if(css_input[i] == '{')
				{
					status = ip;
					add_token(SEL_START, cur_selector);
					++selectors;
				}
				else if(css_input[i] == '}')
				{
					add_token(AT_END, cur_at);
					cur_at = "";
					cur_selector = "";
					sel_separate = vector<int>();
				}
				else if(css_input[i] == ',')
				{
					cur_selector = trim(cur_selector) + ",";
					sel_separate.push_back(cur_selector.length());
				}
				else if(css_input[i] == '\\')
				{
					cur_selector += unicode(css_input,i);
				}
				// remove unnecessary universal selector,  FS#147
				else if(!(css_input[i] == '*' && (s_at(css_input,i+1) == '.' || s_at(css_input,i+1) == '[' || s_at(css_input,i+1) == ':' || s_at(css_input,i+1) == '#')))
				{
					cur_selector += css_input[i];
				}
			}
			else
			{
				int lastpos = cur_selector.length()-1;
				if(!( (ctype_space(cur_selector[lastpos]) || is_token(cur_selector,lastpos) && cur_selector[lastpos] == ',') && ctype_space(css_input[i])))
				{
					cur_selector += css_input[i];
				}
			}
			break;

			/* Case in-property */
			case ip:
			if(is_token(css_input,i))
			{
				if(css_input[i] == ':' || css_input[i] == '=' && cur_property != "") // IE really accepts =, so csstidy will fix those mistakes
				{
					status = iv;
					bool valid = !settings["discard_invalid_properties"] ||
                                    (all_properties.count(cur_property) > 0 && all_properties[cur_property].find(css_level,0) != string::npos);
					if(valid) {
						add_token(PROPERTY, cur_property);
					}
				}
				else if(css_input[i] == '/' && s_at(css_input,i+1) == '*' && cur_property == "")
				{
					status = ic; ++i;
					from = ip;
				}
				else if(css_input[i] == '}')
				{
					explode_selectors();
					status = is;
					invalid_at = false;
					add_token(SEL_END, cur_selector);
					cur_selector = "";
					cur_property = "";
				}
				else if(css_input[i] == ';')
				{
					cur_property = "";
				}
				else if(css_input[i] == '\\')
				{
					cur_property += unicode(css_input,i);
				}
			}
			else if(!ctype_space(css_input[i]))
			{
				cur_property += css_input[i];
			}
			break;

			/* Case in-value */
			case iv:
			pn = ((css_input[i] == '\n' || css_input[i] == '\r') && property_is_next(css_input,i+1) || i == str_size-1);
			if(pn)
			{
				log("Added semicolon to the end of declaration",Warning);
			}
			if(is_token(css_input,i) || pn)
			{
				if(css_input[i] == '/' && s_at(css_input,i+1) == '*')
				{
					status = ic; ++i;
					from = iv;
				}
				else if(css_input[i] == '"' || css_input[i] == '\'' || css_input[i] == '(')
				{
					str_char = (css_input[i] == '(') ? ')' : css_input[i];
					cur_string = css_input[i];
					status = instr;
					from = iv;
				}
				else if(css_input[i] == '\\')
				{
					cur_sub_value += unicode(css_input,i);
				}
				else if(css_input[i] == ';' || pn)
				{
					if(cur_selector.substr(0,1) == "@" && at_rules.count(cur_selector.substr(1)) > 0 && at_rules[cur_selector.substr(1)] == iv)
					{
						cur_sub_value_arr.push_back(trim(cur_sub_value));
						status = is;

						if(cur_selector == "@charset") charset = cur_sub_value_arr[0];
						if(cur_selector == "@namespace") namesp = implode(" ",cur_sub_value_arr);
						if(cur_selector == "@import") import.push_back(implode(" ",cur_sub_value_arr));

						cur_sub_value_arr.clear();
						cur_sub_value = "";
						cur_selector = "";
						sel_separate = vector<int>();
					}
					else
					{
						status = ip;
					}
				}
				else if (css_input[i] == '!') {
					cur_sub_value = optimise_subvalue(cur_sub_value,cur_property);
					cur_sub_value_arr.push_back(trim(cur_sub_value));
					cur_sub_value = "!";
				}
				else if(css_input[i] != '}')
				{
					cur_sub_value += css_input[i];
				}
				if( (css_input[i] == '}' || css_input[i] == ';' || pn) && !cur_selector.empty())
				{
					++properties;

					if(cur_at == "")
					{
						cur_at = "standard";
					}

					// Kill all whitespace
					cur_at = trim(cur_at); cur_selector = trim(cur_selector);
					cur_value = trim(cur_value); cur_property = trim(cur_property);
					cur_sub_value = trim(cur_sub_value);

					// case settings
					if(settings["lowercase_s"])
					{
						cur_selector = strtolower(cur_selector);
					}
					cur_property = strtolower(cur_property);


					if(cur_sub_value != "")
					{
						cur_sub_value = optimise_subvalue(cur_sub_value,cur_property);
						cur_sub_value_arr.push_back(cur_sub_value);
						cur_sub_value = "";
					}

					cur_value = implode(" ",cur_sub_value_arr);

					// Compress !important
					temp = c_important(cur_value);
					if(temp != cur_value)
					{
						log("Optimised !important",Information);
					}
					cur_value = temp;

					// Optimise shorthand properties
					if(shorthands.count(cur_property) > 0)
					{
						temp = shorthand(cur_value);
						if(temp != cur_value)
						{
							log("Optimised shorthand notation (" + cur_property + "): Changed \"" + cur_value + "\" to \"" + temp + "\"",Information);
						}
						cur_value = temp;
					}

					// Compress font-weight (tiny compression)
					if(cur_property == "font-weight" && settings["compress_font-weight"])
					{
						int c_fw = c_font_weight(cur_value);
						if(c_fw == 400)
						{
							log("Optimised font-weight: Changed \"bold\" to \"700\"",Information);
						}
						else if(c_fw == 700)
						{
							log("Optimised font-weight: Changed \"normal\" to \"400\"",Information);
						}
					}

					bool valid = (all_properties.count(cur_property) > 0 && all_properties[cur_property].find(css_level,0) != string::npos);
					if((!invalid_at || settings["preserve_css"]) && (!settings["discard_invalid_properties"] || valid))
					{
						add(cur_at,cur_selector,cur_property,cur_value);
						add_token(VALUE, cur_value);

						// Further Optimisation
						if(cur_property == "background" && settings["optimise_shorthands"] > 1)
						{
							map<string,string> temp = dissolve_short_bg(cur_value);
							css[cur_at][cur_selector].erase("background");
							for(map<string,string>::iterator it = temp.begin(); it != temp.end(); ++it )
							{
								add(cur_at,cur_selector,it->first,it->second);
							}
						}
						if(shorthands.count(cur_property) > 0 && settings["optimise_shorthands"] > 0)
						{
							map<string,string> temp = dissolve_4value_shorthands(cur_property,cur_value);
							for(map<string,string>::iterator it = temp.begin(); it != temp.end(); ++it )
							{
								add(cur_at,cur_selector,it->first,it->second);
							}
							if(shorthands[cur_property][0] != "0")
							{
								css[cur_at][cur_selector].erase(cur_property);
							}
						}
					}
					if(!valid)
					{
						if(settings["discard_invalid_properties"])
						{
							log("Removed invalid property: " + cur_property,Warning);
						}
						else
						{
							log("Invalid property in " + strtoupper(css_level) + ": " + cur_property,Warning);
						}
					}

					//Split multiple selectors here if necessary
					cur_property = "";
					cur_sub_value_arr.clear();
					cur_value = "";
				}
				if(css_input[i] == '}')
				{
					explode_selectors();
					add_token(SEL_END, cur_selector);
					status = is;
					invalid_at = false;
					cur_selector = "";
				}
			}
			else if(!pn)
			{
				cur_sub_value += css_input[i];

				if(ctype_space(css_input[i]))
				{
					if(trim(cur_sub_value) != "")
					{
						cur_sub_value = optimise_subvalue(cur_sub_value,cur_property);
						cur_sub_value_arr.push_back(trim(cur_sub_value));
					}
					cur_sub_value = "";
				}
			}
			break;

			/* Case in-string */
			case instr:
			if(str_char == ')' && (css_input[i] == '"' || css_input[i] == '\'') && str_in_str == false && !escaped(css_input,i))
			{
				str_in_str = true;
			}
			else if(str_char == ')' && (css_input[i] == '"' || css_input[i] == '\'') && str_in_str == true && !escaped(css_input,i))
			{
				str_in_str = false;
			}
			temp_add = ""; temp_add += css_input[i];
			if( (css_input[i] == '\n' || css_input[i] == '\r') && !(css_input[i-1] == '\\' && !escaped(css_input,i-1)) )
			{
				temp_add = "\\A ";
				log("Fixed incorrect newline in string",Warning);
			}
			if (!(str_char == ')' && char2str(css_input[i]).find_first_of(" \n\t\r\0xb") != string::npos && !str_in_str)) {
				cur_string += temp_add;
			}
			if(css_input[i] == str_char && !escaped(css_input,i) && str_in_str == false)
			{
				status = from;
				if (cur_string.find_first_of(" \n\t\r\0xb") == string::npos && cur_property != "content") {
					if (str_char == '"' || str_char == '\'') {
						cur_string = cur_string.substr(1, cur_string.length() - 2);
					} else if (cur_string.length() > 3 && (cur_string[1] == '"' || cur_string[1] == '\'')) /* () */ {
						cur_string = cur_string[0] + cur_string.substr(2, cur_string.length() - 4) + cur_string[cur_string.length()-1];
					}
				}
				if(from == iv)
				{
					cur_sub_value += cur_string;
				}
				else if(from == is)
				{
					cur_selector += cur_string;
				}
			}
			
			break;

			/* Case in-comment */
			case ic:
			if(css_input[i] == '*' && s_at(css_input,i+1) == '/')
			{
				status = from;
				++i;
				add_token(COMMENT, cur_comment);
				cur_comment = "";
			}
			else
			{
				cur_comment += css_input[i];
			}
			break;
		}
	}

	if(settings["merge_selectors"] > 1)
	{
		for(css_struct::iterator i = css.begin(); i != css.end(); i++ )
		{
			merge_selectors(i->second);
		}
	}

	if(settings["optimise_shorthands"] > 0)
	{
		for(css_struct::iterator i = css.begin(); i != css.end(); ++i )
		{
			for(sstore::iterator j = i->second.begin(); j != i->second.end();)
			{
				merge_4value_shorthands(i->first,j->first);
				if(settings["optimise_shorthands"] > 1) {
					merge_bg(j->second);
				}

				if(j->second.size() == 0) {
					i->second.erase(j);
				} else {
					 ++j;
				}
			}
		}
	}
}

string csstidy::optimise_subvalue(string subvalue, const string property)
{
	subvalue = trim(subvalue);

	string temp = compress_numbers(subvalue,property);
	if(temp != subvalue)
	{
		if(temp.length() > subvalue.length())
		{
			log("Fixed invalid number: Changed \"" + subvalue + "\" to \"" + temp + "\"",Warning);
		}
		else
		{
			log("Optimised number: Changed \"" + subvalue + "\" to \"" + temp + "\"",Information);
		}
		subvalue = temp;
	}
	if(settings["compress_colors"])
	{
		temp = cut_color(subvalue);
		if(temp != subvalue)
		{
			if(replace_colors.count(subvalue) > 0)
			{
				log("Fixed invalid color name: Changed \"" + subvalue + "\" to \"" + temp + "\"",Warning);
			}
			else
			{
				log("Optimised color: Changed \"" + subvalue + "\" to \"" + temp + "\"",Information);
			}
			subvalue = temp;
		}
	}
	return subvalue;
}
