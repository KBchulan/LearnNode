#ifndef ATM_MAIN_WIDGET_H
#define ATM_MAIN_WIDGET_H

#include "ViewWidget.h"
#include "EnterWidget.h"
#include "MenuWidget.h"
#include "MysqlManager.h"
#include "WithDrawWidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QApplication>
#include <vector>
#include <QMessageBox>

class MainWidget final : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QVBoxLayout *main_layout;
    ViewWidget *current_view;
    QWidget *choose_widget;

    QWidget *left_widget;
    QWidget *right_widget;
    std::vector<QPushButton *> number_buttons;
    QPushButton *enter_button;
    QLabel *take_cash_label;
    QLabel *insert_cash_label;

    UserInfo _current_user;

private slots:
    void handleNumberClick();
    void handleEnterClick();
    void switchToView(ViewWidget *new_view);
    void simulateKeyPress(const QString &key);
};

#endif // ATM_MAIN_WIDGET_H