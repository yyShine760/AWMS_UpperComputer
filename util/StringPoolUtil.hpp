#ifndef StringPoolUtil_hpp
#define StringPoolUtil_hpp

#include <QString>
#include <unordered_map>

using std::unordered_map, std::string_view;

class StringPoolUtil {
public:
    static StringPoolUtil& getInstance() {
        static StringPoolUtil instance;
        return instance;
    }

    // 获取字符串
    const QString& get(const QString &str) {
        auto it = _strPool.find(str);
        if (it != _strPool.end()) {
            return it->second;
        } else {
            _strPool.insert({str, str});
            return str;
        }
    }

private:
    StringPoolUtil() = default;
    ~StringPoolUtil() = default;

    StringPoolUtil(const StringPoolUtil &) = delete;
    StringPoolUtil& operator=(const StringPoolUtil &) = delete;

private:
    unordered_map<QString, QString> _strPool;
};

#endif
