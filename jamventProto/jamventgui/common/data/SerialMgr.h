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

    void setValueAOpen(bool isOpen);
    void setValueBOpen(bool isOpen);
    void setValueCOpen(bool isOpen);
    void setValueDOpen(bool isOpen);

  signals:
    // JPW @todo Update to do polling.
    void sigReadLine(QByteArray data);
    void sigNameValuePair(QString name, QString value);

  public slots:
    void onFio2Changed(NumType value) override;
    void onTidalVolChanged(NumType value) override;
    void onRespRateChanged(NumType value) override;
    void onIeRatioChanged(NumType value) override;
    void onPeepChanged(NumType value) override;

    void onValveAOpenChanged(bool isOpen) override;
    void onValveBOpenChanged(bool isOpen) override;
    void onValveCOpenChanged(bool isOpen) override;
    void onValveDOpenChanged(bool isOpen) override;

  private slots:
    void onReadyRead();
    void onReadLine(QByteArray data);

  private:
    void toggleValve(const char* valveChar);

    bool m_isValueAOpen = false;
    bool m_isValueBOpen = false;
    bool m_isValueCOpen = false;
    bool m_isValueDOpen = false;

    QSerialPort m_serialPort;
    void onTimeout();
    QTimer m_timer;       // Timer for polling communication
};
#endif /* SERIALMGR_H */
