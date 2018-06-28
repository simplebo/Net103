#ifndef CLIENTCENTER_H
#define CLIENTCENTER_H

#include <QObject>
#include "gateway.h"
#include "device.h"

const int DEVICE_TEST_TIME_T2 = 10;
const int DEVICE_TEST_TIMES = 4;

const int IPACKET_ACK_T2=10;
const int IPACKET_WAIT_ACK_T1=15;

const int IDEL_WAIT_T3=20;
const int START_WAIT_T1=15;

class ClientCenter : public QObject
{
    Q_OBJECT
public:
    explicit ClientCenter(QObject *parent = 0);
    ushort GetRemotePort();
    void SetDeviceList(const QVariantList& list);
    void SetLocalAddr(ushort addr);
    ushort GetLocalAddr();

    Device* GetDevice(ushort sta,ushort dev);

    void SendAppData(ushort sta,ushort dev, const QByteArray& data);

    bool IsLongNumber();
protected:
    void timerEvent(QTimerEvent *);
    void Clear();
private:
    ushort m_remotePort;
    bool m_longNumber;
    QList<GateWay*> m_lstGateWay;
    QList<Device*> m_lstDevice;
    ushort m_localAddr;
    QMap<quint32,Device*> m_mapDevice;
};

#endif // CLIENTCENTER_H