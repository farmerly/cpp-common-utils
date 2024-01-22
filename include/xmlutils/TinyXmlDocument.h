#pragma once

#include "TinyXmlElement.h"
#include <memory>

namespace xmlutils {
class TinyXmlDocument
{
public:
    std::string getXmlString();                // 获取xml消息体
    bool        setXmlString(std::string xml); // 写入xml消息体
    bool        loadXmlFile(std::string path); // 从文件读取xml消息
    bool        saveXmlFile(std::string path); // 保存xml消息到文件
    void        addDeclaration();              // 添加声明

public:
    std::shared_ptr<TinyXmlElement> addRootElement(std::string name); // 添加根节点
    std::shared_ptr<TinyXmlElement> getRootElement();                 // 获取根节点

private:
    tinyxml2::XMLDocument m_doc;
};
} // namespace xmlutils