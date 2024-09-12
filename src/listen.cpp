#include "../inc/listen.h"

LowerComputer::LowerComputer(QObject *parent) : QObject(parent) {}


EquipmentStatus LowerComputer::getLowerComputerMsg() {
    // ...
    // 获取到下位机信息

    return EquipmentStatus::Offline;
}


void LowerComputer::listeningLowerComputer() {
    // 等待下位机发送数据
    // 获取信息
    while (true) {
        // ...

        emit sendMsgToUI(getLowerComputerMsg());

        QThread::msleep(100);
    }
}
