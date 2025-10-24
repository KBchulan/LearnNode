#include "../include/EnterWidget.h"

EnterWidget::EnterWidget(QWidget *parent) : ViewWidget(parent)
{
    auto main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(20);
    main_layout->setContentsMargins(40, 40, 40, 40);

    auto center_widget = new QWidget(this);
    auto center_layout = new QVBoxLayout(center_widget);
    center_layout->setSpacing(30);
    center_layout->setContentsMargins(0, 0, 0, 0);

    title_label = new QLabel("Welcome to ATM", this);
    title_label->setProperty("class", "title");
    title_label->setAlignment(Qt::AlignCenter);
    center_layout->addWidget(title_label);

    center_layout->addSpacing(40);

    auto account_layout = new QHBoxLayout();
    account_label = new QLabel("Account:", this);
    account_edit = new QLineEdit(this);
    account_edit->setPlaceholderText("Enter your account number");
    account_edit->setFixedWidth(400);
    account_edit->setFocus();
    account_layout->addWidget(account_label);
    account_layout->addWidget(account_edit);
    account_layout->addStretch();
    center_layout->addLayout(account_layout);

    auto pass_layout = new QHBoxLayout();
    pass_label = new QLabel("Your PIN:", this);
    pass_edit = new QLineEdit(this);
    pass_edit->setPlaceholderText("Enter your PIN");
    pass_edit->setEchoMode(QLineEdit::Password);
    pass_edit->setFixedWidth(400);
    pass_layout->addWidget(pass_label);
    pass_layout->addWidget(pass_edit);
    pass_layout->addStretch();
    center_layout->addLayout(pass_layout);

    center_layout->addStretch();

    main_layout->addWidget(center_widget, 0, Qt::AlignCenter);
}