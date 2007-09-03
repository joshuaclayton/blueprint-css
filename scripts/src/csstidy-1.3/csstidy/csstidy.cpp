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
extern map< string, vector<string> > shorthands;

csstidy::csstidy()
{ 
	properties = 0;
	selectors = 0;
	charset = "";
	namesp = "";
	line = 1;
	tokens = "{};:()@='\"/,\\!$%&*+.<>?[]^`|~";
	css_level = "CSS2.1";
	
	settings["remove_bslash"] = 1;
	settings["compress_colors"] = 1;
	settings["compress_font-weight"] = 1;
	settings["lowercase_s"] = 0;
	settings["optimise_shorthands"] = 1;
	settings["remove_last_;"] = 0;
	settings["case_properties"] = 0;
	settings["sort_properties"] = 0;
	settings["sort_selectors"] = 0;
	settings["merge_selectors"] = 2;
	settings["discard_invalid_properties"] = 0;
	settings["allow_html_in_templates"] = 0;
	settings["silent"] = 0;
	settings["preserve_css"] = 0;
	settings["timestamp"] = 0;
	
	csstemplate.push_back("<span class=\"at\">"); //string before @rule
	csstemplate.push_back("</span> <span class=\"format\">{</span>\n"); //bracket after @-rule
	csstemplate.push_back("<span class=\"selector\">"); //string before selector
	csstemplate.push_back("</span> <span class=\"format\">{</span>\n"); //bracket after selector
	csstemplate.push_back("<span class=\"property\">"); //string before property
	csstemplate.push_back("</span><span class=\"value\">"); //string after property+before value
	csstemplate.push_back("</span><span class=\"format\">;</span>\n"); //string after value
	csstemplate.push_back("<span class=\"format\">}</span>"); //closing bracket - selector
	csstemplate.push_back("\n\n"); //space between blocks {...}
	csstemplate.push_back("\n<span class=\"format\">}</span>\n\n"); //closing bracket @-rule
	csstemplate.push_back(""); //indent in @-rule
	csstemplate.push_back("<span class=\"comment\">"); // before comment
	csstemplate.push_back("</span>\n"); //after comment
	csstemplate.push_back("\n"); // after last line @-rule
} 
	
void csstidy::add_token(const token_type ttype, const string data, const bool force)
{
	if(settings["preserve_css"] || force) {
		token temp;
		temp.type = ttype;
		temp.data = (ttype == COMMENT) ? data : trim(data);
		csstokens.push_back(temp);
	}
}

void csstidy::copy(const string media, const string selector, const string media_new, const string selector_new)
{
	for(int k = 0; k < css[media][selector].size(); k++)
	{	
		string property = css[media][selector].at(k);
		string value = css[media][selector][property];
		add(media_new,selector_new,property,value);
	}
}

void csstidy::add(const string& media, const string& selector, const string& property, const string& value)
{
	if(settings["preserve_css"]) {
		return;
	}
	
	if(css[media][selector].has(property))
	{
		if( !is_important(css[media][selector][property]) || (is_important(css[media][selector][property]) && is_important(value)) )
		{
			css[media][selector].erase(property);
			css[media][selector][property] = trim(value);
		}
	}
	else
	{
		css[media][selector][property] = trim(value);
	}
}

void csstidy::log(const string msg, const message_type type, int iline)
{
	message new_msg;
	new_msg.m = msg;
	new_msg.t = type;
	if(iline == 0)
	{
		iline = line;
	}
	if(logs.count(line) > 0)
	{
		for(int i = 0; i < logs[line].size(); ++i)
		{
			if(logs[line][i].m == new_msg.m && logs[line][i].t == new_msg.t)
			{
				return;
			}
		}
	}
	logs[line].push_back(new_msg);
}

string csstidy::unicode(string& istring,int& i)
{
	++i;
	string add = "";
	bool replaced = false;
	
	while(i < istring.length() && (ctype_xdigit(istring[i]) || ctype_space(istring[i])) && add.length()< 6)
	{
		add += istring[i];

		if(ctype_space(istring[i]))
		{
			break;
		}
		i++;
	}

	if(hexdec(add) > 47 && hexdec(add) < 58 || hexdec(add) > 64 && hexdec(add) < 91 || hexdec(add) > 96 && hexdec(add) < 123)
	{
		string msg = "Replaced unicode notation: Changed \\" + rtrim(add) + " to ";
		add = static_cast<int>(hexdec(add));
		msg += add;
		log(msg,Information);
		replaced = true;
	}
	else
	{
		add = trim("\\" + add);
	}

	if(ctype_xdigit(istring[i+1]) && ctype_space(istring[i]) && !replaced || !ctype_space(istring[i]))
	{
		i--;
	}
	
	if(add != "\\" || !settings["remove_bslash"] || in_str_array(tokens,istring[i+1]))
	{
		return add;
	}
	if(add == "\\")
	{
		log("Removed unnecessary backslash",Information);
	}
	return "";
}

bool csstidy::is_token(string& istring,const int i)
{
	return (in_str_array(tokens,istring[i]) && !escaped(istring,i));
}

void csstidy::merge_4value_shorthands(string media, string selector)
{
	for(map< string, vector<string> >::iterator i = shorthands.begin(); i != shorthands.end(); ++i )
	{
		string temp;

		if(css[media][selector].has(i->second[0]) && css[media][selector].has(i->second[1])
		&& css[media][selector].has(i->second[2]) && css[media][selector].has(i->second[3]))
		{
			string important = "";
			for(int j = 0; j < 4; ++j)
			{
				string val = css[media][selector][i->second[j]];
				if(is_important(val))
				{
					important = "!important";
					temp += gvw_important(val)+ " ";
				}
				else
				{
					temp += val + " ";
				}
				css[media][selector].erase(i->second[j]);
			}
			add(media, selector, i->first, shorthand(trim(temp + important)));		
		}
	}
} 

map<string,string> csstidy::dissolve_4value_shorthands(string property, string value)
{
	map<string, string> ret;
	extern map< string, vector<string> > shorthands;
	
	if(shorthands[property][0] == "0")
	{
		ret[property] = value;
		return ret;
	}
	
	string important = "";
	if(is_important(value))
	{
		value = gvw_important(value);
		important = "!important";
	}
	vector<string> values = explode(" ",value);

	if(values.size() == 4)
	{
		for(int i=0; i < 4; ++i)
		{
			ret[shorthands[property][i]] = values[i] + important;
		}
	}
	else if(values.size() == 3)
	{
		ret[shorthands[property][0]] = values[0] + important;
		ret[shorthands[property][1]] = values[1] + important;
		ret[shorthands[property][3]] = values[1] + important;
		ret[shorthands[property][2]] = values[2] + important;
	}
	else if(values.size() == 2)
	{
		for(int i = 0; i < 4; ++i)
		{
			ret[shorthands[property][i]] = ((i % 2 != 0)) ? values[1] + important : values[0] + important;
		}
	}
	else
	{
		for(int i = 0; i < 4; ++i)
		{
			ret[shorthands[property][i]] = values[0] + important;
		}	
	}
	
	return ret;
}

void csstidy::explode_selectors()
{
	// Explode multiple selectors
    if (settings["merge_selectors"] == 1)
    {
        vector<string> new_sels;
        int lastpos = 0;
        sel_separate.push_back(cur_selector.length());
        
        for (int i = 0; i < sel_separate.size(); ++i)
        {
            if (i == sel_separate.size()-1) {
                sel_separate[i] += 1;
            }
            
            new_sels.push_back(cur_selector.substr(lastpos,sel_separate[i]-lastpos-1));
            lastpos = sel_separate[i];
        }
 
        if (new_sels.size() > 1)
        {
            for (int i = 0; i < new_sels.size(); ++i)
            {
				for (pstore::iterator j = css[cur_at][cur_selector].begin(); j != css[cur_at][cur_selector].end(); ++j)
				{
            		add(cur_at, new_sels[i], j->first, j->second);
				}
            }
            css[cur_at].erase(cur_selector);
        }
    }
    sel_separate = vector<int>();
}
