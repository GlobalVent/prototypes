#ifndef SERIALMGR_H
#define SERIALMGR_H

#include <QSerialPort>

class SerialMgr: public QObject
{
    Q_OBJECT
  public:
    explicit SerialMgr();
    ~SerialMgr() = default;

    void setValueAOpen(bool isOpen);
    void setValueBOpen(bool isOpen);
    void setValueCOpen(bool isOpen);
    void setValueDOpen(bool isOpen);

  signals:
    void sigReadLine(QByteArray data);
    void sigNameValuePair(QString name, QString value);

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
};
#endif /* SERIALMGR_H */
