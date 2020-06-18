#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>

#include "SerialMgr.h"

namespace {
}

SerialMgr::SerialMgr()
{
#if 0
  const auto infos = QSerialPortInfo::availablePorts();
  for (const QSerialPortInfo &info : infos)
  {
    QString s = "Port: " + info.portName() + "\n"
              + "Location: " + info.systemLocation() + "\n"
              + "Description: " + info.description() + "\n"
              + "Manufacturer: " + info.manufacturer() + "\n"
              + "Serial number: " + info.serialNumber() + "\n"
              + "Vendor Identifier: " + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
              + "Product Identifier: " + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n\n";
    qDebug() << s;
  }
#endif

    m_serialPort.setPortName("/dev/ttyACM0");
    bool status = m_serialPort.open(QIODevice::ReadWrite);
    qDebug() << "open status =" << status;
    m_serialPort.setBaudRate(QSerialPort::Baud9600);

    // Connect serial port.
    connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialMgr::onReadyRead);
    connect(this, &SerialMgr::sigReadLine, this, &SerialMgr::onReadLine, Qt::QueuedConnection);  // Queued to queue read requests
}

SerialMgr::~SerialMgr()
{
}

void SerialMgr::setValueAOpen(bool isOpen)
{
    if (m_isValueAOpen != isOpen)
    {
        m_isValueAOpen = isOpen;
        toggleValve("a");
    }
}

void SerialMgr::setValueBOpen(bool isOpen)
{
    if (m_isValueBOpen != isOpen)
    {
        m_isValueBOpen = isOpen;
        toggleValve("b");
    }
}

void SerialMgr::setValueCOpen(bool isOpen)
{
    if (m_isValueCOpen != isOpen)
    {
        m_isValueCOpen = isOpen;
        toggleValve("c");
    }
}

void SerialMgr::setValueDOpen(bool isOpen)
{
    if (m_isValueDOpen != isOpen)
    {
        m_isValueDOpen = isOpen;
        toggleValve("d");
     }
}

void SerialMgr::onFio2Changed(NumType value)
{
}

void SerialMgr::onTidalVolChanged(NumType value)
{
}

void SerialMgr::onRespRateChanged(NumType value)
{
}

void SerialMgr::onIeRatioChanged(NumType value)
{
}

void SerialMgr::onPeepChanged(NumType value)
{
}

void SerialMgr::onValveAOpenChanged(bool isOpen)
{
}

void SerialMgr::onValveBOpenChanged(bool isOpen)
{
}

void SerialMgr::onValveCOpenChanged(bool isOpen)
{
}

void SerialMgr::onValveDOpenChanged(bool isOpen)
{
}

void SerialMgr::onReadyRead()
{
    while(m_serialPort.canReadLine())
    {
        constexpr qint64 maxSize = 80;
        QByteArray data = m_serialPort.readLine(maxSize);
        // qDebug() << "readLine data =" << data;
        emit sigReadLine(data);
    }
}

void SerialMgr::onReadLine(QByteArray data)
{
    QString str(data);
    str = str.trimmed();   // Trim whitespace ("\r\n")
    // qDebug() << "readLine str =" << str;

    QStringList list = str.split("=");
    //qDebug() << "readLine list =" << list;

    if (list.size() >= 2)
    {
        emit sigNameValuePair(list.at(0), list.at(1));
    }
}

void SerialMgr::toggleValve(const char* valveChar)
{
    const qint64 bytesWritten = m_serialPort.write(valveChar);
    if (1 != bytesWritten)
    {
        qDebug() << "m_serialPort.write() failed. Bytes written:" << bytesWritten;
    }
}
