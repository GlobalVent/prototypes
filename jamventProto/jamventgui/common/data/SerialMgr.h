#ifndef SERIALMGR_H
#define SERIALMGR_H

#include <QTimer>
#include <QMap>
#include <QSerialPort>

#include "CommMgrInterface.h"

class SerialMgr: public CommMgrInterface
{
    Q_OBJECT
  public:
    using NameValueMap = QMap<QString, QString>;

    explicit SerialMgr();
    virtual ~SerialMgr();

    void start();
    void stop();

  signals:
    void sigNewNameValues(NameValueMap map);

  public slots:
    void onFio2Changed(int value) override;
    void onTidalVolChanged(int value) override;
    void onRespRateChanged(int value) override;
    void onIeRatioChanged(int value) override;
    void onPeepChanged(int value) override;

    void onValveAOpenChanged(bool isOpen) override;
    void onValveBOpenChanged(bool isOpen) override;
    void onValveCOpenChanged(bool isOpen) override;
    void onValveDOpenChanged(bool isOpen) override;

    // Temporary for demo phase
    void onMaxPressChanged(int value);
    void onVaTargetChanged(int value);
    void onVbTargetChanged(int value);
    void onSysTargetChanged(int value);

  private slots:
    void onNewNameValues(NameValueMap map);

  private:
    void onTimeout();
    void writeValveChar(const char* valveChar);

    bool m_isValueAOpen = false;
    bool m_isValueBOpen = false;
    bool m_isValueCOpen = false;
    bool m_isValueDOpen = false;

    QSerialPort m_serialPort;
    QTimer m_timer;       // Timer for polling communication
};

// Declare as meta type so can use to pass data in signals/slots.
Q_DECLARE_METATYPE(SerialMgr::NameValueMap)

#endif /* SERIALMGR_H */
