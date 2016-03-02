#ifndef CONFIG_H__
#define CONFIG_H__

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class Config
{
public:
	Config(string filePath);
	~Config();

	void	load();
	void	loadDefaultConfiguration();
	void	save() const;
	void	print() const;

	void	set(const string& key, const string& value);
	void	set(const string& key, bool value);
	void	set(const string& key, float value);
	void	set(const string& key, int value);
	void	set(const string& key, const char* value);

	string	get(const string& key) const;
	bool	getBool(const string& key) const;
	float	getFloat(const string& key) const;
	int		getInt(const string& key) const;

private:
	map<string,string>	m_data;
	map<string,string>	m_defaultConfiguration;
	string				m_filePath;
};
#endif
