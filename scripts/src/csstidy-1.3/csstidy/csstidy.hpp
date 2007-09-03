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
 
#ifndef HEADER_CSSTIDY
#define HEADER_CSSTIDY 

class csstidy 
{ 
	public: 
		int                        properties,selectors,input_size,output_size;
		string                     charset,namesp, css_level;
		vector<string>             import, csstemplate;
		map<int, vector<message> > logs;
		map<string, int>           settings;
	
	private:
		css_struct    css;
		vector<token> csstokens;
		string        tokens, cur_selector, cur_at, cur_property, cur_sub_value, cur_value, cur_string;
		int           line;
		vector<int>   sel_separate;

		void add_token(const token_type ttype, const string data, const bool force = false);
		void _convert_raw_css();
		
		// Add a message to the message log
		void log(const string msg, const message_type type, int iline = 0);
		
		int _seeknocomment(const int key, const int move);
		string _htmlsp(const string istring, const bool plain);	
		string optimise_subvalue(string subvalue, const string property);
		void explode_selectors();
		
		// Parses unicode notations
		string unicode(string& istring,int& i);
		
		// Checks if the chat in istring at i is a token
		bool is_token(string& istring,const int i);
						
	public:
	    csstidy();
	    	
		// Adds a property-value pair to an existing CSS structure
		void add(const string& media, const string& selector, const string& property, const string& value);
	    void copy(const string media, const string selector, const string media_new, const string selector_new);
	
		// Prints CSS code
		void print_css(string filename = "");
		
		// Parse a piece of CSS code
		void parse_css(string css_input);
		
		/* Merges properties like margin */
		void merge_4value_shorthands(string media, string selector);
		
		/* Dissolves properties like padding:10px 10px 10px to padding-top:10px;padding-bottom:10px;... */
		map<string,string> dissolve_4value_shorthands(string property, string value);
};
	    
#endif // HEADER_CSSTIDY
