#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H

#include <QKeyEvent>

class InputEvents
{
  public:
    //InputEvents() = delete; // Never instantiated
    ~InputEvents() = default;

    static QKeyEvent *ClickedEventCreate();
    static QKeyEvent *CwClickEventCreate();
    static QKeyEvent *CcwClickEventCreate();

  private:
};
#endif /* INPUTEVENTS_H */
