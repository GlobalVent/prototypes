#ifndef ALERTDIALOGWIDGET_H
#define ALERTDIALOGWIDGET_H

#include <QDialog>
#include <QLabel>

class AlertDialogWidget : public QDialog
{
  Q_OBJECT

  public:
    AlertDialogWidget(QWidget* parent = nullptr) = delete;
    AlertDialogWidget( const QString& title, int errStatus, const QString& errMessage, QWidget* parent);

    ~AlertDialogWidget() = default;

  protected:
    void paintEvent(QPaintEvent *p_event) override;

  private:

};
#endif /* ALERTDIALOGWIDGET_H */
