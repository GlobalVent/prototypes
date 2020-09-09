#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>

#include "Theme.h"
#include "PushButtonWidget.h"
#include "AlertDialogWidget.h"

namespace
{
    const QString DialogStyle =
        "QDialog {"
        "  font-family: 'Roboto';"
        "  font-size: 12px;"
        "  font-weight: 400;"
        "  color: black;"
        "  border: 2px solid black;"
        "  background-color: white;"
        "}";
}

AlertDialogWidget::AlertDialogWidget(const QString &title, int errStatus, const QString &errMessage, QWidget *parent)
    : QDialog{parent}
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //setPaletteBackgroundColor(Theme::BackgroundColor);
    setStyleSheet(DialogStyle);
    setAutoFillBackground(true);
    setModal(true);
    resize(parent->width() / 2, parent->height() / 2);

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QLabel *titleLabel = new QLabel(this);
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    QFont font = titleLabel->font();
    font.setPixelSize(Theme::FontSizeLarger_px);
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->setAlignment(Qt::AlignHCenter);

    QLabel* errStatusLabel = new QLabel(this);
    layout->addWidget(errStatusLabel);
    QLabel* errMessageLabel = new QLabel(this);
    errMessageLabel->setWordWrap(true);
    layout->addWidget(errMessageLabel);

    layout->addStretch(1);
    PushButtonWidget* button = new PushButtonWidget(tr("OK"), this);
    layout->addWidget(button);
    button->setFocusState(PushButtonWidget::FocusState::Focus);  // Set focus to dialog dismiss button.

    titleLabel->setText(title);
    errStatusLabel->setText("Error number: " + QString::number(errStatus));
    errMessageLabel->setText(errMessage);

    connect(button, &QPushButton::clicked, this, &QDialog::accept);
}

void AlertDialogWidget::paintEvent(QPaintEvent *p_event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(p_event);
}
