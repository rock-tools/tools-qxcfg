#ifndef QXCFG_UTILS_XML_UTILS_HPP
#define QXCFG_UTILS_XML_UTILS_HPP

#include <string>
#include <sstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlwriter.h>
#include <boost/lexical_cast.hpp>

#define QXCFG_XML_RESULT_CHECK(x, msg) \
    if(x < 0) \
    {\
        throw std::runtime_error("qxcfg::utils::XMLUtils:" \
            " xml operation failed: " #msg ); \
    };

namespace qxcfg {
namespace utils {

/**
 * \class XMLUtils
 * \brief Provide a set of helper functions to parse XML files and extract
 * information from xml nodes
 */
class XMLUtils
{
public:
    /**
     * Get name of the node
     * \param node xml node to retrieve the name from
     * \param useNamespace whether to prepend the namespace or not
     * \return name
     */
    static std::string getName(xmlNodePtr node, bool useNamespace = false);

    /**
     * Check if the name of the node matches the given string
     * \param node xml node to retrieve the name from
     * \param name name to match against the name of the given node
     * \param useNamespace whether to prepend the namespace or not
     * \return true if name matches, false otherwise
     */
    static bool nameMatches(xmlNodePtr node, const std::string& name, bool useNamespace = false);

    static std::string resolveNamespacePrefix(xmlDocPtr doc, xmlNodePtr node, const std::string& prefix);

    static std::string getContent(xmlDocPtr doc, xmlNodePtr node, size_t count = 1, bool plainTxt = false);

    static bool hasContent(xmlDocPtr doc, xmlNodePtr node, size_t count = 1);

    static std::string getProperty(xmlNodePtr node, const std::string& name);

    static std::string getFullPath(xmlDocPtr doc, xmlNodePtr node, const std::string& separator = "/", bool includeRoot = false);

    template <typename T>
    static T getNumericProperty(xmlNodePtr node, const std::string& name)
    {
        std::string propertyTxt = getProperty(node, name);
        return boost::lexical_cast<T>(propertyTxt);
    }

    /**
     * Retrieve the content of a named subnode
     * \param doc xml document
     * \param node the current node
     * \param name the name of the subnode
     * \throws std::invalid_argument if subnode of given name cannot be found
     */
    static std::string getSubNodeContent(xmlDocPtr doc, xmlNodePtr node, const std::string& name);

    static xmlChar* convertInput(const char* in, const char* encoding);

    static void writeComment(xmlTextWriterPtr writer, const std::string& comment, const std::string& encoding = "UTF-8");
    static void writeCDATA(xmlTextWriterPtr writer, const std::string& cdata, const std::string& encoding = "UTF-8");
    static void writeString(xmlTextWriterPtr writer, const std::string& string, const std::string& encoding = "UTF-8");

    static void startElement(xmlTextWriterPtr writer, const std::string& element);
    static void endElement(xmlTextWriterPtr writer);

    template<typename T>
    static void writeAttribute(xmlTextWriterPtr writer, const std::string& key, T value)
    {
        std::stringstream ss;
        ss << value;
        QXCFG_XML_RESULT_CHECK( xmlTextWriterWriteAttribute(writer, BAD_CAST key.c_str(), BAD_CAST ss.str().c_str()), writeAttribute );
    }

    /**
     * Perform linting of given xml file
     */
    static void lint(const std::string& path);
};

} // end namespace utils
} // end namespace qxcfg
#endif // QXCFG_UTILS_XML_UTILS_HPP
