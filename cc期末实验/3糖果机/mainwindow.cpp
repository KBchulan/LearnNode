#include "mainwindow.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createCandyList();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto mainLayout = new QHBoxLayout(centralWidget);

    m_candyList = new QListWidget(this);
    m_candyList->setMinimumWidth(300);

    auto cartWidget = new QWidget(this);
    auto cartLayout = new QVBoxLayout(cartWidget);

    m_cartList = new QListWidget(this);
    m_totalLabel = new QLabel("总价: ¥0.00", this);

    cartLayout->addWidget(new QLabel("购物车"));
    cartLayout->addWidget(m_cartList);
    cartLayout->addWidget(m_totalLabel);

    mainLayout->addWidget(m_candyList);
    mainLayout->addWidget(cartWidget);

    connect(m_candyList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item)
    {
        int index = m_candyList->row(item);
        addToCart(m_candies[index]); 
    });

    connect(m_cartList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item)
    {
        int index = m_cartList->row(item);
        removeFromCart(index); 
    });
}

void MainWindow::createCandyList()
{
    m_candies = 
    {
        CandyItem("巧克力", 5.50),
        CandyItem("棒棒糖", 2.00),
        CandyItem("果冻", 3.50),
        CandyItem("软糖", 4.00),
        CandyItem("薄荷糖", 1.50),
        CandyItem("口香糖", 2.50),
        CandyItem("泡泡糖", 1.00)
    };

    for (const auto &candy : m_candies)
    {
        auto item = new QListWidgetItem(
            QString("%1 - ¥%2").arg(candy.getName()).arg(candy.getPrice<double>(), 0, 'f', 2));
        m_candyList->addItem(item);
    }
}

void MainWindow::addToCart(const CandyItem &candy)
{
    for (int i = 0; i < m_cartItems.size(); ++i)
    {
        if (m_cartItems[i].getName() == candy.getName())
        {
            m_cartItems[i].incrementQuantity();

            m_cartList->item(i)->setText(QString("%1 x%2 - ¥%3").arg(m_cartItems[i].getName()).arg(m_cartItems[i].getQuantity()).arg(m_cartItems[i].getPrice<double>(), 0, 'f', 2));
            updateTotal();
            return;
        }
    }

    auto newCandy = candy;
    newCandy.setQuantity(1);
    auto item = new QListWidgetItem(QString("%1 x1 - ¥%2").arg(newCandy.getName()).arg(newCandy.getPrice<double>(), 0, 'f', 2));
    m_cartList->addItem(item);
    m_cartItems.push_back(newCandy);
    updateTotal();
}

void MainWindow::removeFromCart(int index)
{
    if (index < 0 || index >= m_cartItems.size())
        return;

    m_cartItems[index].decrementQuantity();

    if (m_cartItems[index].getQuantity() <= 0)
    {
        delete m_cartList->takeItem(index);
        m_cartItems.remove(index);
    }
    else
    {
        m_cartList->item(index)->setText(QString("%1 x%2 - ¥%3").arg(m_cartItems[index].getName()).arg(m_cartItems[index].getQuantity()).arg(m_cartItems[index].getPrice<double>(), 0, 'f', 2));
    }
    updateTotal();
}

void MainWindow::updateTotal()
{
    double total = 0.0;
    for (const auto &item : m_cartItems)
    {
        total += item.getPrice<double>();
    }
    m_totalLabel->setText(QString("总价: ¥%1").arg(total, 0, 'f', 2));
}