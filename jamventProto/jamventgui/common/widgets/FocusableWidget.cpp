#include <QDebug>

#include "FocusableWidget.h"

namespace
{
}

FocusableWidget::FocusableWidget()
    : m_focusState(FocusState::None)
{
}

void FocusableWidget::setFocusState(FocusState state)
{
    m_focusState = state;
}

FocusableWidget::FocusState FocusableWidget::getFocusState()
{
    return m_focusState;
}
