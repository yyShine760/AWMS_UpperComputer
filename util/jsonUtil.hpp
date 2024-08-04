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
        return _jsonFile.exists();
    }

    bool isOpenJsonFile(QIODevice::OpenMode mode) {
        if (_jsonFile.open(mode | QIODevice::Text)) {
            return true;
        } else {
            qDebug() << "Failed to open json file: " << _jsonFile.errorString();
            return false;
        }
    }

    void closeJsonFile() {
        _jsonFile.close();
    }

    void createJsonFile() {
        QDir dir;
        dir.mkpath(QDir::currentPath() + "/data");

        if (isOpenJsonFile(QIODevice::WriteOnly)) {
            closeJsonFile();
        }
    }

    QJsonObject getJsonObject() {
        if (!_isJsonObjCached) {
            if (!isJsonFileExist()) {
                createJsonFile();
            }

            if (!isOpenJsonFile(QIODevice::ReadOnly)) {
                return {};
            }

            auto _jsonData = _jsonFile.readAll();
            closeJsonFile();

            auto _jsonDoc = QJsonDocument::fromJson(_jsonData);
            if (_jsonDoc.isNull() || !_jsonDoc.isObject()) {
                _jsonObj = QJsonObject();
            } else {
                _jsonObj = _jsonDoc.object();
            }
            _isJsonObjCached = true;
        }
        return _jsonObj;
    }

    void rewriteJsonFile(const QJsonObject &jsonObj) {
        if (isOpenJsonFile(QIODevice::WriteOnly)) {
            QJsonDocument jsonDoc(jsonObj);
            _jsonFile.resize(0);
            _jsonFile.write(jsonDoc.toJson());
            closeJsonFile();

            _jsonObj = jsonObj;
            _isJsonObjCached = false;
        }
    }

    QStringList getJsonKeys() {
        return getJsonObject().keys();
    }

    QStringList getJsonValues() {
        QStringList values;
        for (const auto &key : getJsonKeys()) {
            values.append(getJsonObject().value(key).toString());
        }
        return values;
    }

private:
    JsonUtil() 
        : _jsonFile(QDir::currentPath() + "/data/device.json"),
          _isJsonObjCached(false) {}

    ~JsonUtil() = default;
    JsonUtil(const JsonUtil &) = delete;
    JsonUtil &operator=(const JsonUtil &) = delete;

    QFile _jsonFile;
    QJsonObject _jsonObj;
    bool _isJsonObjCached;
};

#endif // JSONUTIL_H
