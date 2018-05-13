#include "Configuration.hpp"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <sstream>
#include "utils/XMLUtils.hpp"
#include <boost/filesystem.hpp>

namespace qxcfg {

Configuration::Configuration(const std::string& path)
{
    loadXML(path);
}

const std::string& Configuration::getValue(const std::string& key, const std::string& defaultValue) const
{
    std::map<std::string, std::string>::const_iterator cit = mProperties.find(key);
    if(cit != mProperties.end())
    {
        return cit->second;
    }
    if(defaultValue.empty())
    {
        throw std::invalid_argument("qxcfg::Configuration::getValue: key '" + key + "' does not exist");
    } else {
        return defaultValue;
    }

}

template<>
bool Configuration::getValueAs(const std::string& key, const bool& defaultValue) const
{
    try {
        if(!key.empty())
        {
            std::string value = getValue(key);
            std::transform(value.begin(), value.end(), value.begin(), ::tolower);
            if(value == "true")
            {
                return true;
            } else if(value == "false")
            {
                return false;
            }
            throw std::runtime_error("qxcfg::Configuration: value for key '" + key + "' cannot be converted to bool, expected one of true or false");
        }
    } catch(const std::invalid_argument& e)
    {
        LOG_DEBUG_S << e.what();
    }

    return defaultValue;
}

void Configuration::loadXML(const std::string& url)
{
    if(url.empty())
    {
        return;
    }

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    // The resulting document tree
    xmlDocPtr doc = NULL;

    xmlParserOption options =  XML_PARSE_NOENT; // http://xmlsoft.org/html/libxml-parser.html#xmlParserOption

    try {
        // xmlReadFile can take filename or url
        doc = xmlReadFile(url.c_str(), NULL, options);
        if(doc == NULL)
        {
            throw std::runtime_error("qxcfg::Configuration::loadXML: Failed to parse url '" + url + "'");
        }

        //xmlChar* xpath = (xmlChar*) "//node()[not(node())]";
        xmlChar* xpath = (xmlChar*) "//*[not(*)]";
        xmlXPathContextPtr context;
        xmlXPathObjectPtr result;

        context = xmlXPathNewContext(doc);
        if (context == NULL)
        {
            throw std::runtime_error("qxcfg::Configuration::loadXML: Error in xmlXPathNewContenxt");
        }
        result = xmlXPathEvalExpression(xpath, context);
        xmlXPathFreeContext(context);
        if (result == NULL)
        {
            throw std::runtime_error("qxcfg::Configuration::loadXML: Error in xmlXPathEvalExpression");
        }
        if(xmlXPathNodeSetIsEmpty(result->nodesetval))
        {
           xmlXPathFreeObject(result);
           throw std::runtime_error("qxcfg::Configuration::loadXML: Configuration is empty");
        }

        xmlNodeSetPtr nodeset;
        xmlChar* value;

        if (result)
        {
            nodeset = result->nodesetval;
            for(int i=0; i < nodeset->nodeNr; ++i)
            {
                xmlNode* node = nodeset->nodeTab[i]->xmlChildrenNode;
                value = xmlNodeListGetString(doc,node , 1);
                std::string propertyName = utils::XMLUtils::getFullPath(doc, node);
                if( mProperties.count(propertyName) )
                {
                    throw std::runtime_error("qxcfg::Configuration::loadXML: property '" + propertyName + "' with multiple entries (while it must have unique entries)");
                } else {
                    mProperties[propertyName] = std::string( (char*) value );
                    LOG_INFO_S << "Add configuration: " << propertyName << " " << value;
                }
                xmlFree(value);
            }
            xmlXPathFreeObject (result);
        }

    } catch(const std::exception& e)
    {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        throw;
    }

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();

}

std::string Configuration::toString() const
{
    std::stringstream ss;
    for(const std::pair<std::string, std::string>& p : mProperties)
    {
        ss << p.first << " -> " << p.second << std::endl;
    }
    return ss.str();
}

void Configuration::save(const std::string& path, const std::string& encoding) const
{
    xmlTextWriterPtr writer;
    xmlDocPtr doc;

    doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
    if(doc == NULL)
    {
        throw std::runtime_error("qxcfg::Configuration::save: error creating"
                " xml document");
    }

    // Create a new XmlWriter for uri with no compression
    writer = xmlNewTextWriterDoc(&doc, 0);
    if(writer == NULL)
    {
        throw std::runtime_error("qxcfg::Configuration::save: failed to open file '"
                + path + "'");
    }

    // Start the document with the xml default for the version and given encoding
    int rc = xmlTextWriterStartDocument(writer, NULL, encoding.c_str(), NULL);
    if(rc < 0)
    {
        throw std::runtime_error("qxcfg::Configuration::save: failed to"
                " start document:  '" + path + "'");
    }

    std::map<std::string, std::map<std::string, std::string> > topLevel;
    topLevel["qxcfg"] = mProperties;

    write(writer, topLevel);

    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
    xmlSaveFileEnc(path.c_str(), doc, encoding.c_str());

    utils::XMLUtils::lint(path);
}

std::string Configuration::saveTemp(const std::string& label) const
{
    boost::filesystem::path tempDir = boost::filesystem::temp_directory_path();
    boost::filesystem::path temp = tempDir / boost::filesystem::unique_path();

    std::string filename = temp.native();
    if(!label.empty())
    {
        filename += "-" + label;
    }
    filename += ".xml";

    save(filename);
    return filename;
}

void Configuration::write(xmlTextWriterPtr writer, const std::map<std::string, std::map<std::string, std::string> >& elements) const
{
    for(const std::pair<std::string, std::map<std::string, std::string> >& p : elements)
    {
        utils::XMLUtils::startElement(writer, p.first);
        writePropertyMap(writer, p.second);
        utils::XMLUtils::endElement(writer);
    }
}

void Configuration::writePropertyMap(xmlTextWriterPtr writer, const std::map<std::string, std::string>& properties) const
{
    // to extract common high level elements and add the rest to it
    std::map<std::string, std::map<std::string, std::string> > children;

    for(const std::pair<std::string, std::string>& p : properties)
    {
        std::string label = p.first;
        std::string value = p.second;

        size_t pos = label.find_first_of("/");
        if(pos == 0)
        {
            label = label.substr(pos+1);
            pos = label.find_first_of("/");
        }

        if(pos != std::string::npos)
        {
            std::string firstElement = label.substr(0, pos);
            std::string remaining = label.substr(pos+1);
            children[firstElement].insert( std::pair<std::string, std::string>(remaining, value) );
        } else {
            // found an actual entry
            utils::XMLUtils::startElement(writer, label);
            utils::XMLUtils::writeString(writer, value);
            utils::XMLUtils::endElement(writer);
        }
    }

    write(writer, children);
}

} // end namespace qxcfg
