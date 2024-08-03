#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

class JsonUtil {
public:
    static JsonUtil &getInstance() {
        static JsonUtil instance;
        return instance;
    }

    bool isJsonFileExist() const {
        return jsonFile.exists();
    }

    bool isOpenJsonFile(QIODeviceBase::OpenModeFlag mode) {
        if (jsonFile.open(mode | QIODevice::Text)) {
            return true;
        } else {
            qDebug() << "打开json文件失败，" + jsonFile.errorString();
            return false;
        }
    }

    void closeJsonFile() {
        jsonFile.close();
    }

    void createJsonFile() {
        // 创建路径
        QDir dir;
        dir.mkpath(QDir::currentPath() + "/data");

        isOpenJsonFile(QIODevice::WriteOnly);
        closeJsonFile();
    }

    QJsonObject getJsonObj() {
        if (!isJsonFileModified && !jsonObj.isEmpty()) {
            // 文件暂未需要修改，返回缓存的 JSON 对象
            return jsonObj;
        }
        

        // 感觉用不到isJsonFileModified标志位，直接把下面这段放到构造函数里
        if (!isOpenJsonFile(QIODevice::ReadOnly)) {
            return {};
        }

        jsonData = jsonFile.readAll();
        closeJsonFile();
        jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isNull() || !jsonDoc.isObject()) {
            jsonObj = QJsonObject();
        } else {
            jsonObj = jsonDoc.object();
        }
        isJsonFileModified = false; // 重置修改标志
        return jsonObj;
    }

    void rewriteJsonFile(const QJsonObject &jsonObj) {
        if (isOpenJsonFile(QIODevice::WriteOnly)) {
            // 清空JSON文档
            jsonDoc.setObject(QJsonObject());
            // 将更新后的JSON对象设置回JSON文档
            jsonDoc.setObject(jsonObj);
            // 清空文件内容，准备写入更新后的JSON数据
            jsonFile.resize(0);
            // 将JSON文档对象转换为JSON数据，并写入文件
            jsonFile.write(jsonDoc.toJson());
            // 关闭文件
            closeJsonFile();

            // 更新缓存
            this->jsonObj = jsonObj;
            isJsonFileModified = false; // 文件内容已更新，重置修改标志
        }
    }

    QStringList getJsonKeys() {
        return getJsonObj().keys();
    }

    QStringList getJsonValues() {
        QStringList values{};
        for (const auto &key : getJsonKeys()) {
            values.append(getJsonObj().value(key).toString());
        }
        return values;
    }

private:
    JsonUtil() = default;
    ~JsonUtil() = default;
    JsonUtil(const JsonUtil &) = delete;
    JsonUtil &operator=(const JsonUtil &) = delete;

    QFile jsonFile{QDir::currentPath() + "/data/device.json"};
    QJsonObject jsonObj{}; // 用于缓存json数据
    QJsonDocument jsonDoc{};
    QByteArray jsonData{};
    bool isJsonFileModified = true; // 初始化为 true，确保第一次读取文件
};

#endif // JSONUTIL_H
