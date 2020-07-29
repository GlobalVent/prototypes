#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H

#include <QKeyEvent>

class InputEvents
{
  public:
    enum class Event_E
    {
        ButtonClicked = Qt::Key_C,
        ButtonPressed = Qt::Key_D,
        ButtonReleased = Qt::Key_U,
        ButtonLongPress = Qt::Key_P,
        CwClick = Qt::Key_R,
        CcwClick = Qt::Key_L
    };

    InputEvents() = delete; // Never instantiated
    ~InputEvents() = default;

    static QKeyEvent *ButtonClickedEventCreate();
    static QKeyEvent *ButtonPressedEventCreate();
    static QKeyEvent *ButtonReleasedEventCreate();
    static QKeyEvent *ButtonLongPressEventCreate();
    static QKeyEvent *CwClickEventCreate();
    static QKeyEvent *CcwClickEventCreate();

  private:
};
#endif /* INPUTEVENTS_H */
