#ifndef INPUTEVENTMGR_H
#define INPUTEVENTMGR_H

#include <QKeyEvent>

class InputEventMgr: public QObject
{
        Q_OBJECT
  public:
    InputEventMgr();
    ~InputEventMgr() = default;

  signals:

  public slots:
    void onKnobEvent(QKeyEvent* event);

  private:
};
#endif /* INPUTEVENTMGR_H */
