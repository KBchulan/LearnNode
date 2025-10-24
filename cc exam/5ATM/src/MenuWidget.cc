#include "../include/MenuWidget.h"

MenuWidget::MenuWidget(QWidget *parent) : ViewWidget(parent)
{
    auto main_layout = new QVBoxLayout(this);

    title_label = new QLabel("Main menu", this);
    title_label->setAlignment(Qt::AlignCenter);
    QFont title_font = title_label->font();
    title_font.setPointSize(16);
    title_font.setBold(true);
    title_label->setFont(title_font);
    main_layout->addWidget(title_label);

    second_label = new QLabel("1 - View my balance", this);
    third_label = new QLabel("2 - Withdraw cash", this);
    forth_label = new QLabel("3 - Deposit funds", this);
    fifth_label = new QLabel("4 - Exit", this);

    main_layout->addWidget(second_label);
    main_layout->addWidget(third_label);
    main_layout->addWidget(forth_label);
    main_layout->addWidget(fifth_label);

    auto input_layout = new QHBoxLayout();
    tip_label = new QLabel("Enter a choice:", this);
    input_edit = new QLineEdit(this);
    input_edit->setFixedWidth(50);
    input_edit->setFocus();
    input_layout->addWidget(tip_label);
    input_layout->addWidget(input_edit);
    input_layout->addStretch();
    main_layout->addLayout(input_layout);

    main_layout->addStretch();
}