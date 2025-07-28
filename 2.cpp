#include <iostream>
#include <jsoncpp/json/json.h>

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
    int age = parsedJson["age"].asInt();
    std::string city = parsedJson["city"].asString();

    // 打印解析后的数据
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "City: " << city << std::endl;

    return 0;
}