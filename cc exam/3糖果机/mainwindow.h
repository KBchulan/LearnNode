#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "candyitem.h"

#include <QVector>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class QListWidget;
class QLabel;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addToCart(const CandyItem &candy);
    void removeFromCart(int index);
    void updateTotal();

private:
    void setupUI();
    void createCandyList();

    QLabel *m_totalLabel;
    QListWidget *m_candyList;
    QListWidget *m_cartList;
    QVector<CandyItem> m_candies;
    QVector<CandyItem> m_cartItems;
};

#endif // MAINWINDOW_H