## C++ 使用 JSON 的超详尽教程（基于 nlohmann/json）

---

### 1. 引言与 JSON 简介

**JSON（JavaScript Object Notation）** 是一种轻量级的数据交换格式，广泛用于 Web、移动、服务器、桌面等各种应用中，用于结构化数据的表达和通信。

在 C++ 中，由于语言本身的复杂性和类型系统，解析和生成 JSON 曾经是一个繁琐的任务。而 `nlohmann/json` 库极大地简化了这一过程，使 JSON 像使用标准容器一样直观。

---

### 2. nlohmann/json 概述

* **项目地址**：[https://github.com/nlohmann/json](https://github.com/nlohmann/json)
* **作者**：Niels Lohmann
* **开源协议**：MIT
* **特点**：

  * 单头文件实现
  * 类似 Python / JavaScript 的语法
  * 自动类型推导
  * 支持 STL 容器和用户自定义类型

---

### 3. 安装与配置

#### 3.1 单头文件方式（推荐）

下载 `json.hpp` 文件，放入项目中并引入：

```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;
```

#### 3.2 使用 vcpkg 安装

```bash
vcpkg install nlohmann-json
```

然后在 CMakeLists.txt 中：

```cmake
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
```

#### 3.3 使用 Conan 安装

```bash
[requires]
nlohmann_json/3.11.2
```

---

### 4. JSON 数据结构与语法基础

#### JSON 支持的基本类型：

* null
* boolean（true / false）
* number（整数 / 浮点）
* string
* array（数组）
* object（键值对）

#### nlohmann::json 映射：

```cpp
json null_value = nullptr;
json boolean_value = true;
json number_value = 42;
json float_value = 3.14;
json string_value = "Hello";
json array_value = {1, 2, 3};
json object_value = {{"key", "value"}, {"num", 100}};
```

---

### 5. 基本操作

#### 5.1 创建 JSON 对象

```cpp
json j;
j["name"] = "Alice";
j["age"] = 25;
j["skills"] = {"C++", "Python"};
```

#### 5.2 访问与修改成员

```cpp
std::string name = j["name"];
int age = j["age"];
j["age"] = 30; // 修改值
```

#### 5.3 遍历 JSON 对象

```cpp
for (auto& [key, value] : j.items()) {
    std::cout << key << ": " << value << std::endl;
}
```

#### 5.4 判断字段是否存在

```cpp
if (j.contains("name")) {
    std::cout << "Name is: " << j["name"] << std::endl;
}
```

---

### 6. JSON 与 STL 类型自动转换

```cpp
std::vector<int> v = {1, 2, 3};
json jv = v;
std::vector<int> v2 = jv.get<std::vector<int>>();

std::map<std::string, int> m = {{"a", 1}, {"b", 2}};
json jm = m;
```

---

### 7. 序列化与反序列化

#### 7.1 序列化为字符串

```cpp
json j = {{"lang", "C++"}, {"year", 2025}};
std::string dumped = j.dump();
```

#### 7.2 带缩进输出

```cpp
std::cout << j.dump(4) << std::endl; // 缩进为 4
```

#### 7.3 从字符串解析

```cpp
std::string raw = R"({"name": "Bob", "score": 88})";
json parsed = json::parse(raw);
```

#### 7.4 从文件读取 / 写入

```cpp
#include <fstream>

std::ifstream in("data.json");
json j;
in >> j;

std::ofstream out("output.json");
out << j.dump(4);
```

---

### 8. 自定义类型支持

#### 定义结构体

```cpp
struct Person {
    std::string name;
    int age;
};
```

#### 实现 to\_json / from\_json

```cpp
void to_json(json& j, const Person& p) {
    j = json{{"name", p.name}, {"age", p.age}};
}

void from_json(const json& j, Person& p) {
    j.at("name").get_to(p.name);
    j.at("age").get_to(p.age);
}
```

#### 使用示例

```cpp
Person p{"Tom", 30};
json j = p;
Person p2 = j.get<Person>();
```

---

### 9. 高级用法

#### 9.1 JSON Pointer

```cpp
json j = R"({"person": {"name": "Alice"}})"_json;
std::string name = j["/person/name"_json_pointer];
```

#### 9.2 JSON Patch

```cpp
json source = R"({"a": 1})"_json;
json patch = R"([{ "op": "replace", "path": "/a", "value": 2 }])"_json;
json result = source.patch(patch);
```

#### 9.3 合并 Patch

```cpp
json j1 = {"a": 1, "b": 2};
json j2 = {"b": 3, "c": 4};
j1.merge_patch(j2);
```

---

### 10. 错误处理与异常机制

常见异常类型：

* `json::parse_error`
* `json::type_error`
* `json::out_of_range`

示例：

```cpp
try {
    json j = json::parse("not valid json");
} catch (const json::parse_error& e) {
    std::cerr << e.what() << std::endl;
}
```

---

### 11. 性能优化建议

* 使用 `emplace_back` 和 `emplace` 避免额外拷贝
* 避免频繁解析 JSON 字符串（可缓存）
* 使用 `move` 语义减少深拷贝
* 优先使用 `contains()` 判断 key 存在性，避免异常

---

### 12. 编译器兼容性与跨平台说明

* 支持 C++11 及以上
* 测试通过编译器：GCC, Clang, MSVC
* 兼容平台：Linux, Windows, macOS, FreeBSD

---

### 13. 实战案例

#### 13.1 游戏配置文件

```cpp
json config = {
    {"resolution", {"width", 1920, "height", 1080}},
    {"fullscreen", true},
    {"volume", 0.8}
};
```

#### 13.2 网络传输（序列化消息）

```cpp
struct Packet {
    std::string type;
    json payload;
};

json j = {{"type", "login"}, {"payload", {"user", "abc", "pwd", "123"}}};
```

#### 13.3 嵌套对象与 vector 容器

```cpp
json j = {
    {"users", {
        {"id", 1},
        {"name", "Alice"}
    }}
};
```

---

### 14. 单元测试与调试技巧

* 使用 `doctest` / `Catch2` / `GoogleTest` 测试 `to_json / from_json`
* 输出时使用 `j.dump(4)` 便于人类可读
* 判断 key 是否存在前使用 `contains()`

---

### 15. 替代方案比较

| 库名            | 速度    | 易用性   | 备注      |
| ------------- | ----- | ----- | ------- |
| nlohmann/json | ⭐⭐    | ⭐⭐⭐⭐⭐ | 最易用     |
| RapidJSON     | ⭐⭐⭐⭐⭐ | ⭐⭐    | C 风格，极快 |
| Boost.JSON    | ⭐⭐⭐⭐  | ⭐⭐    | 标准化趋势   |

---

### 16. 附录

#### 16.1 常用 API

```cpp
j.dump();          // 转为字符串
j.dump(4);         // 缩进格式化
json::parse(str);  // 从字符串解析
j.at("key");       // 安全访问
j.contains("key"); // 判断键是否存在
```

#### 16.2 示例项目

* [https://github.com/nlohmann/json](https://github.com/nlohmann/json)
* [https://github.com/miloyip/rapidjson](https://github.com/miloyip/rapidjson)

#### 16.3 常见问题 FAQ

* Q: 如何支持中文？
  A: dump 支持 UTF-8 编码，确保控制台支持即可。
* Q: 是否支持流式解析？
  A: 支持输入输出流对象（fstream, istringstream 等）。

---

以上内容涵盖了从基础到进阶的 C++ JSON 使用全流程。
