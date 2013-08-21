#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

namespace Feanwork
{
	class Parser
	{
	public:
		Parser(string _file);
		~Parser();

		string	 getNextKeyword();
		unsigned getNumParams();
		bool	 checkNumParams(unsigned _num);

		int		 getInt(unsigned _param);
		unsigned getUnsigned(unsigned _param);
		bool	 getBool(unsigned _param);
		string   getString(unsigned _param);
		float	 getFloat(unsigned _param);

	protected:
		void   skipWhitespaceChars();
		string trimLine(string _line);
		void   splitParams(string _params); //Only call on trimmed line

	protected:
		ifstream	   mStream;
		vector<string> mParams;
	};
}

#endif
