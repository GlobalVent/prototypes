#ifndef SERIALMGR_H
#define SERIALMGR_H

#include <QTimer>
#include <QSerialPort>

#include "CommMgrInterface.h"

class SerialMgr: public CommMgrInterface
{
    Q_OBJECT
  public:
    explicit SerialMgr();
    virtual ~SerialMgr();

    void start();
    void stop();

  signals:
    // JPW @todo Update to do polling.  Remove when switched over to ppolling
    void sigReadLine(QByteArray data);

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
    void onReadyRead();
    void onReadLine(QByteArray data);

  private:
    void onTimeout();
    void toggleValve(const char* valveChar);

    bool m_isValueAOpen = false;
    bool m_isValueBOpen = false;
    bool m_isValueCOpen = false;
    bool m_isValueDOpen = false;

    QSerialPort m_serialPort;
    QTimer m_timer;       // Timer for polling communication
};
#endif /* SERIALMGR_H */
