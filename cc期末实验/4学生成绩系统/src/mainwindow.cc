#include "../include/mainwindow.h"

#include <QLabel>
#include <sstream>
#include <iomanip>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      nameInput(nullptr),
      subjectInput(nullptr),
      scoreInput(nullptr),
      gradeTable(nullptr),
      searchBtn(nullptr),
      showAllBtn(nullptr),
      addGradeBtn(nullptr)
{
    setupUI();
}

void MainWindow::setupUI()
{
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout(centralWidget);
    auto *inputLayout = new QGridLayout();

    nameInput = new QLineEdit(this);
    subjectInput = new QLineEdit(this);
    scoreInput = new QLineEdit(this);

    gradeTable = new QTableWidget(this);
    setupTable();

    searchBtn = new QPushButton(tr("查询学生"), this);
    showAllBtn = new QPushButton(tr("显示所有"), this);
    addGradeBtn = new QPushButton(tr("添加成绩"), this);

    inputLayout->addWidget(new QLabel(tr("姓名:"), this), 0, 0);
    inputLayout->addWidget(nameInput, 0, 1);
    inputLayout->addWidget(searchBtn, 0, 2);

    inputLayout->addWidget(new QLabel(tr("科目:"), this), 1, 0);
    inputLayout->addWidget(subjectInput, 1, 1);
    inputLayout->addWidget(showAllBtn, 1, 2);

    inputLayout->addWidget(new QLabel(tr("分数:"), this), 2, 0);
    inputLayout->addWidget(scoreInput, 2, 1);
    inputLayout->addWidget(addGradeBtn, 2, 2);

    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(gradeTable);

    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::searchStudent);
    connect(showAllBtn, &QPushButton::clicked, this, &MainWindow::showAllStudents);
    connect(addGradeBtn, &QPushButton::clicked, this, &MainWindow::addNewGrade);

    setMinimumSize(600, 400);
}

void MainWindow::setupTable()
{
    QStringList headers;
    headers << tr("学生姓名") << tr("科目") << tr("分数") << tr("平均分");

    gradeTable->setColumnCount(headers.size());
    gradeTable->setHorizontalHeaderLabels(headers);
    gradeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    gradeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    gradeTable->setAlternatingRowColors(true);
    gradeTable->setSortingEnabled(true);
}

void MainWindow::updateTable(const std::vector<std::pair<std::string, std::vector<Grade>>> &data)
{
    gradeTable->clearContents();
    gradeTable->setRowCount(0);

    for (const auto &[name, grades] : data)
    {
        float avg = StudentManager::GetInstance()->calcuteAverage(name);

        for (const auto &grade : grades)
        {
            int row = gradeTable->rowCount();
            gradeTable->insertRow(row);

            gradeTable->setItem(row, COL_NAME, new QTableWidgetItem(QString::fromStdString(name)));
            gradeTable->setItem(row, COL_SUBJECT, new QTableWidgetItem(QString::fromStdString(grade._subject)));
            gradeTable->setItem(row, COL_SCORE,
                                new QTableWidgetItem(QString::number(grade._score, 'f', 1)));
            gradeTable->setItem(row, COL_AVERAGE,
                                new QTableWidgetItem(QString::number(avg, 'f', 2)));
        }
    }
}

void MainWindow::searchStudent()
{
    auto &manager = StudentManager::GetInstance();
    QString name = nameInput->text().trimmed();

    if (name.isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("请输入学生姓名"));
        return;
    }

    auto grades = manager->getStudentGrades(name.toStdString());
    if (grades.empty())
    {
        QMessageBox::information(this, tr("提示"),
                                 tr("未找到学生 %1 的成绩记录").arg(name));
        return;
    }

    std::vector<std::pair<std::string, std::vector<Grade>>> data;
    data.emplace_back(name.toStdString(), std::move(grades));
    updateTable(data);
}

void MainWindow::showAllStudents()
{
    auto &manager = StudentManager::GetInstance();
    const auto &allGrades = manager->getAllGrades();

    if (allGrades.empty())
    {
        QMessageBox::information(this, tr("提示"), tr("暂无学生成绩记录"));
        return;
    }

    std::vector<std::pair<std::string, std::vector<Grade>>> data;
    for (const auto &[name, grades] : allGrades)
    {
        data.emplace_back(name, grades);
    }
    updateTable(data);
}

void MainWindow::addNewGrade()
{
    QString name = nameInput->text().trimmed();
    QString subject = subjectInput->text().trimmed();
    QString scoreText = scoreInput->text().trimmed();

    if (name.isEmpty() || subject.isEmpty() || scoreText.isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("请填写完整信息"));
        return;
    }

    bool ok;
    float score = scoreText.toFloat(&ok);

    if (!ok || score < 0 || score > 100)
    {
        QMessageBox::warning(this, tr("错误"), tr("请输入有效的分数(0-100)"));
        return;
    }

    auto &manager = StudentManager::GetInstance();
    manager->addGrade(name.toStdString(), subject.toStdString(), score);

    QMessageBox::information(this, tr("成功"), tr("成绩添加成功！"));

    // 刷新显示
    std::vector<std::pair<std::string, std::vector<Grade>>> data;
    data.emplace_back(name.toStdString(), manager->getStudentGrades(name.toStdString()));
    updateTable(data);

    // 清空输入
    subjectInput->clear();
    scoreInput->clear();
}