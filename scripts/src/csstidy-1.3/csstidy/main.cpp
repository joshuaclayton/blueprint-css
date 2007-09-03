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

using namespace std;

#include "csspp_globals.hpp"

#include "prepare.hpp"

map< string, vector<string> > predefined_templates;


int main(int argc, char *argv[])
{
	prepare();
	predefined_templates["high"].push_back("<span class=\"at\">");
	predefined_templates["high"].push_back("</span> <span class=\"format\">{</span>\n");
	predefined_templates["high"].push_back("<span class=\"selector\">");
	predefined_templates["high"].push_back("</span><span class=\"format\">{</span>");
	predefined_templates["high"].push_back("<span class=\"property\">");
	predefined_templates["high"].push_back("</span><span class=\"value\">");
	predefined_templates["high"].push_back("</span><span class=\"format\">;</span>");
	predefined_templates["high"].push_back("<span class=\"format\">}</span>");
	predefined_templates["high"].push_back("\n");
	predefined_templates["high"].push_back("\n<span class=\"format\">}\n</span>");
	predefined_templates["high"].push_back("");
	predefined_templates["high"].push_back("<span class=\"comment\">"); // before comment
	predefined_templates["high"].push_back("</span>"); //after comment
	predefined_templates["high"].push_back("\n"); // after last line @-rule
	
	predefined_templates["highest"].push_back("<span class=\"at\">");
	predefined_templates["highest"].push_back("</span><span class=\"format\">{</span>");
	predefined_templates["highest"].push_back("<span class=\"selector\">");
	predefined_templates["highest"].push_back("</span><span class=\"format\">{</span>");
	predefined_templates["highest"].push_back("<span class=\"property\">");
	predefined_templates["highest"].push_back("</span><span class=\"value\">");
	predefined_templates["highest"].push_back("</span><span class=\"format\">;</span>");
	predefined_templates["highest"].push_back("<span class=\"format\">}</span>");
	predefined_templates["highest"].push_back("");
	predefined_templates["highest"].push_back("<span class=\"format\">}</span>");
	predefined_templates["highest"].push_back("");
	predefined_templates["highest"].push_back("<span class=\"comment\">"); // before comment
	predefined_templates["highest"].push_back("</span>"); //after comment
	predefined_templates["highest"].push_back(""); // after last line @-rule
		
	predefined_templates["low"].push_back("<span class=\"at\">");
	predefined_templates["low"].push_back("</span> <span class=\"format\">{</span>\n");
	predefined_templates["low"].push_back("<span class=\"selector\">");
	predefined_templates["low"].push_back("</span>\n<span class=\"format\">{</span>\n");
	predefined_templates["low"].push_back("\t<span class=\"property\">");
	predefined_templates["low"].push_back("</span><span class=\"value\">");
	predefined_templates["low"].push_back("</span><span class=\"format\">;</span>\n");
	predefined_templates["low"].push_back("<span class=\"format\">}</span>");
	predefined_templates["low"].push_back("\n\n");
	predefined_templates["low"].push_back("\n<span class=\"format\">}</span>\n\n");
	predefined_templates["low"].push_back("\t");
	predefined_templates["low"].push_back("<span class=\"comment\">"); // before comment
	predefined_templates["low"].push_back("</span>\n"); //after comment
	predefined_templates["low"].push_back("\n"); // after last line @-rule
	
	csstidy csst;

	if(argc > 1)
	{
		string filein = argv[1];
		if(filein != "-" && !file_exists(argv[1]))
		{
			cout << "The file \"" << filein << "\" does not exist." << endl;
			return EXIT_FAILURE;
		}
		
		string output_filename;
		
		for(int i = 2; i < argc; ++i)
		{
			bool output_file = true;
			for(map<string,int>::iterator j = csst.settings.begin(); j != csst.settings.end(); ++j )
			{
				if(trim(argv[i]) == "--" + j->first + "=false" || trim(argv[i]) == "--" + j->first + "=0")
				{
					csst.settings[j->first] = 0;
					output_file = false;
				}
				else if(trim(argv[i]) == "--" + j->first + "=true" || trim(argv[i]) == "--" + j->first + "=1")
				{
					csst.settings[j->first] = 1;
					output_file = false;
				}
				else if(trim(argv[i]) == "--" + j->first + "=2")
				{
					csst.settings[j->first] = 2;
					output_file = false;
				}
			}
			if(trim(argv[i]).substr(0,12) == "--css_level=")
			{
				csst.css_level = strtoupper(trim(argv[i]).substr(12));
				output_file = false;
			}
			else if(trim(argv[i]).substr(0,11) == "--template=")
			{
				string template_value = trim(argv[i]).substr(11);
				if(template_value == "high" || template_value == "highest" || template_value == "low")
				{
					csst.csstemplate = predefined_templates[template_value];
				}
				else if(template_value != "default")
				{
					string tpl_content = file_get_contents(template_value);
					if(tpl_content != "")
					{
						vector<string> tpl_arr = explode("|",tpl_content,true);
						csst.csstemplate = tpl_arr;
					}
				}
				output_file = false;
			}
			if(output_file)
			{
				output_filename = trim(argv[i]);
			}
		}
		
		string css_file;
        if(filein == "-") {
			string temp;
			do {
				getline(cin, temp, '\n');
				css_file += (temp + "\n");
			} while(cin);
		} else {
            css_file = file_get_contents(argv[1]);
        }

		csst.parse_css(css_file);
		
		// Print CSS to screen if no output file is specified
		if(output_filename == "")
		{
			csst.print_css();
		}
		else
		{
			csst.print_css(output_filename);
		}
		
		return EXIT_SUCCESS;
	}

	cout << endl << "Usage:" << endl << endl << "csstidy input_filename [\n";
	for(map<string,int>::iterator j = csst.settings.begin(); j != csst.settings.end(); ++j )
	{
		if (j->first == "optimise_shorthands" || j->first == "merge_selectors"
		   || j->first == "case_properties") {
			continue;
		}
		
		cout << " --" << j->first;
		if(j->second == true)
		{
			cout << "=[true|false] |\n";
		}
		else
		{
			cout << "=[false|true] |\n";
		}
	}
	cout << " --merge_selectors=[2|1|0] |\n";
	cout << " --case_properties=[0|1|2] |\n";
	cout << " --optimise_shorthands=[1|2|0] |\n";
	cout << " --template=[default|filename|low|high|highest] |\n";
	cout << " output_filename ]*" << endl;
	
	return EXIT_SUCCESS;
}
