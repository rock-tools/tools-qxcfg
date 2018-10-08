#ifndef QXCFG_CONFIGURATION_HPP
#define QXCFG_CONFIGURATION_HPP

#include <string>
#include <map>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <base-logging/Logging.hpp>
#include <algorithm>

struct _xmlTextWriter;

namespace qxcfg {

typedef std::map<std::string, std::string> PropertyMap;

class Configuration
{
public:

    /**
     * Default configuration
     */
    Configuration(const std::string& path = "");

    /**
     * Set a configuration value
     */
    void setValue(const std::string& key, const std::string& value) { mProperties[key] = value; }

    /**
     *
     * \throw std::invalid_argument if key does not exist
     */
    const std::string& getValue(const std::string& key, const std::string& defaultValue = std::string()) const;

    template<typename T>
    T getValueAs(const std::string& key, const T& defaultValue = T()) const
    {
        try {
            if(!key.empty())
            {
                return boost::lexical_cast<T>( getValue(key) );
            }
        } catch(const std::invalid_argument& e)
        {
            LOG_DEBUG_S << e.what();
        }

        return defaultValue;
    }

    std::string toString() const;

    /**
     * Save configuration as XML document
     */
    void save(const std::string& filename, const std::string& encodingg = "UTF-8") const;

    /**
     * Save configuration as temporary file with an optional label
     * \return name of the temporary file
     */
    std::string saveTemp(const std::string& label = "") const;

    /**
     * Return the complete property map
     */
    const PropertyMap& getPropertyMap() const { return mProperties; }

private:
    void loadXML(const std::string& path);

    void write(_xmlTextWriter* writer, const std::map<std::string, std::map<std::string, std::string> >& elements) const;

    void writePropertyMap(_xmlTextWriter* writer, const std::map<std::string, std::string>& properties) const;

    PropertyMap mProperties;
};

template<>
bool Configuration::getValueAs(const std::string& key, const bool& defaultValue) const;

} // end namespace qxcfg
#endif // QXCFG_CONFIGURATION_HPP
