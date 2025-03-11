#include "../include/WithDrawWidget.h"

WithdrawWidget::WithdrawWidget(QWidget *parent) : ViewWidget(parent)
{
    auto main_layout = new QVBoxLayout(this);
    
    title_label = new QLabel("Withdrawal Menu", this);
    title_label->setAlignment(Qt::AlignCenter);
    QFont title_font = title_label->font();
    title_font.setPointSize(16);
    title_font.setBold(true);
    title_label->setFont(title_font);
    main_layout->addWidget(title_label);

    auto amount_layout = new QHBoxLayout();
    auto amount_label = new QLabel("Enter amount to withdraw:", this);
    amount_edit = new QLineEdit(this);
    amount_edit->setFixedWidth(200);
    amount_edit->setFocus();
    amount_layout->addWidget(amount_label);
    amount_layout->addWidget(amount_edit);
    amount_layout->addStretch();
    main_layout->addLayout(amount_layout);

    main_layout->addStretch();
}