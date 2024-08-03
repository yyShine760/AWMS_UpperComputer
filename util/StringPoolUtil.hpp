#ifndef StringPoolUtil_hpp
#define StringPoolUtil_hpp

#include <string_view>
#include <unordered_map>

using std::unordered_map, std::string_view;

class StringPoolUtil {
public:
    static StringPoolUtil& getInstance() {
        static StringPoolUtil instance;
        return instance;
    }

    const string_view& get(const string_view &str) {
        return _strPool[str];
    }


private:
    StringPoolUtil() {
        _strPool["装备编号"] = "装备编号";
        _strPool["员工名字"] = "员工名字";
        _strPool["安全帽状态"]  = "安全帽状态";
        _strPool["安全带状态"] = "安全带状态";
        _strPool["警告"] = "警告";
        _strPool["请输入装备编号"] = "请输入装备编号";
        _strPool["成功"] = "成功";
    };
    ~StringPoolUtil() = default;

    StringPoolUtil(const StringPoolUtil &) = delete;
    StringPoolUtil& operator=(const StringPoolUtil &) = delete;

private:
    unordered_map<string_view, string_view> _strPool;
};

#endif