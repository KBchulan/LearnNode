#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "studentManager.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void searchStudent();
    void showAllStudents();
    void addNewGrade();

private:
    void setupUI();
    void setupTable();
    void updateTable(const std::vector<std::pair<std::string, std::vector<Grade>>> &data);

    QLineEdit *nameInput;
    QLineEdit *scoreInput;
    QLineEdit *subjectInput;

    QPushButton *searchBtn;
    QPushButton *showAllBtn;
    QPushButton *addGradeBtn;

    QTableWidget *gradeTable;

    enum TableColumns
    {
        COL_NAME = 0,
        COL_SUBJECT = 1,
        COL_SCORE = 2,
        COL_AVERAGE = 3
    };
};

#endif // MAINWINDOW_H