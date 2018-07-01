#ifndef NETPACKET_H
#define NETPACKET_H
#include <QByteArray>

class NetPacket
{
public:
    NetPacket();
    NetPacket(const QByteArray& ba);
    ushort GetLength()const;

    void SetDestAddr(ushort sta,ushort dev);
    void GetDestAddr(ushort& sta,ushort& dev)const;

    void AddAppData(const QByteArray& data);
    QByteArray GetAppData()const;
public:
    QString m_remoteIP;
    ushort dev_addr;
    QByteArray m_data;
};

#endif // NETPACKET_H
