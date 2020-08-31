#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <cmath>

#include "ConfigJson.h"
#include "SerialMgr.h"
#include "JamCtrlMgrInterface.h"

namespace
{
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

    connect(this, &SerialMgr::sigNewNameValues, this, &SerialMgr::onNewNameValues, Qt::QueuedConnection);  // Queued to queue read requests
}

SerialMgr::~SerialMgr()
{
    m_timer.start();
}

void SerialMgr::start()
{
    const ConfigJson::ConfigData &configData = ConfigJson::Instance().getConfigData();
    m_timer.setInterval(configData.pollingTimeout_ms);
    m_timer.start();
}

void SerialMgr::stop()
{
    m_timer.stop();
}

void SerialMgr::onFio2Changed(int value)
{
    qDebug() << "SerialMgr::onFio2Changed(" << value << ") called.";
}

void SerialMgr::onTidalVolChanged(int value)
{
    qDebug() << "SerialMgr::onTidalVolChanged(" << value << ") called.";
}

void SerialMgr::onRespRateChanged(int value)
{
    qDebug() << "SerialMgr::onRespRateChanged(" << value << ") called.";
}

void SerialMgr::onIeRatioChanged(int value)
{
    qDebug() << "SerialMgr::onIeRatioChanged(" << value << ") called.";
}

void SerialMgr::onPeepChanged(int value)
{
    qDebug() << "SerialMgr::onPeepChanged(" << value << ") called.";
}

void SerialMgr::onValveAOpenChanged(bool isOpen)
{
    if (m_isValueAOpen != isOpen)
    {
        m_isValueAOpen = isOpen;
        const char *s = isOpen ? "A" : "a";
        writeValveChar(s);
    }
}

void SerialMgr::onValveBOpenChanged(bool isOpen)
{
    if (m_isValueBOpen != isOpen)
    {
        m_isValueBOpen = isOpen;
        const char *s = isOpen ? "B" : "b";
        writeValveChar(s);
    }
}

void SerialMgr::onValveCOpenChanged(bool isOpen)
{
    if (m_isValueCOpen != isOpen)
    {
        m_isValueCOpen = isOpen;
        const char *s = isOpen ? "C" : "c";
        writeValveChar(s);
    }
}

void SerialMgr::onValveDOpenChanged(bool isOpen)
{
    if (m_isValueDOpen != isOpen)
    {
        m_isValueDOpen = isOpen;
        const char *s = isOpen ? "D" : "d";
        writeValveChar(s);
    }
}

void SerialMgr::onMaxPressChanged(int value)
{
    qDebug() << "SerialMgr::onMaxPressChanged(" << value << ")";
}

void SerialMgr::onVaTargetChanged(int value)
{
    qDebug() << "SerialMgr::onVaTargetChanged(" << value << ")";
}

void SerialMgr::onVbTargetChanged(int value)
{
    qDebug() << "SerialMgr::onVbTargetChanged(" << value << ")";
}

void SerialMgr::onSysTargetChanged(int value)
{
    qDebug() << "SerialMgr::onSysTargetChanged(" << value << ")";

    QString qStr = QString::number(value);

    QByteArray ba("STarg=");
    ba.append(qStr.toUtf8());
    char *str = ba.data();

    qDebug() << "..write(" << str << ")";

    const qint64 bytesWritten = m_serialPort.write(str);
    if (bytesWritten < 1)
    {
        qDebug() << "m_serialPort.write() failed. Bytes written:" << bytesWritten;
    }
}

void SerialMgr::onNewNameValues(NameValueMap map)
{
    JamCtrlMgrInterface::DataIn newInData;

    bool isDataAdded = false;  // Used to see if have data to emit.

    QMapIterator<QString, QString> i(map);
    while (i.hasNext())
    {
        i.next();
        const QString& name = i.key();
        const QString& value = i.value();

        if ("L" == name)
        {
            bool isOk;
            double doubleValue = value.toDouble(&isOk);
            if (isOk)
            {
                // Convert from a range of 0.0 to 100.0 to a range of 0.0 to 10.0.
                double tmp = doubleValue / 100.0;
                tmp *= 10.0;  // 0.0 to 10.0
                newInData.pSys_bar = tmp;  // JPW @todo This may not be right,.but not used currently
                isDataAdded = true;
            }
            else
            {
                qDebug() << "Failed to convert value string to double. value =" << value;
            }
        }
        else if ("H" == name)
        {
            bool isOk;
            double doubleValue = value.toDouble(&isOk);
            if (isOk)
            {
                // Convert from a range of 0.0 to 1000.0 to a range of -1.0 to 1.0
                double tmp = doubleValue / 1000.0;
                tmp *= 2.0;  // 0.0 to 2.0
                tmp -= 1.0;  // -1.0 to 1.0
                newInData.pRes_bar = tmp; // JPW @todo This may not be right,.but not used currently
                isDataAdded = true;
            }
            else
            {
                qDebug() << "Failed to convert value string to double. value =" << value;
            }
        }
    }

    if (isDataAdded)
    {
        emit sigNewInData(newInData);
    }

}

void SerialMgr::onTimeout()
{
    qDebug() << "SerialMgr::onTimeout() called.";
    NameValueMap map;

    // Read each line and add to map.  Retaining the last value for each name.
    while(m_serialPort.canReadLine())
    {
        constexpr qint64 maxSize = 80;
        QByteArray data = m_serialPort.readLine(maxSize);

        QString str(data);
        //qDebug() << "..str =" << str;
        str = str.trimmed(); // Trim whitespace ("\r\n")

        QStringList list = str.split("=");

        if (list.size() >= 2)
        {
            QString name = list.at(0);
            QString value = list.at(1);
            map.insert(name, value);
            //qDebug() << "Adding name =" << name << ", value =" << value;
        }
    }

    qDebug() << "..map = " << map;
    emit sigNewNameValues(map);
}

void SerialMgr::writeValveChar(const char* valveChar)
{
    const qint64 bytesWritten = m_serialPort.write(valveChar);
    if (1 != bytesWritten)
    {
        qDebug() << "m_serialPort.write() failed. Bytes written:" << bytesWritten;
    }
}
