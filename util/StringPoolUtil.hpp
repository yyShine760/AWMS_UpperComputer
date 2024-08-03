// #ifndef StringPoolUtil_hpp
// #define StringPoolUtil_hpp

// #include <map>

// // 单例模式的字符串常量池
// class StringPoolUtil
// {
// public:
//     static StringPoolUtil* getInstance() {
//         if (m_instance == nullptr) {
//             m_instance = new StringPoolUtil();
//         }
//         return m_instance;
//     }
    
//     const char* getString(const char* str) {
//         return m_stringPool[str];
//     }
    
// private:
//     StringPoolUtil() {
//         m_stringPool["设备管理文件路径"] = "./data/device.json";
//         m_stringPool["警告"] = "警告";

//     }
//     ~StringPoolUtil();
    
//     static StringPoolUtil* m_instance;
//     std::map<const char*, const char*> m_stringPool;
// };

// #endif