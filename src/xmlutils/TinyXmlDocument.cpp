#include "TinyXmlDocument.h"
#include "TinyXmlElement.h"
#include <memory>

using namespace xmlutils;

std::string TinyXmlDocument::getXmlString()
{
    tinyxml2::XMLPrinter printer;
    m_doc.Print(&printer);
    return printer.CStr();
}

bool TinyXmlDocument::setXmlString(std::string xml)
{
    return tinyxml2::XML_SUCCESS == m_doc.Parse(xml.c_str());
}

bool TinyXmlDocument::loadXmlFile(std::string path)
{
    return tinyxml2::XML_SUCCESS == m_doc.LoadFile(path.c_str());
}

bool TinyXmlDocument::saveXmlFile(std::string path)
{
    return tinyxml2::XML_SUCCESS == m_doc.SaveFile(path.c_str());
}

void TinyXmlDocument::addDeclaration()
{
    tinyxml2::XMLDeclaration *declaration = m_doc.NewDeclaration();
    m_doc.InsertFirstChild(declaration);
}

std::shared_ptr<TinyXmlElement> TinyXmlDocument::addRootElement(std::string name)
{
    tinyxml2::XMLElement *root = m_doc.NewElement(name.c_str());
    if (!m_doc.InsertEndChild(root)) {
        return nullptr;
    }
    return std::make_shared<TinyXmlElement>(root);
}

std::shared_ptr<TinyXmlElement> TinyXmlDocument::getRootElement()
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_doc.RootElement())) {
        return nullptr;
    }
    return std::make_shared<TinyXmlElement>(element);
}