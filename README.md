# JSON 使用介绍

## 什么是 JSON？

**JSON**（JavaScript Object Notation）是一种轻量级的数据交换格式，  
可读性强，结构清晰，广泛应用于前后端通信、配置文件和数据存储。

它的基本结构包括：

- **对象（Object）**：键值对集合，使用 `{}` 包裹；
- **数组（Array）**：值的有序集合，使用 `[]` 包裹；
- **值（Value）**：可以是字符串、数字、布尔值、数组、对象或 `null`；
- **键（Key）**：必须是双引号包裹的字符串。

示例：

```json
{
  "name": "Alice",
  "age": 25,
  "isStudent": false,
  "skills": ["C++", "JavaScript"],
  "address": {
    "city": "Shanghai",
    "postalCode": "200000"
  }
}
JSON 的使用场景
配置文件：如 ESLint、VSCode、Prettier 等工具的 .json 配置文件；

前后端通信：Web 应用中前端与后端通常使用 JSON 进行数据交互；

数据存储：适合存储结构化、轻量级的数据，便于持久化或跨平台传输；

日志与序列化：JSON 常用于将对象序列化为文本或记录事件日志。

C++ 中如何解析 JSON
推荐使用开源库 nlohmann/json。

安装方式（vcpkg）：
bash
复制
编辑
vcpkg install nlohmann-json
或直接使用头文件：

cpp
复制
编辑
#include <nlohmann/json.hpp>
using json = nlohmann::json;
示例代码：
cpp
复制
编辑
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    // 从字符串解析 JSON
    std::string raw = R"({
        "name": "Alice",
        "age": 25,
        "skills": ["C++", "Python"],
        "isStudent": false
    })";

    json data = json::parse(raw);

    std::cout << "Name: " << data["name"] << std::endl;
    std::cout << "Age: " << data["age"] << std::endl;
    std::cout << "Is Student: " << std::boolalpha << data["isStudent"] << std::endl;

    std::cout << "Skills: ";
    for (const auto& skill : data["skills"]) {
        std::cout << skill << " ";
    }
    std::cout << std::endl;

    return 0;
}
JSON 的优势
✅ 轻量：格式简单，结构紧凑；

✅ 可读性强：易于理解和手动编辑；

✅ 语言无关：支持多种语言（C++、Python、JavaScript 等）；

✅ 易解析：配合库能快速转换为对象结构；

✅ 人机兼容：既适合机器处理，也适合人工维护；

✅ 广泛支持：浏览器、服务端、嵌入式系统均支持。

与其他存储方式对比
特性	JSON	XML	YAML	数据库（关系型）
可读性	✅ 高	❌ 中等，冗长	✅ 高	❌ 差（面向结构体）
嵌套结构支持	✅ 支持	✅ 支持	✅ 支持	部分支持（需设计表结构）
注释支持	❌ 不支持	✅ 支持	✅ 支持	❌ 不支持
第三方库支持	✅ 丰富	✅ 丰富	❌ 相对较少	✅ 非常丰富
适用场景	配置、通信、缓存	文档、配置	DevOps、配置中心	事务型数据、持久化存储

注意事项
❗ JSON 不支持注释，请勿在 .json 文件中添加 // 或 #；

❗ 键必须使用双引号包裹，如 "name"；

❗ 不支持循环引用，应避免对象间互相嵌套导致死循环；

❗ 二进制数据、日期时间等特殊类型需转换为字符串；

❗ 对于超大数据或结构复杂数据，应考虑使用数据库或分块存储方案。










| 特点           | `cin`                    | `readline`                          |
| ------------ | ------------------------ | ----------------------------------- |
| **缓冲区管理**    | 不管理用户输入缓冲区，直接读终端行缓冲内容    | 自己维护一个输入缓冲区，支持行编辑和历史                |
| **输入处理时机**   | 标准输入是行缓冲模式，只有按回车才把内容送给程序 | 同样按回车才返回整行给程序，但提供API访问和修改内部缓冲       |
| **功能**       | 仅读取输入数据，功能简单             | 支持光标移动、编辑、历史、补全，能动态操作缓冲区内容          |
| **实时获取输入内容** | 不支持，只能等回车后读取             | 支持，可通过 `rl_copy_text()` 等接口实时访问编辑内容 |
