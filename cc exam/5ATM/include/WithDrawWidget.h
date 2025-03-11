#ifndef ATM_WITHDRAW_WIDGET_H
#define ATM_WITHDRAW_WIDGET_H

#include "ViewWidget.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class WithdrawWidget : public ViewWidget
{
    Q_OBJECT
public:
    explicit WithdrawWidget(QWidget *parent = nullptr);
    QLineEdit* getAmountEdit() { return amount_edit; }

private:
    QLabel *title_label;
    QLineEdit *amount_edit;
};

#endif // ATM_WITHDRAW_WIDGET_H