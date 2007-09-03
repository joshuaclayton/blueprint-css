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

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
using namespace std;


template <class keyT, class valT>
class umap 
{ 
	typedef map<keyT,valT> StoreT;
	typedef std::vector<typename StoreT::iterator> FifoT;
	private:
		FifoT sortv;
		StoreT content;
	
	public:
		// Functions, same as map<> (simplified)
		void erase(const keyT& key)
		{
			for (typename FifoT::iterator i = sortv.begin(), e = sortv.end(); i != e; ++i)
			{
				if( (*i)->first == key)
				{
					content.erase(*i);
					sortv.erase(i);
					return;
				}
			}
		}
		
		int size()
		{
			return content.size();
		}
		
		bool empty()
		{
			return content.empty();
		}
		
		void sort()
		{
			sortv.clear();
			for(typename StoreT::iterator i = content.begin(), e = content.end(); i != e; ++i)
			{
				sortv.push_back(i);
			}
		}
		
		// Checks if the map has the key
		bool has(const keyT key)
		{
			return (content.find(key) != content.end());
		}

		void operator=(umap<keyT,valT>& set)
		{
			sortv.clear();
			content = set.content;
			for(typename map<keyT,valT>::iterator i = content.begin(); i != content.end(); ++i)
			{
				sortv.push_back(i);
			}
		}
				
		bool operator==(umap<keyT,valT>& comp) const
		{
			if(sortv.size() != comp.sortv.size())
			{
				return false;
			}
					
			for(int i = 0; i < sortv.size(); ++i)
			{
				if(sortv[i]->first != comp.sortv[i]->first)
				{
					return false;
				}
			}
			
			return (content == comp.content);
		}

		// Access the map per [] or at()  
		valT& operator[](keyT const& key )
		{
			typename StoreT::iterator mIt = content.find(key);
			if (mIt == content.end())
			{
				mIt = content.insert(typename StoreT::value_type(key, valT())).first;
				sortv.push_back(mIt);
			}
			return mIt->second;
		}

		valT& at(int index)
		{
			if(index < size() && index >= 0)
			{
				return sortv[index]->second;
			}
		} 
		
		// Iterator
		
		class iterator
		{
			friend typename umap<keyT,valT>::iterator umap<keyT,valT>::begin();
			friend typename umap<keyT,valT>::iterator umap<keyT,valT>::end();
			friend void umap<keyT,valT>::erase(const typename umap<keyT,valT>::iterator& it);
			
			private:
				typename FifoT::iterator pos;
				int num;
				int size;
				
			public:
				iterator() 
				{
					num = 0;
				}
				
				typename umap<keyT,valT>::iterator& operator= (const typename umap<keyT,valT>::iterator& iter)
				{
					pos = iter.pos;
					num = 0;
					size = 0;
					return *this;
				}
				
				bool islast()
				{
					return (num == size-1);
				}
				
				void operator++()
				{
					++pos;
					++num;
				}
				
				void operator++(int)
				{
					++pos;
					++num;
				}
				
				bool operator!= (typename umap<keyT,valT>::iterator iter)
				{
					return (iter.pos != pos); 
				}
				
				typename StoreT::iterator& operator->()
				{
					return *pos;
				}
		};

		void erase(const typename umap<keyT,valT>::iterator& it)
		{
			content.erase(*it.pos);
			sortv.erase(it.pos);
		}
		
		typename umap<keyT,valT>::iterator begin()
		{
			typename umap<keyT,valT>::iterator test;
			test.pos = sortv.begin();
			test.size = size();
			return test;
		}

		typename umap<keyT,valT>::iterator end()
		{
			typename umap<keyT,valT>::iterator test;
			test.pos = sortv.end();
			test.size = size();
			return test;
		}
};
