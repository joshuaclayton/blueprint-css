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

string csstidy::_htmlsp(const string istring, const bool plain)
{
    if (!plain) {
        return htmlspecialchars(istring);
    }
    return istring;
}

void csstidy::_convert_raw_css()
{
	csstokens = vector<token>();

    for (css_struct::iterator i = css.begin(); i != css.end(); ++i)
    {
        if (settings["sort_selectors"]) i->second.sort();
        if (i->first != "standard") {
            add_token(AT_START, i->first, true);
        }

        for(sstore::iterator j = i->second.begin(); j != i->second.end(); ++j)
        {
            if (settings["sort_properties"]) j->second.sort();
            add_token(SEL_START, j->first, true);

            for(umap<string,string>::iterator k = j->second.begin(); k != j->second.end(); ++k)
            {
                add_token(PROPERTY, k->first, true);
                add_token(VALUE, k->second, true);
            }

            add_token(SEL_END, j->first, true);
        }

        if (i->first != "standard") {
            add_token(AT_END, i->first, true);
        }
    }
}

int csstidy::_seeknocomment(const int key, int move)
{
    int go = (move > 0) ? 1 : -1;
    for (int i = key + 1; abs(key-i)-1 < abs(move); i += go) {
        if (i < 0 || i > csstokens.size()) {
            return -1;
        }
        if (csstokens[i].type == COMMENT) {
            move += 1;
            continue;
        }
        return csstokens[i].type;
    }
}

void csstidy::print_css(string filename)
{
	if(css.empty() && charset == "" && namesp == "" && import.empty() && csstokens.empty())
	{
		if(!settings["silent"]) cout << "Invalid CSS!" << endl;
		return;
	}

	ofstream file_output;
	if(filename != "")
	{
		file_output.open(filename.c_str(),ios::binary);
		if(file_output.bad())
		{
			if(!settings["silent"]) cout << "Error when trying to save the output file!" << endl;
			return;
		}
	}

	if(!settings["preserve_css"]) {
		_convert_raw_css();
	}

	if(!settings["allow_html_in_templates"])
	{
		for(int i = 0; i < csstemplate.size(); ++i)
		{
			csstemplate[i] = strip_tags(csstemplate[i]);
		}
	}

	stringstream output, in_at_out;

	if (settings["timestamp"]) {
		  time_t rawtime;
		  time(&rawtime);
		  token temp;
		  temp.data = " CSSTidy ";
		  temp.data +=  CSSTIDY_VERSION;
		  temp.data += ": " + rtrim(asctime (localtime ( &rawtime ))) + " ";
		  temp.type = COMMENT;
		  csstokens.insert(csstokens.begin(), temp);
	}

	if(charset != "")
	{
		output << csstemplate[0] << "@charset " << csstemplate[5] << charset << csstemplate[6];
	}

	if(import.size() > 0)
	{
		for(int i = 0; i < import.size(); i ++)
		{
			output  << csstemplate[0] << "@import " << csstemplate[5] << import[i] << csstemplate[6];
		}
	}

	if(namesp != "")
	{
		output << csstemplate[0] << "@namespace " << csstemplate[5] << namesp << csstemplate[6];
	}

	output << csstemplate[13];
	stringstream* out =& output;

    bool plain = !settings["allow_html_in_templates"];

    for (int i = 0; i < csstokens.size(); ++i)
    {
        switch (csstokens[i].type)
        {
            case AT_START:
                *out << csstemplate[0] << _htmlsp(csstokens[i].data, plain) + csstemplate[1];
                out =& in_at_out;
                break;

            case SEL_START:
                if(settings["lowercase_s"]) csstokens[i].data = strtolower(csstokens[i].data);
                *out << ((csstokens[i].data[0] != '@') ? csstemplate[2] + _htmlsp(csstokens[i].data, plain) : csstemplate[0] + _htmlsp(csstokens[i].data, plain));
                *out << csstemplate[3];
                break;

            case PROPERTY:
                if(settings["case_properties"] == 2) csstokens[i].data = strtoupper(csstokens[i].data);
                if(settings["case_properties"] == 1) csstokens[i].data = strtolower(csstokens[i].data);
                *out << csstemplate[4] << _htmlsp(csstokens[i].data, plain) << ":" << csstemplate[5];
                break;

            case VALUE:
                *out << _htmlsp(csstokens[i].data, plain);
                if(_seeknocomment(i, 1) == SEL_END && settings["remove_last_;"]) {
                    *out << str_replace(";", "", csstemplate[6]);
                } else {
                    *out << csstemplate[6];
                }
                break;

            case SEL_END:
                *out << csstemplate[7];
                if(_seeknocomment(i, 1) != AT_END) *out << csstemplate[8];
                break;

            case AT_END:
				out =& output;
            	*out << csstemplate[10] << str_replace("\n", "\n" + csstemplate[10], in_at_out.str());
            	in_at_out.str("");
                *out << csstemplate[9];
                break;

            case COMMENT:
                *out << csstemplate[11] <<  "/*" << _htmlsp(csstokens[i].data, plain) << "*/" << csstemplate[12];
                break;
        }
    }

	string output_string = trim(output.str());

	if(!settings["silent"]) {
		cout << endl << "Selectors: " << selectors << " | Properties: " << properties << endl;
		float ratio = round(((input_size - (float) output_string.length())/input_size)*100,2);
		float i_b = round(((float) input_size)/1024,3);
		float o_b = round(((float) output_string.length())/1024,3);
		cout << "Input size: " << i_b << "KiB  Output size: " << o_b << "KiB  Compression ratio: " << ratio << "%" << endl;
	}

	if(filename == "")
	{
		if(!settings["silent"]) cout << "-----------------------------------\n\n";
		cout << output_string << "\n";
	}
	else
	{
		file_output << output_string;
	}

	if(logs.size() > 0 && !settings["silent"])
	{
		cout << "-----------------------------------\n\n";
		for(map<int, vector<message> >::iterator j = logs.begin(); j != logs.end(); j++ )
		{
			for(int i = 0; i < j->second.size(); ++i)
			{
				cout << j->first << ": " << j->second[i].m << "\n" ;
			}
		}
	}

	if(!settings["silent"]) {
		cout << "\n-----------------------------------" << endl << "CSSTidy " << CSSTIDY_VERSION << " by Florian Schmitz 2005, 2006" << endl;
	}
	file_output.close();
}
