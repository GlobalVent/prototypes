#ifndef FOCUSABLEWIDGET_H
#define FOCUSABLEWIDGET_H

class FocusableWidget
{
  public:
    enum class FocusState
    {
        None,
        Focus,
        FocusAndSelected
    };

    FocusableWidget();
    virtual ~FocusableWidget() = default;

    // Virtual as focus state will affect display visual for each widget type.
    virtual void setFocusState(FocusState state);

    FocusState getFocusState();

  protected:

  private:
    FocusState m_focusState = FocusState::None;

};
#endif /* FOCUSABLEWIDGET_H */
