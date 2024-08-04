#ifndef WIDGET_H
#define WIDGET_H

#include "../util/StringPoolUtil.hpp"
#include "../util/jsonUtil.hpp"
#include "ui_widget.h"

#include <QInputDialog>
#include <QRegularExpression>
#include <QWidget>
#include <QmessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    inline void updateTable() {
        // 从json文件读出key值数据写入表格的装备编号
        if (JsonUtil::getInstance().isJsonFileExist()) {
            auto keys = JsonUtil::getInstance().getJsonKeys();
            auto values = JsonUtil::getInstance().getJsonValues();

            ui->tableWidget->setRowCount(keys.size()); // 动态设置行数

            for (int row = 0; row < keys.size(); ++row) {
                // 打印编号列
                QTableWidgetItem *tableWidgetItem =
                    new QTableWidgetItem(keys[row]);
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

    void on_add_facility_clicked();

    void on_bind_facility_clicked();

    void on_rebind_facility_clicked();

    void on_del_facility_clicked();

    void on_switchoverButton_clicked();

    void on_searchButton_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
