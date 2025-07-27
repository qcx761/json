# 序列化技术 #

##  一、序列化和反序列化

## 1.1 什么是现序列化与反序列化

### 序列化

将 **对象（如结构体、类）转换为字节流**（如字符串或二进制数据），用于：

- 网络传输 （网络只能传输字节流，需要转换）
- 文件存储 （文件系统不认识对象格式，需要序列化为文本或二进制）
- 缓存数据 （	缓存系统（如 Redis）只支持简单类型）

### 反序列化

将**字节流还原为原始对象**，用于：

- 恢复传输的数据
- 读取文件或缓存
- 与其他系统的数据交互

---

## 1.2 序列化与反序列化使用

- **原始数据**：可包括基础数据类型和复合数据类型。
- **流程**：
  ```
  发送端：
    原始数据 → 序列化（编码） → 特殊格式字符串
    发送该字符串

  接收端：
    接收特殊格式字符串 → 反序列化（解码） → 恢复原始数据
    处理原始数据
  ```

- **目的**：实现跨平台/跨终端分发，确保各端能正确解析，如网络传输、磁盘复制等。
- **注意**：序列化是为了解决数据传输，不是为了加密。

---

## 1.3 为什么需要序列化？

在网络传输中常见问题：

- **平台差异**：例如 32 位 vs 64 位机器上的结构体在内存中的对齐不同。
- **字节序问题**：不同系统字节顺序不一致（大小端）。
- **语言差异**：
  - C 的 `char` 占 1 字节；
  - Java 的 `char` 占 2 字节；
- **对齐问题**：结构体字节对齐导致可移植性差。

若直接将结构体通过 `send()` 发出，接收端可能无法正确解析，因此必须用序列化转换为稳定格式。

---

## 1.4 常用序列化方式

- XML（Extensible Markup Language）
- JSON（JavaScript Object Notation）
- Protocol Buffer（Protobuf）
- ASN.1（Abstract Syntax Notation One）
- Boost 序列化库（C++）

---
##  二 JSON

# 2.1 JSON

- JSON（JavaScript Object Notation）起源于 JavaScript 的“关联数组”，提供一种轻量级、易于人类阅读和机器解析的数据交换格式。
- **优点**：
  - 轻量简洁
    - 结构简单，格式紧凑，比 XML 更短小，节省带宽和存储空间。
  - 人类可读
    - 格式直观，键值对清晰，方便开发者阅读和调试。
  - 跨语言支持广泛
    - 绝大多数编程语言（JavaScript、Python、Java、C++、Go 等）都支持 JSON 解析和生成。
  - 解析速度快
    - 结构简单，现成的解析库众多，性能良好。
  - 灵活性强
    - 支持复杂的嵌套结构（对象、数组），满足各种数据结构需求。

---

## 2.2 JSON 数据结构

 **JSON 支持的数据类型** :数字、字符串、布尔值、空值、数组、对象

- **数字**：包括整数和浮点数，如 42 或 3.14
- **字符串**：由双引号括起来的字符序列，如 "Hello World"
- **布尔值**：即 true 或 false
- **空值**：表示空数据，即 null
- **数组**：类似于 C++ 数组，但元素类型可混合。支持如下类型：
  - `int`, `double`, `float`, `bool`, `string`, `char*`, 嵌套 JSON 数组/对象  
```cpp
[
  42,
  "hello",
  true,
  null,
  {"id": 1, "name": "Alice"},
  [1, 2, 3]
]
```
- **对象**：使用 `{}` 表示，由键（字符串）与值（任意有效类型）组成，多个键值对用逗号分隔。

```cpp
{
  "id": 1,
  "name": "Alice",
  "age": 30,
  "isStudent": false
}
```


## 2.3 使用 `jsoncpp` 类库

示例代码：
```cpp
#include <iostream>
#include <json/json.h>

int main() {
    // 创建JSON对象
    Json::Value jsonValue;
    jsonValue["name"] = "Alice";
    jsonValue["age"] = 18;
    jsonValue["city"] = "New York";

    // 将JSON对象转换为字符串
    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, jsonValue);
    std::cout << "JSON to string: " << jsonString << std::endl;

    // 将字符串转换为JSON对象
    Json::CharReaderBuilder reader;
    Json::Value parsedJson;
    std::istringstream jsonStringStream(jsonString);
    Json::parseFromStream(reader, jsonStringStream, &parsedJson, nullptr);


    // 从JSON对象中获取数据
    std::string name = parsedJson["name"].asString();
    // Json::Value value = parsedJson["name"];
    // std::string name = value.asString();
    int age = parsedJson["age"].asInt();
    std::string city = parsedJson["city"].asString();

    // 打印解析后的数据
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "City: " << city << std::endl;

    return 0;
}

```

输出：
```cpp
JSON to string: {
        "age" : 18,
        "city" : "New York",
        "name" : "Alice"
}
Name: Alice
Age: 18
City: New York
```

## 2.4 使用 nlohmann/json 类库

示例代码：
```cpp
#include <iostream>
#include <nlohmann/json.hpp>

int main() {
    // 创建JSON对象
    nlohmann::json jsonValue;
    jsonValue["name"] = "Alice";
    jsonValue["age"] = 18;
    jsonValue["city"] = "New York";

    // 将JSON对象转换为字符串
    std::string jsonString = jsonValue.dump();
    std::cout << "JSON to string: " << jsonString << std::endl;

    // 将字符串转换为JSON对象
    nlohmann::json parsedJson = nlohmann::json::parse(jsonString);

    // 从JSON对象中获取数据
    std::string name = parsedJson["name"].get<std::string>();
    // std::string name = parsedJson.value("name", "unknown");
    int age = parsedJson["age"].get<int>();
    // int age = parsedJson.value("age", 0);
    std::string city = parsedJson["city"].get<std::string>();
    // bool isStudent = parsedJson.value("is_student", false);

    // 打印解析后的数据
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "City: " << city << std::endl;

    return 0;
}
```

输出：
```cpp
JSON to string: {"age":18,"city":"New York","name":"Alice"}
Name: Alice
Age: 18
City: New York
```

---

## 对比总结：jsoncpp vs nlohmann/json

| 比较项         | jsoncpp                  | nlohmann/json                 |
|----------------|--------------------------|-------------------------------|
| 是否 Header-only |  需要编译安装           |  只需一个头文件              |
| 语法风格       | C++98 风格，显式类型转换多 | C++11/14 风格，结构直观简洁     |
| 序列化方式     | `.toStyledString()` 手动写入 | `.dump()` 直接美化输出          |
| 反序列化方式     | Reader + `.asXXX()` 类型转换 | `>>` 流操作符 + 自动类型推断    |
| STL 容器支持   |  手动构造数组、对象      |  直接支持 `vector`、`map` 等 |

---


# 三 Protobuf（Protocol Buffer）

- Google 开发的跨语言结构化数据序列化标准，适用于 RPC 和持久化数据交换。
- 优点：体积小、效率高，由 `.proto` 文件定义结构、自动生成代码支持 C++, Java, C#, Python 等语言。

## 3.1 数据组织示例

定义 `.proto` 文件：
```proto
syntax = "proto3";
message Person {
  int32 id = 1;
  string name = 2;
  string sex = 3;
  int32 age = 4;
}
```

通过：
```bash
protoc Person.proto --cpp_out=./
```
生成对应的 C++ 类。

## 3.2 使用场景示例

```cpp
#include "Person.pb.h"
#include <iostream>

int main() {
    Person p;
    p.set_id(1);
    p.set_name("李四");
    p.set_sex("man");
    p.set_age(18);

    std::string output;
    p.SerializeToString(&output);
    std::cout << "序列化后: " << output << std::endl;

    Person pp;
    pp.ParseFromString(output);
    std::cout << "名字: " << pp.name() << ", 年龄: " << pp.age()
              << ", 性别: " << pp.sex() << ", id: " << pp.id() << std::endl;
    return 0;
}
```

示例封装类：
```cpp
class My_Protobuf {
    Person m_person;
    std::string m_enstr;
public:
    My_Protobuf(const Info* info) {
        m_person.set_id(info->id);
        m_person.set_name(info->name);
        m_person.set_sex(info->sex);
        m_person.set_age(info->age);
    }
    My_Protobuf(const std::string& str) : m_enstr(str) {}
    std::string encodeMsg() {
        std::string out;
        m_person.SerializeToString(&out);
        return out;
    }
    Person* decodeMsg() {
        Person* p = new Person();
        p->ParseFromString(m_enstr);
        return p;
    }
};
```

## 3.3 编码原理简述

- **Varints 编码**：可变长度整型，优点是数字越小占字节越少，最高位作标志位。
- **ZigZag 编码**：将负整数映射为无符号正数，从而减少负数序列化长度。

---

# 4 JSON vs Protobuf 对比

## 性能与效率对比

- **Protobuf** 通常序列化速度快、体积小。对数字数据对象，可减少 60–90% 空间，解码速度优于 JSON（尤其对复杂/大数据结构）。
- **JSON** 在小型、文本密集型场景下解析性能好，但整体较慢且不紧凑。

## 易用性与可读性

- JSON：无需定义 schema，文本可读、调试方便；
- Protobuf：需要 `.proto` 文件编译，二进制不可读，但支持严格结构验证。

## 可演进性与兼容性

- JSON：动态、松散结构，缺少版本控制机制；
- Protobuf：支持版本演进、忽略未知字段、内置字段弃用机制。

## 适用场景对比

| 场景类型                  | 推荐使用         |
|---------------------------|------------------|
| Web 公共 API、配置文件、日志 | JSON             |
| 多语言、高性能、低带宽内部 RPC | Protobuf         |

---

# 总结

- **JSON**：格式简单、跨语言、可读性强，适用于快速开发、调试、Web API；
- **Protobuf**：二进制格式、体积小、性能高、支持 schema 演进，非常适合对性能和兼容性要求高的系统。



| 比较项              | JSON                 | Protobuf              |
|---------------------|----------------------|------------------------|
| 格式                | 文本格式（可读）      | 二进制格式（不可读）   |
| 体积                | 相对较大              | 更小，更紧凑           |
| 性能                | 较慢（字符串解析）     | 更快（预编译结构）     |
| 可读性              | 人类可读              | 不可读（调试困难）     |
| 跨平台/语言支持     | 非常好（原生支持）      | 非常好（需生成代码）    |
| 静态类型支持        | 弱（类型检查靠约定）   | 强（生成强类型类）     |

---


