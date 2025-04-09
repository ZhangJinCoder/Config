#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>


class Config {
public:
    Config() {};
    ~Config() {};
public:
    // 设置值
    inline void set(const std::string& key, const std::string& value) {
        set("default", key, value);
    }
    inline void set(const std::string& section, const std::string& key, const std::string& value) {
        m_sections[section][key] = value;
    }
    // 获取值
    inline std::string get(const std::string& key) {
        return get("default", key); // 调用重载的get方法
    }
    inline std::string get(const std::string& section, const std::string& key) {
        auto sectionIt = m_sections.find(section);
        if (sectionIt != m_sections.end()) {
            auto keyIt = sectionIt->second.find(key);
            if (keyIt != sectionIt->second.end()) {
                return keyIt->second;
            }
        }
        return std::string();
    }
    // 保存配置文件
    inline void save(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }
        for (const auto& sectionPair : m_sections) {
            const std::string& section = sectionPair.first;
            if (section != "default") {
                file << "[" << section << "]" << std::endl;
            }
            for (const auto& keyValue : sectionPair.second) {
                file << keyValue.first << " = " << keyValue.second << std::endl;
            }
            file << std::endl;
        }
        file.close();
    }
    // 解析配置文件
    inline void parse(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }
        std::string currentSection;
        std::string line;
        while (std::getline(file, line)) {
            line = trim(line);
            // 跳过注释和空行
            if (line.empty() || line[0] == ';' || line[0] == '#') {
                continue;
            }
            // 检测节（section）
            if (line[0] == '[' && line[line.size() - 1] == ']') {
                currentSection = line.substr(1, line.size() - 2);
                continue;
            }
            // 检测键值对
            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos) {
                std::string key = trim(line.substr(0, equalPos));
                std::string value = trim(line.substr(equalPos + 1));
                if (!currentSection.empty()) {
                    m_sections[currentSection][key] = value;
                } else {
                    m_sections["default"][key] = value;
                }
            }
        }
        file.close();
    }

    // 删除键
    inline void erase(const std::string& section, const std::string& key) {
        auto sectionIt = m_sections.find(section);
        if (sectionIt != m_sections.end()) {
            sectionIt->second.erase(key);
        }
    }
    // 删除节
    inline void eraseSection(const std::string& section) {
        m_sections.erase(section);
    }

    // 输出所有键值对
    inline void print() {
        for (const auto& sectionPair : m_sections) {
            const std::string& section = sectionPair.first;
            if(section != "default") std::cout << "[" << section << "]" << std::endl; 
            for (const auto& keyValue : sectionPair.second) {
                std::cout << keyValue.first << " = " << keyValue.second << std::endl; 
            }
        } 
    }
protected:
    // 去除字符串首尾的空白字符
    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t");
        if (start == std::string::npos) return "";
        size_t end = str.find_last_not_of(" \t");
        return str.substr(start, end - start + 1);
    }

private:
    std::map<std::string, std::map<std::string, std::string>> m_sections;     // sections["section"]["key"] = "value"
};


#endif // CONFIG_H
