#include "person.pb.h"
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