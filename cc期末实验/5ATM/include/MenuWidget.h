#ifndef ATM_MENU_WIDGET_H
#define ATM_MENU_WIDGET_H

#include "ViewWidget.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MenuWidget final : public ViewWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);
    QLineEdit *getInputEdit() { return input_edit; }

private:
    QLabel *title_label;
    QLabel *second_label;
    QLabel *third_label;
    QLabel *forth_label;
    QLabel *fifth_label;
    QLabel *tip_label;
    QLineEdit *input_edit;
};

#endif // ATM_MENU_WIDGET_H