#include <QCloseEvent>
#include "../include/MainWidget.h"
#include "../include/LogManager.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    QFile styleFile("resources/style.qss");
    if (styleFile.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet);
        styleFile.close();
    }

    setFixedSize(1024, 768);
    setObjectName("mainWidget");

    main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(30);
    main_layout->setContentsMargins(40, 30, 40, 30);

    current_view = new EnterWidget(this);
    main_layout->addWidget(current_view);

    choose_widget = new QWidget(this);
    auto choose_layout = new QHBoxLayout(choose_widget);
    choose_layout->setSpacing(50);

    left_widget = new QWidget(choose_widget);
    auto left_layout = new QGridLayout(left_widget);
    left_layout->setSpacing(10);

    for (int i = 0; i < 9; i++)
    {
        auto btn = new QPushButton(QString::number(i + 1), left_widget);
        btn->setFixedSize(80, 80);
        btn->setProperty("class", "numpad");
        btn->setFocusPolicy(Qt::NoFocus);
        number_buttons.push_back(btn);
        left_layout->addWidget(btn, i / 3, i % 3);
        connect(btn, &QPushButton::clicked, this, &MainWidget::handleNumberClick);
    }

    auto btn0 = new QPushButton("0", left_widget);
    btn0->setFixedSize(80, 80);
    btn0->setProperty("class", "numpad");
    btn0->setFocusPolicy(Qt::NoFocus);
    number_buttons.push_back(btn0);
    left_layout->addWidget(btn0, 3, 1);
    connect(btn0, &QPushButton::clicked, this, &MainWidget::handleNumberClick);

    enter_button = new QPushButton("Enter", left_widget);
    enter_button->setFixedSize(80, 80);
    enter_button->setProperty("class", "enter");
    left_layout->addWidget(enter_button, 3, 2);
    connect(enter_button, &QPushButton::clicked, this, &MainWidget::handleEnterClick);

    right_widget = new QWidget(choose_widget);
    auto right_layout = new QVBoxLayout(right_widget);
    right_layout->setSpacing(40);
    right_layout->setContentsMargins(20, 20, 20, 20);

    take_cash_label = new QLabel("Take cash here", right_widget);
    insert_cash_label = new QLabel("Insert cash here", right_widget);

    take_cash_label->setProperty("class", "atm-hint");
    take_cash_label->setProperty("type", "take");
    take_cash_label->setAlignment(Qt::AlignCenter);

    insert_cash_label->setProperty("class", "atm-hint");
    insert_cash_label->setProperty("type", "insert");
    insert_cash_label->setAlignment(Qt::AlignCenter);

    right_layout->addWidget(take_cash_label);
    right_layout->addWidget(insert_cash_label);
    right_layout->addSpacing(20);
    right_layout->addStretch();

    choose_layout->addWidget(left_widget);
    choose_layout->addWidget(right_widget);

    main_layout->addWidget(choose_widget);
}

void MainWidget::handleNumberClick()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;

    QString number = button->text();

    if (auto enter_widget = dynamic_cast<EnterWidget *>(current_view))
    {
        if (enter_widget->getAccountEdit()->hasFocus())
        {
            enter_widget->getAccountEdit()->insert(number);
        }
        else if (enter_widget->getPassEdit()->hasFocus())
        {
            enter_widget->getPassEdit()->insert(number);
        }
    }
    else if (auto menu_widget = dynamic_cast<MenuWidget *>(current_view))
    {
        menu_widget->getInputEdit()->insert(number);
    }
    else if (auto withdraw_widget = dynamic_cast<WithdrawWidget *>(current_view))
    {
        withdraw_widget->getAmountEdit()->insert(number);
    }
}

std::function<QString(QString)> xorString = [](QString input)
{
    QString result = input;
    int length = input.length();
    
    const QString key = "WhxChat";
    int keyLength = key.length();
    
    for(int i = 0; i < length; i++) 
        result[i] = QChar(input[i].unicode() ^ key[i % keyLength].unicode());
    
    return result;
};

void MainWidget::handleEnterClick()
{
    if (auto enter_widget = dynamic_cast<EnterWidget*>(current_view))
    {
        QString account = enter_widget->getAccountEdit()->text();
        QString password = enter_widget->getPassEdit()->text();
        password = xorString(password);
        
        if (account.isEmpty() || password.isEmpty())
        {
            QMessageBox::warning(this, "Warning", "Account and password cannot be empty!");
            return;
        }

        UserInfo userInfo;
        bool success = MysqlManager::GetInstance()->CheckPwd(
            account.toStdString(), 
            password.toStdString(), 
            userInfo
        );

        if (success)
        {
            _current_user = userInfo;
            LOG_SERVER->info("User {} logged in successfully", userInfo._email);
            switchToView(new MenuWidget(this));
        }
        else
        {
            QMessageBox::critical(this, "Error", "Invalid account or password!");
            enter_widget->getAccountEdit()->clear();
            enter_widget->getPassEdit()->clear();
            enter_widget->getAccountEdit()->setFocus();
            LOG_SERVER->warn("Login failed for account: {}", account.toStdString());
        }
    }
    else if (auto menu_widget = dynamic_cast<MenuWidget*>(current_view))
    {
        QString choice = menu_widget->getInputEdit()->text();
        if (choice == "2")
        {
            switchToView(new WithdrawWidget(this));
        }
    }
}

void MainWidget::switchToView(ViewWidget *new_view)
{
    if (current_view)
    {
        main_layout->removeWidget(current_view);
        current_view->deleteLater();
        current_view = nullptr;
    }
    current_view = new_view;
    main_layout->insertWidget(0, current_view);

    if (auto enter_widget = dynamic_cast<EnterWidget *>(current_view))
    {
        enter_widget->getAccountEdit()->setFocus();
    }
    else if (auto menu_widget = dynamic_cast<MenuWidget *>(current_view))
    {
        menu_widget->getInputEdit()->setFocus();
    }
    else if (auto withdraw_widget = dynamic_cast<WithdrawWidget *>(current_view))
    {
        withdraw_widget->getAmountEdit()->setFocus();
    }
}

void MainWidget::simulateKeyPress(const QString &key)
{
    if (auto enter_widget = dynamic_cast<EnterWidget *>(current_view))
    {
        if (enter_widget->getAccountEdit()->hasFocus())
        {
            enter_widget->getAccountEdit()->insert(key);
        }
        else if (enter_widget->getPassEdit()->hasFocus())
        {
            enter_widget->getPassEdit()->insert(key);
        }
    }
    else if (auto menu_widget = dynamic_cast<MenuWidget *>(current_view))
    {
        menu_widget->getInputEdit()->insert(key);
    }
    else if (auto withdraw_widget = dynamic_cast<WithdrawWidget *>(current_view))
    {
        withdraw_widget->getAmountEdit()->insert(key);
    }
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    event->accept();

    if (!_current_user._email.empty()) {
        LOG_SERVER->info("User {} logged out", _current_user._email);
    }

    LOG_SERVER->flush();
    LOG_SQL->flush();

    LogManager::GetInstance()->Shutdown();
    MysqlManager::GetInstance().reset();

    QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
}

MainWidget::~MainWidget()
{
    if (current_view)
    {
        delete current_view;
        current_view = nullptr;
    }

    number_buttons.clear();
}