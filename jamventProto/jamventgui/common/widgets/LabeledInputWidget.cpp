#include <QDebug>
#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>

#include "Theme.h"
#include "LabeledInputWidget.h"

namespace
{
    const QString LabelStyle = "QLabel { font-family: 'Roboto'; font-size: 14px; font-weight: 500; color: black; }";
    const QString NoFocusStyle = ".LabeledInputWidget { border: none; padding: 2px;}";
    const QString FocusStyle = ".LabeledInputWidget { border: 2px dashed seagreen; padding: 2px; }";
    const QString FocusAndSelectedStyle = ".LabeledInputWidget { border: 3px solid darkgreen; padding: 2px; }";
}

LabeledInputWidget::LabeledInputWidget(const QString& label)
    : QWidget()
    , m_label{new QLabel{label}}
    , m_spinBox{new SpinBoxWidget}
{
    init();
}

LabeledInputWidget::LabeledInputWidget(const QString &label, SpinBoxWidget *spinBox)
    : QWidget(), m_label{new QLabel{label}}, m_spinBox{spinBox}
{
    init();
}

void LabeledInputWidget::paintEvent(QPaintEvent *p_event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(p_event);
}

#if 0
void LabeledInputWidget::focusInEvent(QFocusEvent *event)
{
    qDebug() << "LabeledInputWidget::focusInEvent(" << event << ") called.";
    setFocusState(FocusState::Focus);
}

void LabeledInputWidget::focusOutEvent(QFocusEvent *event)
{
    qDebug() << "LabeledInputWidget::focusOutEvent(" << event << ") called.";
    setFocusState(FocusState::NoFocus);
}
#endif
SpinBoxWidget* LabeledInputWidget::getSpinBox()
{
    return m_spinBox;
}

void LabeledInputWidget::setFocusState(FocusState state)
{
    qDebug() << "LabeledInputWidget::setFocusState(" << static_cast<int>(state) << ") called.";

    FocusableWidget::setFocusState(state);

    if (FocusState::Focus == state)
    {
        setStyleSheet(LabelStyle + FocusStyle);
    }
    else if (FocusState::FocusAndSelected == state)
    {
        setStyleSheet(LabelStyle + FocusAndSelectedStyle);
    }
    else
    {
        setStyleSheet(LabelStyle + NoFocusStyle);
    }
}

void LabeledInputWidget::init()
{
    // Set size policy
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    setFixedHeight(48);
    setMinimumWidth(60);

    // Add to a horizontal layout.
    auto layout = new QVBoxLayout{};
    const int marginSpacing = 4;
    layout->setContentsMargins(marginSpacing, marginSpacing, marginSpacing, marginSpacing);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->setSpacing(2);
    setLayout(layout);

    layout->addWidget(m_label);

    m_spinBox->setMinimumWidth(80);
    layout->addWidget(m_spinBox);

    m_label->setStyleSheet(LabelStyle);
    m_label->setFont(Theme::MainFont);

    connect(m_spinBox, &SpinBoxWidget::sigFocusChanged, this, &LabeledInputWidget ::onSpinBoxFocusChanged);
}

void LabeledInputWidget::onSpinBoxFocusChanged(bool hasFocus)
{
    qDebug() << "LabeledInputWidget::onSpinBoxFocusChanged(" << hasFocus << ") called.";

    if (hasFocus)
    {
        setFocusState(FocusState::Focus);
    }
    else
    {
        setFocusState(FocusState::NoFocus);
    }
}
