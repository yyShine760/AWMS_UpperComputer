#include "inc/widget.h"

inline void Widget::updateTable() {
    // 从json文件读出key值数据写入表格的装备编号
    if (JsonUtil::getInstance().isJsonFileExist()) {
        auto keys = JsonUtil::getInstance().getJsonKeys();
        auto values = JsonUtil::getInstance().getJsonValues();

        ui->tableWidget->setRowCount(keys.size()); // 动态设置行数

        for (int row = 0; row < keys.size(); ++row) {
            // 打印编号列
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem(keys[row]);
            tableWidgetItem->setTextAlignment(
                Qt::AlignCenter); // 设置文本居中对齐
            ui->tableWidget->setItem(
                row, 0, tableWidgetItem); // 将 key 值写入装备编号列

            // 打印员工姓名列
            tableWidgetItem = new QTableWidgetItem(values[row]);
            tableWidgetItem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 1, tableWidgetItem);
        }
    } else {
        // 如果不存在则创建
        JsonUtil::getInstance().createJsonFile();
    }
}

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 设置表格的列数和字段名
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "装备编号"
                                                             << "员工名字"
                                                             << "安全帽状态"
                                                             << "安全带状态");
    // 设置表格的列宽自适应
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    // 禁止编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 禁止自动排序
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->horizontalHeader()->setSortIndicatorShown(false);
    updateTable();
}

Widget::~Widget() {
    delete ui;
}

/*
    添加装备按钮的槽函数
    · 弹出输入对话框，获取输入的装备编号
    · 读取JSON文件，检查装备编号是否已存在
    · 如果装备编号不存在，则添加装备并保存到JSON文件
    · 如果已存在，则弹出提示框
    · 如果添加成功，则弹出提示框
*/
void Widget::on_add_facility_clicked() {
    // 判断文件是否存在，如果不存在则创建文件
    if (!JsonUtil::getInstance().isJsonFileExist()) {
        JsonUtil::getInstance().createJsonFile();
    }

    QJsonObject jsonObject = JsonUtil::getInstance().getJsonObj();
    // 清除输入中的空格和其他特殊符号
    QString getUserInput =
        QInputDialog::getText(this, "添加装备", "请输入装备编号").simplified();
    getUserInput.remove(' ');

    if (getUserInput.isEmpty()) {
        return;
    }
    // 检测编号是否存在
    if (jsonObject.contains(getUserInput)) {
        QMessageBox::warning(this, "警告", "编号已存在");
        return;
    }

    // 在JSON对象中插入一个新的键值对
    jsonObject.insert(getUserInput, QJsonValue::Null);
    JsonUtil::getInstance().rewriteJsonFile(jsonObject);
    QMessageBox::information(this, "成功", "装备已成功添加");
    updateTable();
}

/*
    绑定装备按钮的槽函数
    · 弹出输入对话框，获取输入的装备编号和绑定的员工名字
    · 读取JSON文件，绑定的员工名字是否已存在
    · 如果员工名字不存在，则将员工绑定装备编号并保存到JSON文件
    · 如果已存在，则弹出提示框
    · 如果绑定成功，则弹出提示框
*/
void Widget::on_bind_facility_clicked() {
    if (!JsonUtil::getInstance().isJsonFileExist()) {
        QMessageBox::warning(this, "警告", "暂无装备被添加");
        return;
    }

    QJsonObject jsonObj = JsonUtil::getInstance().getJsonObj();
    // 获取装备编号和员工名字
    QString deviceNumber =
        QInputDialog::getText(this, "绑定装备", "请输入装备编号:").simplified();
    deviceNumber.remove(' ');

    if (deviceNumber.isEmpty()) {
        return;
    }
    // 检查装备编号是否已存在
    if (jsonObj.contains(deviceNumber)) {
        QString employeeName =
            QInputDialog::getText(this, "绑定装备", "请输入员工名字:").simplified();
        employeeName.remove(' ');
        if (employeeName.isEmpty()) {
            return;
        }
        // 装备编号已存在，检查员工名字是否为null
        if (jsonObj[deviceNumber].isNull()) {
            // 员工名字是null，可以直接将用户输入的员工名字设置为值
            jsonObj[deviceNumber] = employeeName;
        } else {
            // 员工名字不是null，弹出警告对话框
            QMessageBox::warning(this, "警告",
                                 "装备编号已存在，且已有员工名字绑定");
            return;
        }
    } else {
        // 装备编号不存在，弹出警告对话框
        QMessageBox::warning(this, "警告", "装备编号不存在");
        return;
    }

    JsonUtil::getInstance().rewriteJsonFile(jsonObj);
    updateTable();

    // 显示绑定成功的信息
    QMessageBox::information(this, "成功", "装备已成功绑定");
}

/*
    解绑装备按钮的槽函数
    · 弹出输入对话框，获取输入的装备编号
    · 读取JSON文件，装备编号是否已存在
    · 如果装备编号不存在，则弹出提示框
    · 如果装备编号存在，则将装备编号对应的值为null并保存到JSON文件
    · 如果解绑成功，则弹出提示框
*/
void Widget::on_rebind_facility_clicked() {
    // QFile jsonFile(JSONPATH);
    // if (!jsonFile.exists()) {
    //     QMessageBox::warning(this, "警告", "暂无装备被添加");
    //     return;
    // }
    // // 打开JSON文件并读取内容
    // if (!jsonFile.open(QIODevice::ReadOnly)) {
    //     qWarning() << "无法打开JSON文件";
    //     return;
    // }
    // QByteArray jsonData = jsonFile.readAll();
    // jsonFile.close();

    // // 将JSON数据解析为JSON文档对象
    // QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    // QJsonObject jsonObject = jsonDoc.object();

    // if (jsonDoc.isNull() || !jsonDoc.isObject()) {
    //     QMessageBox::warning(this, "警告", "暂无装备被添加");
    //     return;
    // }

    // // 获取装备编号
    // QString deviceNumber =
    //     QInputDialog::getText(this, "解绑装备", "请输入装备编号:");
    // QString employeeName;
    // if (jsonObject.contains(deviceNumber)) {
    //     // 装备编号已存在，检查员工名字是否为null
    //     if (jsonObject[deviceNumber].isNull()) {
    //         QMessageBox::warning(this, "警告", "装备暂未被绑定");
    //         return;
    //     } else {
    //         employeeName = jsonObject[deviceNumber].toString();
    //         jsonObject[deviceNumber] = QJsonValue::Null;
    //     }
    // } else {
    //     // 装备编号不存在，弹出警告对话框
    //     QMessageBox::warning(this, "警告", "装备编号不存在");
    //     return;
    // }

    // // 写入更新后的JSON数据到文件
    // if (!jsonFile.open(QIODevice::WriteOnly)) {
    //     qWarning() << "无法打开JSON文件进行写入";
    //     return;
    // }
    // jsonDoc.setObject(jsonObject);
    // jsonFile.resize(0);
    // jsonFile.write(jsonDoc.toJson());
    // jsonFile.close();

    // // 显示解绑成功的信息
    // QMessageBox::information(this, "成功", employeeName +
    // "的装备已成功解绑");
}

/*
    删除装备按钮的槽函数
    · 弹出输入对话框，获取输入的装备编号
    · 读取JSON文件
    · 如果装备编号不存在，则弹出提示框
    · 如果装备编号处于绑定状态，则弹出警告对话框
    · 如果装备编号未处于绑定状态，则将装备编号键值对删除
    · 保存更新后的JSON数据到文件
    · 如果删除成功，则弹出提示框
*/
void Widget::on_del_facility_clicked() {
    // QFile jsonFile(JSONPATH);
    // if (!jsonFile.exists()) {
    //     QMessageBox::warning(this, "警告", "暂无装备被添加");
    //     return;
    // }
    // // 打开JSON文件并读取内容
    // if (!jsonFile.open(QIODevice::ReadOnly)) {
    //     qWarning() << "无法打开JSON文件";
    //     return;
    // }
    // QByteArray jsonData = jsonFile.readAll();
    // jsonFile.close();

    // // 将JSON数据解析为JSON文档对象
    // QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // if (jsonDoc.isNull() || !jsonDoc.isObject()) {
    //     QMessageBox::warning(this, "警告", "暂无装备被添加");
    //     return;
    // }
    // QJsonObject jsonObject = jsonDoc.object();

    // QString deviceNumber =
    //     QInputDialog::getText(this, "删除装备", "请输入装备编号:");
    // QString deviceNumber_t;
    // if (jsonObject.contains(deviceNumber)) {
    //     // 装备编号已存在，检查员工名字是否为null
    //     if (jsonObject[deviceNumber].isNull()) {
    //         deviceNumber_t = jsonObject[deviceNumber].toString();
    //         jsonObject.remove(deviceNumber);
    //     } else {
    //         QMessageBox::warning(this, "警告", "装备处于绑定状态无法删除");
    //         return;
    //     }
    // } else {
    //     // 装备编号不存在，弹出警告对话框
    //     QMessageBox::warning(this, "警告", "装备编号不存在");
    //     return;
    // }

    // // 写入更新后的JSON数据到文件
    // if (!jsonFile.open(QIODevice::WriteOnly)) {
    //     qWarning() << "无法打开JSON文件进行写入";
    //     return;
    // }
    // jsonDoc.setObject(jsonObject);
    // jsonFile.resize(0);
    // jsonFile.write(jsonDoc.toJson());
    // jsonFile.close();

    // // 显示解绑成功的信息
    // QMessageBox::information(this, "成功",
    //                          "编号" + deviceNumber_t + "装备已删除");
}

/*
    切换信息按钮
    1. 打开json文件
    2. 读取出一条信息
    3. 关闭文件
    4. 显示信息到界面
*/
void Widget::on_pushButton_laststaff_clicked() {
    // QFile jsonFile(JSONPATH);
    // if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     qWarning("未能打开文件");
    //     return;
    // }

    // QByteArray jsonData = jsonFile.readAll();
    // QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // if (jsonDoc.isNull() || !jsonDoc.isObject()) {
    //     qWarning("JSON 文档无效");
    //     return;
    // }
    // QJsonObject jsonObject = jsonDoc.object();

    // jsonObject
}

void Widget::on_pushButton_nextstaff_clicked() {
}
