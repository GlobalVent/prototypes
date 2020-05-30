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

void SerialMgr::toggleValve(const char* valveChar)
{
    const qint64 bytesWritten = m_serialPort.write(valveChar);
    if (1 != bytesWritten)
    {
        qDebug() << "m_serialPort.write() failed. Bytes written:" << bytesWritten;
    }
}
