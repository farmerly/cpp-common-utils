#pragma once

#include "TinyXmlElement.h"
#include <memory>

namespace xmlutils {
class TinyXmlDocument
{
public:
    TinyXmlDocument();
    TinyXmlDocument(const std::string &path);
    virtual ~TinyXmlDocument();

public:
    std::string toXmlString();                          // 获取xml消息体
    bool        parseXmlString(const std::string &xml); // 写入xml消息体
    bool        loadXmlFile(const std::string &path);   // 从文件读取xml消息
    bool        saveXmlFile(const std::string &path);   // 保存xml消息到文件
    void        addDeclaration();                       // 添加声明
    bool        parseSuccess();                         // 获取是否解析成功

private:
    bool setParseResult(bool result); // 设置解析结果

public:
    std::shared_ptr<TinyXmlElement> addRootElement(std::string name); // 添加根节点
    std::shared_ptr<TinyXmlElement> getRootElement();                 // 获取根节点

private:
    tinyxml2::XMLDocument m_xmlDocument;
    bool                  m_loadSuccess;
};
} // namespace xmlutils