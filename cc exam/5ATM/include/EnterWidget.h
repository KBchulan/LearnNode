#ifndef ATM_ENTER_WIDGET_H
#define ATM_ENTER_WIDGET_H

#include "ViewWidget.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

class EnterWidget final : public ViewWidget
{
    Q_OBJECT
    
public:
    explicit EnterWidget(QWidget *parent = nullptr);
    QLineEdit *getAccountEdit() { return account_edit; }
    QLineEdit *getPassEdit() { return pass_edit; }

private:
    QLabel *title_label;
    QLabel *account_label;
    QLineEdit *account_edit;
    QLabel *pass_label;
    QLineEdit *pass_edit;
};

#endif // ATM_ENTER_WIDGET_H