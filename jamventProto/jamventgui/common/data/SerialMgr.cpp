#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>

#include "SerialMgr.h"

namespace
{
    const qreal TimerInterval_ms = 100;
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
    qDebug() << "Serial port open status =" << status;
    m_serialPort.setBaudRate(QSerialPort::Baud9600);

    // Connect serial port.

    // Disable handling data in signal and poll instead.
#if 0
    connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialMgr::onReadyRead);
#else
    connect(&m_timer, &QTimer::timeout, this, &SerialMgr::onTimeout);
#endif

    connect(this, &SerialMgr::sigReadLine, this, &SerialMgr::onReadLine, Qt::QueuedConnection);  // Queued to queue read requests
}

SerialMgr::~SerialMgr()
{
    m_timer.start();
}

void SerialMgr::start()
{
    m_timer.setInterval(TimerInterval_ms);
    m_timer.start();
}

void SerialMgr::stop()
{
    m_timer.start();
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
    if (m_isValueAOpen != isOpen)
    {
        m_isValueAOpen = isOpen;
        toggleValve("a");
    }
}

void SerialMgr::onValveBOpenChanged(bool isOpen)
{
    if (m_isValueBOpen != isOpen)
    {
        m_isValueBOpen = isOpen;
        toggleValve("b");
    }
}

void SerialMgr::onValveCOpenChanged(bool isOpen)
{
    if (m_isValueCOpen != isOpen)
    {
        m_isValueCOpen = isOpen;
        toggleValve("c");
    }
}

void SerialMgr::onValveDOpenChanged(bool isOpen)
{
    if (m_isValueDOpen != isOpen)
    {
        m_isValueDOpen = isOpen;
        toggleValve("d");
    }
}

void SerialMgr::onMaxPressChanged(NumType value)
{
    qDebug() << "SerialMgr::onMaxPressChanged(" << value << ")";
}

void SerialMgr::onVaTargetChanged(NumType value)
{
    qDebug() << "SerialMgr::onVaTargetChanged(" << value << ")";
}

void SerialMgr::onVbTargetChanged(NumType value)
{
    qDebug() << "SerialMgr::onVbTargetChanged(" << value << ")";
}

void SerialMgr::onSysTargetChanged(NumType value)
{
    qDebug() << "SerialMgr::onSysTargetChanged(" << value << ")";
}

// JPW @todo remove when switiched to polling
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
    //qDebug() << "readLine str =" << str;

    QStringList list = str.split("=");
    //qDebug() << "readLine list =" << list;

    if (list.size() >= 2)
    {
        DataIn newInData;

        QString name = list.at(0);
        QString value = list.at(1);

        if ("H" == name)
        {
            bool isOk;
            int intValue = value.toInt(&isOk, 10);
            if (isOk)
            {
                // Convert to a range of 0.0 to 10.0 to 0.0 to 2.0 times 1000.
                float t = intValue / 10.0;
                t *= 2.0;
                t *= 1000;
                newInData.pressRes = t;
                emit sigNewInData(newInData);
            }
            else
            {
                qDebug() << "Failed to convert value string to int. value =" << value;
            }
        }
    }
}

void SerialMgr::onTimeout()
{
    while(m_serialPort.canReadLine())
    {
        constexpr qint64 maxSize = 80;
        QByteArray data = m_serialPort.readLine(maxSize);
        // qDebug() << "readLine data =" << data;
        emit sigReadLine(data);
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
