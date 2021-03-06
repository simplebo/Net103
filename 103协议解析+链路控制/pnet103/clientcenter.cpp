#include "clientcenter.h"
#include <QSettings>

ClientCenter::ClientCenter(QObject *parent) :
    QObject(parent)
{
    QSettings setting("../etc/p103.ini",QSettings::IniFormat);
    setting.beginGroup("p103");
    m_remotePort = setting.value("port",1032).toUInt();
    setting.endGroup();
    server_a=new TcpServer(this);
    server_b=new TcpServer(this);
    startTimer(1000);
}

ushort ClientCenter::GetRemotePort()
{
    return m_remotePort;
}

void ClientCenter::SendAppData(ushort sta,ushort dev, const QByteArray& data)
{
    foreach (Device* d, m_lstDevice) {
        if( (sta==0xff || sta == d->m_stationAddr ) &&
                (dev==0xffff || dev == d->m_devAddr ) ){
            d->SendAppData(data);
        }
    }
}

Device* ClientCenter::GetDevice(ushort sta,ushort dev)
{
    quint32 addr = (((quint32)sta)<<16)+dev;
    Device* d = m_mapDevice.value(addr,0);
    if(d){
        return d;
    }
    d = new Device(this);
    d->m_stationAddr= sta;
    d->m_devAddr=dev;
    m_lstDevice.append(d);
    m_mapDevice[addr]=d;
    return d;
}

QList<QTcpSocket*> ClientCenter::GetSocketList()
{
    return m_socketList;
}

void ClientCenter::Clear()
{
    while(m_lstGateWay.count()>0){
        delete m_lstGateWay.takeFirst();
    }
    while(m_lstDevice.count()>0){
        delete m_lstDevice.takeFirst();
    }
    m_mapDevice.clear();
    m_socketList.clear();
}

void ClientCenter::SetDeviceList(const QVariantList& list)
{
    foreach (QVariant v, list) {
        QVariantMap map = v.toMap();
        QVariantList ips = map.value("ips").toList();
        ushort sta = map.value("staAddr").toUInt();
        ushort dev = map.value("devAddr").toUInt();
        if(ips.isEmpty()){
            continue;
        }
        QStringList sips;
        foreach (QVariant ip, ips) {
            QString sip = ip.toString();
            if(!sip.isEmpty()){
                sips+=sip;
            }
        }
        if(sips.isEmpty()){
            continue;
        }
        GateWay* gw = new GateWay(this);
        gw->Init(sta,dev,sips);
        m_lstGateWay.append(gw);
    }
    StartListen();
}

void ClientCenter::StartListen(){
    server_a->SetGateWay(m_lstGateWay);
    server_b->SetGateWay(m_lstGateWay);
    server_a->listen(QHostAddress::Any, 1048);
    server_b->listen(QHostAddress::Any, 1049);
}

void ClientCenter::timerEvent(QTimerEvent *)
{
    foreach (GateWay* g, m_lstGateWay) {
        g->OnTime();
    }
    foreach (Device* d, m_lstDevice) {
        d->OnTimer();
    }
}
