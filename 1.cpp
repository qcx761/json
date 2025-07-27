#include <iostream>
#include <nlohmann/json.hpp>

int main() {
    // 创建JSON对象
    nlohmann::json jsonValue;
    jsonValue["name"] = "John";
    jsonValue["age"] = 30;
    jsonValue["city"] = "New York";

    // 将JSON对象转换为字符串
    std::string jsonString = jsonValue.dump();
    std::cout << "JSON to string: " << jsonString << std::endl;

    // 将字符串转换为JSON对象
    nlohmann::json parsedJson = nlohmann::json::parse(jsonString);

    // 从JSON对象中获取数据
    std::string name = parsedJson["name"].get<std::string>();
    int age = parsedJson["age"].get<int>();
    std::string city = parsedJson["city"].get<std::string>();

    // 打印解析后的数据
    std::cout << "Parsed JSON:" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "City: " << city << std::endl;

    return 0;
}