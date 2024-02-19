#include "TinyXmlDocument.h"
#include "TinyXmlElement.h"
#include <memory>

using namespace xmlutils;

TinyXmlDocument::TinyXmlDocument()
{
    m_loadSuccess = false;
}

TinyXmlDocument::TinyXmlDocument(const std::string &path)
{
    auto retCode = m_xmlDocument.LoadFile(path.c_str());
    setParseResult(retCode == tinyxml2::XML_SUCCESS);
}

TinyXmlDocument::~TinyXmlDocument()
{
}

std::string TinyXmlDocument::toXmlString()
{
    tinyxml2::XMLPrinter printer;
    m_xmlDocument.Print(&printer);
    return printer.CStr();
}

bool TinyXmlDocument::parseXmlString(const std::string &xml)
{
    auto retCode = m_xmlDocument.Parse(xml.c_str());
    return setParseResult(retCode == tinyxml2::XML_SUCCESS);
}

bool TinyXmlDocument::loadXmlFile(const std::string &path)
{
    auto retCode = m_xmlDocument.LoadFile(path.c_str());
    return setParseResult(retCode == tinyxml2::XML_SUCCESS);
}

bool TinyXmlDocument::saveXmlFile(const std::string &path)
{
    auto retCode = m_xmlDocument.SaveFile(path.c_str());
    return retCode == tinyxml2::XML_SUCCESS;
}

void TinyXmlDocument::addDeclaration()
{
    auto declaration = m_xmlDocument.NewDeclaration();
    m_xmlDocument.InsertFirstChild(declaration);
}

bool TinyXmlDocument::parseSuccess()
{
    return m_loadSuccess;
}

bool TinyXmlDocument::setParseResult(bool result)
{
    return m_loadSuccess = result;
}

std::shared_ptr<TinyXmlElement> TinyXmlDocument::addRootElement(std::string name)
{
    auto root = m_xmlDocument.NewElement(name.c_str());
    if (!m_xmlDocument.InsertEndChild(root)) {
        return nullptr;
    }
    return std::make_shared<TinyXmlElement>(root);
}

std::shared_ptr<TinyXmlElement> TinyXmlDocument::getRootElement()
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_xmlDocument.RootElement())) {
        return nullptr;
    }
    return std::make_shared<TinyXmlElement>(element);
}
