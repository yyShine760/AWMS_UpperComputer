#ifndef LISTEN_H
#define LISTEN_H

#define OBJECT

#include <QObject>
#include <QThread>
#include <QDebug>


// 离线、已佩戴、未佩戴
enum class EquipmentStatus {
    Offline = 0,
    Wearing,
    NotWearing
};



#ifdef THREAD
#include <QThread>
#include <mutex>
#include <condition_variable>
class LowerComputer : public QThread {
public:
    static LowerComputer &getInstance();

    void run() override;

    LowerComputer(const LowerComputer &) = delete;
    LowerComputer &operator=(const LowerComputer &) = delete;

private:
    LowerComputer() = default;
    ~LowerComputer() override = default;

    void listeningLowerComputer();

    QString getLowerComputerMsg() const;

private:
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _isMsgReceived = false;
    QString _MsgLowerCpt;
};
#endif

#ifdef OBJECT

class LowerComputer : public QObject {
    Q_OBJECT
public:
    explicit LowerComputer(QObject *parent = nullptr);
    /**
     * @brief 工作函数
     * @note  监听下位机的消息
     */
    void listeningLowerComputer();
    /**
     * @brief 获取并处理下位机信息
     * 
     * @return EquipmentStatus ：状态枚举类
     */
    EquipmentStatus getLowerComputerMsg();

signals:
    /**
     * @brief 发送状态信息至主线程
     * 
     * @param status ：状态
     */
    void sendMsgToUI(EquipmentStatus status);

private:
    QString _MsgLowerCpt;
};

#endif


#endif



