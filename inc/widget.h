#ifndef WIDGET_H
#define WIDGET_H

#include "../util/StringPoolUtil.hpp"
#include "../util/jsonUtil.hpp"
#include "listen.h"
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
    void updateTable();

    void on_add_facility_clicked();

    void on_bind_facility_clicked();

    void on_rebind_facility_clicked();

    void on_del_facility_clicked();

    void on_switchoverButton_clicked();

    void on_searchButton_clicked();

private:
    Ui::Widget *ui;
    QThread *thread;
    LowerComputer *lowerComputer;
};
#endif // WIDGET_H
