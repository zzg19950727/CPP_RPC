#ifndef CONFIG_READ_H
#define CONFIG_READ_H
#include <string>
#include <map>

class Config
{
public:
	Config(const char* path);
	~Config();
	std::string value(const std::string& key)const;

private:
	void read_file(const char* path);
	
	std::map<std::string, std::string> m_keys;
};

#endif	//CONFIG_READ_H
