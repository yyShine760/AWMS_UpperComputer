#include "inc/widget.h"
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 绑定两个按钮为一个槽函数
    connect(ui->pushButton_laststaff, &QPushButton::clicked, this,
            &Widget::on_switchoverButton_clicked);
    connect(ui->pushButton_nextstaff, &QPushButton::clicked, this,
            &Widget::on_switchoverButton_clicked);

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

    auto jsonObject = JsonUtil::getInstance().getJsonObject();
    // 清除输入中的空格和其他特殊符号
    QString getUserInput =
        QInputDialog::getText(this, "添加装备",
                              StringPoolUtil::getInstance().get(INPUTFACILITY))
            .simplified();
    getUserInput.remove(' ');

    if (getUserInput.isEmpty()) {
        return;
    }
    // 检测编号是否存在
    if (jsonObject.contains(getUserInput)) {
        QMessageBox::warning(this, StringPoolUtil::getInstance().get(WARNING),
                             "编号已存在");
        return;
    }

    // 在JSON对象中插入一个新的键值对
    jsonObject.insert(getUserInput, QJsonValue::Null);
    JsonUtil::getInstance().rewriteJsonFile(jsonObject);
    QMessageBox::information(this, StringPoolUtil::getInstance().get(SUCCEED),
                             "装备已成功添加");
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
    auto jsonObject = JsonUtil::getInstance().getJsonObject();

    if (!JsonUtil::getInstance().isJsonFileExist() || jsonObject.isEmpty()) {
        QMessageBox::warning(this, StringPoolUtil::getInstance().get(WARNING),
                             StringPoolUtil::getInstance().get(NOFACILITY));
        return;
    }

    // 获取装备编号和员工名字
    QString deviceNumber =
        QInputDialog::getText(this, "绑定装备",
                              StringPoolUtil::getInstance().get(INPUTFACILITY))
            .simplified();
    deviceNumber.remove(' ');

    if (deviceNumber.isEmpty()) {
        return;
    }
    // 检查装备编号是否已存在
    if (jsonObject.contains(deviceNumber)) {
        QString employeeName =
            QInputDialog::getText(this, "绑定装备", "请输入员工名字：")
                .simplified();
        employeeName.remove(' ');
        if (employeeName.isEmpty()) {
            return;
        }
        // 装备编号已存在，检查员工名字是否为null
        if (jsonObject[deviceNumber].isNull()) {
            // 员工名字是null，可以直接将用户输入的员工名字设置为值
            jsonObject[deviceNumber] = employeeName;
        } else {
            // 员工名字不是null，弹出警告对话框
            QMessageBox::warning(this,
                                 StringPoolUtil::getInstance().get(WARNING),
                                 "装备已被员工绑定");

            return;
        }
    } else {
        // 装备编号不存在，弹出警告对话框
        QMessageBox::warning(this, StringPoolUtil::getInstance().get(WARNING),
                             StringPoolUtil::getInstance().get(NONUMBER));
        return;
    }

    JsonUtil::getInstance().rewriteJsonFile(jsonObject);
    updateTable();

    // 显示绑定成功的信息
    QMessageBox::information(this, StringPoolUtil::getInstance().get(SUCCEED),
                             "装备已成功绑定");
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
    auto jsonObject = JsonUtil::getInstance().getJsonObject();

    if (!JsonUtil::getInstance().isJsonFileExist() || jsonObject.isEmpty()) {
        QMessageBox::warning(this, StringPoolUtil::getInstance().get(WARNING),
                             StringPoolUtil::getInstance().get(NOFACILITY));
        return;
    }
    // 获取装备编号
    QString deviceNumber =
        QInputDialog::getText(this, "解绑装备",
                              StringPoolUtil::getInstance().get(INPUTFACILITY))
            .simplified();
    deviceNumber.remove(' ');
    if (deviceNumber.isEmpty()) {
        return;
    }

    QString employeeName{};
    if (jsonObject.contains(deviceNumber)) {
        // 装备编号已存在，检查员工名字是否为null
        if (jsonObject[deviceNumber].isNull()) {
            QMessageBox::warning(this,
                                 StringPoolUtil::getInstance().get(WARNING),
                                 "装备暂未被绑定");
            return;
        } else {
            employeeName = jsonObject[deviceNumber].toString();
            jsonObject[deviceNumber] = QJsonValue::Null;
        }
    } else {
        // 装备编号不存在，弹出警告对话框
        QMessageBox::warning(this, StringPoolUtil::getInstance().get(WARNING),
                             StringPoolUtil::getInstance().get(NONUMBER));
        return;
    }

    JsonUtil::getInstance().rewriteJsonFile(jsonObject);
    updateTable();
    // 显示解绑成功的信息
    QMessageBox::information(this, StringPoolUtil::getInstance().get(SUCCEED),
                             employeeName + "的装备已成功解绑");
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
    auto jsonObject = JsonUtil::getInstance().getJsonObject();

    if (!JsonUtil::getInstance().isJsonFileExist() || jsonObject.isEmpty()) {
        QMessageBox::warning(this, StringPoolUtil::getInstance().get(WARNING),
                             StringPoolUtil::getInstance().get(NOFACILITY));
        return;
    }

    QString deviceNumber =
        QInputDialog::getText(this, "删除装备",
                              StringPoolUtil::getInstance().get(INPUTFACILITY))
            .simplified();
    deviceNumber.remove(' ');

    if (deviceNumber.isEmpty()) {
        return;
    }

    QString deviceNumber_t{deviceNumber};
    if (jsonObject.contains(deviceNumber)) {
        // 装备编号已存在，检查员工名字是否为null
        if (jsonObject[deviceNumber].isNull()) {
            jsonObject.remove(deviceNumber);
        } else {
            QMessageBox::warning(this,
                                 StringPoolUtil::getInstance().get(WARNING),
                                 "装备处于绑定状态无法删除");
            return;
        }
    } else {
        // 装备编号不存在，弹出警告对话框
        QMessageBox::warning(this, StringPoolUtil::getInstance().get(WARNING),
                             StringPoolUtil::getInstance().get(NONUMBER));
        return;
    }

    // 写入更新后的JSON数据到文件
    JsonUtil::getInstance().rewriteJsonFile(jsonObject);
    updateTable();
    // 显示解绑成功的信息
    QMessageBox::information(this, StringPoolUtil::getInstance().get(SUCCEED),
                             "编号" + deviceNumber_t + "装备已删除");
}

/*
    切换信息按钮
    1. 切换表格行
*/

void Widget::on_switchoverButton_clicked() {
    auto jsonObject = JsonUtil::getInstance().getJsonObject();
    if (!JsonUtil::getInstance().isJsonFileExist() || jsonObject.isEmpty()) {
        QMessageBox::warning(this, StringPoolUtil::getInstance().get(WARNING),
                             StringPoolUtil::getInstance().get(NOFACILITY));
        return;
    }

    // 切换表格
    static int row = -1;
    // 判断是哪个按钮进入槽函数
    auto objButton = qobject_cast<QPushButton *>(sender());
    if (objButton == ui->pushButton_laststaff) {
        if (row <= 0) {
            row = ui->tableWidget->rowCount() - 1;
        } else {
            row--;
        }
    } else {
        if (row == ui->tableWidget->rowCount() - 1) {
            row = 0;
        } else {
            row++;
        }
    }
    ui->tableWidget->selectRow(row);
}



void Widget::on_searchButton_clicked() {
    QString str = ui->lineEdit->text().simplified();
    str.remove(' ');

    if (!str.isEmpty()) {

        // 根据输入框中的数据查找并选择QTableWidget中的行
        // 该循环遍历整个QTableWidget的每一行，检查第0列和第1列的数据是否与输入框中的数据匹配
        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            // 如果找到匹配的行
            if (ui->tableWidget->item(i, 0)->text() == str ||
                ui->tableWidget->item(i, 1)->text() == str) {
                // 选择该行
                ui->tableWidget->selectRow(i);
                // 清空输入框
                ui->lineEdit->setText("");
                // 让输入框失去焦点
                ui->lineEdit->clearFocus();
                return;
            }
        }
        // 如果没有找到匹配的行，弹出提示框
        QMessageBox::warning(this, StringPoolUtil::getInstance().get(WARNING),
                             "查找失败！请检查输入是否正确！");
    }
    ui->lineEdit->setText("");
    ui->lineEdit->clearFocus();
}
