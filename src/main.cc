#include <iostream>
#include "config.h"

int main(int argc, char *argv[]) {
    Config config;
    config.set("name", "test");     // 设置默认配置项
    config.set("age", "18");        // 设置默认配置项
    config.set("section1", "key1", "value1"); // 设置section1的配置项
    config.set("section1", "key2", "value2"); // 设置section1的配置项
    config.save("test.ini");        // 保存配置文件

    Config config2;
    config2.parse("test.ini");          // 解析配置文件
    // config2.print();                 // 打印配置文件内容
    std::cout << "name: " << config2.get("name") << std::endl;  // 获取默认配置项
    std::cout << "age: " << config2.get("age") << std::endl;    // 获取默认配置项
    std::cout << "section1.key1: " << config2.get("section1", "key1") << std::endl; // 获取section1的配置项
    std::cout << "section1.key2: " << config2.get("section1", "key2") << std::endl; // 获取section1的配置项

    return 0;
}