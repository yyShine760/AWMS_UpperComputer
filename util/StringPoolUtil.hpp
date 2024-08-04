#ifndef StringPoolUtil_hpp
#define StringPoolUtil_hpp

#include <QString>
#include <QHash>

inline const QString WARNING = "警告";
inline const QString SUCCEED = "成功";
inline const QString NOFACILITY = "暂无装备被添加";
inline const QString NONUMBER = "装备编号不存在";
inline const QString INPUTFACILITY = "请输入装备编号：";

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
            return it.value();
        } else {
            _strPool.emplace(str, str);
            return _strPool[str];
        }
    }

private:
    StringPoolUtil() = default;
    ~StringPoolUtil() = default;

    StringPoolUtil(const StringPoolUtil &) = delete;
    StringPoolUtil& operator=(const StringPoolUtil &) = delete;

private:
    QHash<QString, QString> _strPool;
};

#endif
