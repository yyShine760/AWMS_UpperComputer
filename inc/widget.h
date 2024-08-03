#ifndef WIDGET_H
#define WIDGET_H

#include "../util/jsonUtil.hpp"
#include "ui_widget.h"
#include <QInputDialog>
#include <QRegularExpression>
#include <QWidget>
#include <QmessageBox>

typedef enum {
    INPUT_W = 300,
    INPUT_H = 50,
} WinSize;

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
    inline void updateTable();

    void on_add_facility_clicked();

    void on_bind_facility_clicked();

    void on_rebind_facility_clicked();

    void on_del_facility_clicked();

    void on_pushButton_laststaff_clicked();

    void on_pushButton_nextstaff_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
