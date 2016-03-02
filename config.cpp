#include "config.h"

Config::Config(string filePath)
: m_filePath(filePath)
{
}

Config::~Config(){}

void Config::loadDefaultConfiguration()
{
	m_data = map<string,string>
	{
		// Default setting goes here
		{"window_width", "800"},
		{"window_height", "600"},
	};
}

void Config::load()
{
	std::ifstream   fileStream(m_filePath);
	string          line;

	this->loadDefaultConfiguration();

	// Create the file with default configuration if it's not there
	if (!fileStream.is_open())
		this->save();
	else
	{
		// Read file line by line
		while(std::getline(fileStream, line))
		{
			string	key;
			string	value;
			bool	keyFound     = false;
			bool	readingValue = false;

			// Read character by character
			for(char x : line)
			{
				// Comment
				if(	x == '#' || x == '!' || x == ';')
					break;

				// Ignore whitespace if we are reading the key
				else if((x == ' ' || x == '\t') && !readingValue)
					continue;

				else if(x == '=')
					keyFound = true;

				else if (!keyFound)
					key.push_back(x);

				else if (keyFound)
				{
					value.push_back(x);
					readingValue = true;
				}
			}

			// Insert {key,value} in the map
			if(keyFound)
				this->set(key, value);
		}
	}
	fileStream.close();
}

void Config::save() const
{
	std::fstream fileStream(m_filePath, std::fstream::out);

	fileStream << "# Configuration"  << std::endl;
	fileStream << std::endl;

	for(auto x : m_data)
		fileStream << x.first << " = " << x.second << std::endl;

	fileStream.close();
}

void Config::print() const
{
	for(auto x : m_data)
		std::cout << x.first << " = " << x.second << std::endl;
}

void Config::set(const string& key, const string& value)
{
	// Check if already in the map
	if (m_data.find(key) != m_data.end())
		m_data.at(key) = value;
	else
		m_data.insert({key, value});
}

void Config::set(const string& key, bool value)
{
	this->set(key, string((value) ? "true" : "false"));
}

void Config::set(const string& key, float value)
{
	this->set(key, std::to_string(value));
}

void Config::set(const string& key, int value)
{
	this->set(key, std::to_string(value));
}

void Config::set(const string& key, const char* value)
{
	this->set(key, string(value));
}

string Config::get(const string& key) const
{
	if (m_data.find(key) != m_data.end())
		return m_data.at(key);
	else
		return "";
}

bool Config::getBool(const string& key) const
{
	if(m_data.find(key) != m_data.end())
	{
		string value = m_data.at(key);
		std::transform(value.begin(), value.end(), value.begin(), tolower);

		if( value == "true" || value == "1" ||
			value == "on" 	|| value == "enable")
			return true;
	}
	return false;
}

float Config::getFloat(const string& key) const
{
	if(m_data.find(key) != m_data.end())
		return std::atof(m_data.at(key).c_str());
	else
		return 0.f;
}

int Config::getInt(const string& key) const
{
	if(m_data.find(key) != m_data.end())
		return std::atoi(m_data.at(key).c_str());
	else
		return 0;
}
