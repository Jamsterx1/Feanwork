#include "Parser.h"

namespace Feanwork
{
	Parser::Parser(string _file)
	{
		mStream.open(_file);
		if(!mStream.is_open())
			cout << "Could not open file " << _file << " for parsing\n";
	}

	Parser::~Parser()
	{
		if(mStream.is_open())
			mStream.close();
	}

	string Parser::getNextKeyword()
	{
		if(!mStream.is_open())
			return "";

		string buffer = "";
		do
		{
			buffer = "";
			skipWhitespaceChars();
			getline(mStream, buffer);
			buffer = trimLine(buffer);
		}
		while(buffer[0] == '~'); //Ignore comments

		if(buffer[0] != '#')
		{
			cout << "could not locate next keyword\n";
			return "";
		}
		else
			buffer.erase(buffer.begin());

		unsigned pos = buffer.find_first_of(" ");
		splitParams(buffer.substr(pos + 1));

		return buffer.substr(0, pos);
	}

	unsigned Parser::getNumParams()
	{
		return mParams.size();
	}

	bool Parser::checkNumParams(unsigned _num)
	{
		return (_num == getNumParams());
	}

	int	Parser::getInt(unsigned _param)
	{
		if(_param >= mParams.size())
		{
			cout << "parameter out of vector range\n";
			return 0;
		}

		return stoi(mParams[_param]);
	}

	unsigned Parser::getUnsigned(unsigned _param)
	{
		if(_param >= mParams.size())
		{
			cout << "parameter out of vector range\n";
			return 0;
		}

		return stoul(mParams[_param]);
	}

	bool Parser::getBool(unsigned _param)
	{
		if(_param >= mParams.size())
		{
			cout << "parameter out of vector range\n";
			return 0;
		}

		return (mParams[_param].compare("true") != 0);
	}

	string Parser::getString(unsigned _param)
	{
		if(_param >= mParams.size())
		{
			cout << "parameter out of vector range\n";
			return 0;
		}

		return mParams[_param];
	}

	float Parser::getFloat(unsigned _param)
	{
		if(_param >= mParams.size())
		{
			cout << "parameter out of vector range\n";
			return 0;
		}

		return stof(mParams[_param]);
	}

	void Parser::skipWhitespaceChars()
	{
		if(!mStream.is_open())
			return;

		//Skip all whitespaces, newlines & tabs
		char c = mStream.peek();
		while(!mStream.eof() && (c == ' ' || c == '\t' || c == '\n'))
		{
			mStream.get();
			c = mStream.peek();
		}
	}

	string Parser::trimLine(string _line)
	{
		//Remove all whitespace characters from the string
		//except for the first whitespace after the keyword
		bool keyword = false;
		string res   = "";

		for(auto c: _line)
		{
			if(c == ' ' || c == '\t')
			{
				if(keyword)
				{
					res += ' ';
					keyword = false;
				}
				continue;
			}
			else if(c == '#')
				keyword = true;

			res += c;
		}

		return res;
	}

	void Parser::splitParams(string _params)
	{
		mParams.clear();
		string param = "";

		for(auto c: _params)
		{
			if(c == ',')
			{
				mParams.push_back(param);
				param = "";
			}
			else
				param += c;
		}

		//Make sure to push back the last param as well
		mParams.push_back(param);
	}
}
