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

## 1.2 序列化与反序列化流程

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

| 序列化方式       | 格式  | 可读性 | 体积 | 解析速度 | 支持语言 |
| ----------- | --- | --- | -- | ---- | ---- |
| JSON        | 文本  | 高   | 中  | 中    | 广泛   |
| XML         | 文本  | 中   | 大  | 慢    | 广泛   |
| Protobuf    | 二进制 | 低   | 小  | 快    | 广泛   |

---

##  二 JSON

## 2.1 JSON

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
  {"i": 1, "name": "Alice"},
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

`jsoncpp` 是一个经典的 C++ JSON 库，支持 JSON 的创建、序列化和反序列化。它不是 header-only，需要编译安装。

### 2.3.1 序列化（对象转字符串）

- 通过 `Json::Value` 创建 JSON 对象，支持键值对赋值。
- 使用 `Json::StreamWriterBuilder` 生成 Writer，将 JSON 对象转换成字符串。

示例：
```cpp
Json::Value jsonValue;
jsonValue["name"] = "Alice";
jsonValue["age"] = 18;
jsonValue["city"] = "New York";

Json::StreamWriterBuilder writer;
std::string jsonString = Json::writeString(writer, jsonValue);
std::cout << "JSON to string: " << jsonString << std::endl;
```

### 2.3.2 反序列化（字符串转对象）

- 使用 Json::CharReaderBuilder 创建 Reader，将 JSON 字符串解析成 Json::Value 对象。
- 支持 .asString()、.asInt() 等方法获取具体数据。

示例：
```cpp
Json::CharReaderBuilder reader;
Json::Value parsedJson;
std::istringstream jsonStringStream(jsonString);
Json::parseFromStream(reader, iss, &root, nullptr);

std::string name = parsedJson["name"].asString();
int age = parsedJson["age"].asInt();
std::string city = parsedJson["city"].asString();
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

nlohmann/json 是现代 C++ JSON 库，header-only，依赖 C++11 及以上，语法简洁，易用。

### 2.4.1 序列化（对象转字符串）

- 通过 nlohmann::json 类型创建 JSON 对象，支持直接赋值。
- 调用 .dump() 方法将 JSON 对象转换为字符串。

示例代码：
```cpp
nlohmann::json jsonValue;
jsonValue["name"] = "Alice";
jsonValue["age"] = 18;
jsonValue["city"] = "New York";

std::string jsonString = jsonValue.dump();
std::cout << "JSON to string: " << jsonString << std::endl;
```
### 2.4.2 反序列化（字符串转对象）

- 使用 nlohmann::json::parse() 方法将字符串转换为 JSON 对象。
- 通过 .get<T>() 方法或者 value() 函数提取数据。

示例代码：
```cpp
nlohmann::json parsedJson = nlohmann::json::parse(jsonString);

std::string name = parsedJson["name"].get<std::string>();
int age = parsedJson["age"].get<int>();
std::string city = parsedJson["city"].get<std::string>();

std::string name = parsedJson.value("name", "abc");
int age = parsedJson.value("age", 0);
std::string city = parsedJson.value("city", "123");
```

输出：
```cpp
JSON to string: {"age":18,"city":"New York","name":"Alice"}
Name: Alice
Age: 18
City: New York
```

---

# 三 Protobuf（Protocol Buffer）

- Google 开发的跨语言结构化数据序列化标准，适用于持久化数据交换，是一种高效的二进制数据格式。
- 优点：体积小、效率高，由 `.proto` 文件定义结构、自动生成代码支持 C++, Java, C#, Python 等语言。
- **优点**：
  - 体积小、传输快
    - 使用二进制格式编码，比 JSON、XML 更紧凑，网络传输效率高。
  - 强类型、安全可靠
    - 生成语言原生类型代码，支持结构验证和字段兼容性控制。
  - 跨语言支持
    - 官方支持 C++, Java, Python, Go 等多种语言，方便系统集成。

## 3.1 数据组织示例

定义 `person.proto` 文件：
```proto
// 语法版本声明，表示你当前使用的是 proto3 语法规范
syntax = "proto3";
// 数据结构声明
message Person {
  int64 id = 1; // 后面的数字是字段编号，不要求按顺序
  string name = 2;
  string sex = 3;
  int32 age = 4;
}
```

通过：
```bash
protoc Person.proto --cpp_out=./
```
生成对应的 C++ 类

| 文件名              | 作用                                                                       |
| ---------------- | ------------------------------------------------------------------------ |
| **person.pb.cc** | 源文件，包含这些 C++ 类的具体实现，包括序列化、反序列化、拷贝、比较等逻辑。                                 |
| **person.pb.h**  | 头文件，声明了 `.proto` 文件中定义的消息（message）对应的 C++ 类和相关方法。包含类成员函数、访问器（get/set）声明。 |


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

    Person pp;
    pp.ParseFromString(output);
    std::cout << "名字: " << pp.name() << ", 年龄: " << pp.age()
              << ", 性别: " << pp.sex() << ", id: " << pp.id() << std::endl;
    return 0;
}
```

```cpp
g++ main.cpp person.pb.cc -lprotobuf -o myapp
```

输出：
```cpp
名字: 李四, 年龄: 18, 性别: man, id: 1
```


封装：
```cpp
class My_Protobuf {
private:
    Person m_person;       // protobuf 生成的 Person 对象，存储消息数据
    std::string m_enstr;   // 用于存储序列化后的二进制字符串（编码后的数据）
public:
    // 构造函数1：用 Info 结构体指针初始化 m_person 对象
    My_Protobuf(const Info* info) {
        m_person.set_id(info->id);
        m_person.set_name(info->name);
        m_person.set_sex(info->sex);
        m_person.set_age(info->age);
    }

    // 构造函数2：用已经序列化好的字符串初始化 m_enstr
    My_Protobuf(const std::string& str) : m_enstr(str) {}

    // 将 m_person 序列化为字符串
    std::string encodeMsg() {
        std::string out;
        m_person.SerializeToString(&out);  // protobuf自带方法，将对象编码成二进制字符串
        return out;
    }

    // 将 m_enstr 反序列化成 Person 对象，返回指针
    Person* decodeMsg() {
        Person* p = new Person();
        p->ParseFromString(m_enstr);  // protobuf自带方法，将字符串解码成Person对象
        return p;
    }
};
```

调用：
```cpp
struct Info {
    int id;
    std::string name;
    std::string sex;
    int age;
};

Info info = {1, "Alice", "Female", 30};

// 创建 My_Protobuf 对象
My_Protobuf proto(&info);

// 获取编码后的字符串
std::string encoded = proto.encodeMsg();

// encoded 是 protobuf 编码后的二进制数据，可以发送或保存


// 假设收到 encoded 字符串
std::string received_encoded = ...;

// 用字符串构造 My_Protobuf 对象
My_Protobuf proto(received_encoded);

// 解码成 Person 对象
Person* person = proto.decodeMsg();

std::cout << "ID: " << person->id() << "\n";
std::cout << "Name: " << person->name() << "\n";

delete person;  // 手动释放

```


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
| 配置文件、日志 | JSON             |
| 多语言、高性能 | Protobuf         |

---

# 总结

| 特点   | Protobuf                             | JSON                      |
| ---- | ------------------------------------ | ------------------------- |
| 数据定义 | 先写 `.proto` 文件定义结构，然后用 `protoc` 生成代码 | 不需要额外定义，直接构造 JSON 对象即可    |
| 使用流程 | 需要编译 `.proto` 文件生成代码，才能在程序里构造和解析     | 直接用库提供的 API 创建和解析 JSON 数据 |
| 数据格式 | 二进制，体积小，解析快，适合高效传输和存储                | 文本格式，人类可读，调试方便            |
| 可维护性 | 强类型，结构固定，支持版本演进                      | 灵活但缺少严格类型，容易出现字段名错误       |

---
