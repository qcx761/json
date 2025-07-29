class My_Protobuf {
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