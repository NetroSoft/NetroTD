#ifndef CONFIGLOADER_H__
#define CONFIGLOADER_H__

#include <string>
#include <map>

typedef pair<const std::string, int> Config;
class ConfigLoader{
    public:
        void load_config(std::string filepath);
        void save_config();

        Config GetConfig(std::string key) const;
    private:
        std::map<std::string, int> m_configs;
};
#endif // !CONFIGLOADER_H__
