#include "mainwindow.h"
#include <QFrame>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QObject>
#include "search.h"

struct ui {
    QLineEdit *le_input;
    QPushButton *btn_search;
    QPushButton *btn_add;
    QPlainTextEdit *pte_show;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMaximumHeight(50);

    QFrame *frame = new QFrame(this);
    this->setCentralWidget(frame);

    m_ui = new struct ui;

    m_ui->le_input = new QLineEdit();
    m_ui->btn_search = new QPushButton(tr("search"));
    m_ui->btn_add = new QPushButton(tr("add/edit"));
    m_ui->pte_show = new QPlainTextEdit();

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(m_ui->le_input, 0);
    hlayout->addWidget(m_ui->btn_search, 0);
    hlayout->addWidget(m_ui->btn_add, 0);

    QHBoxLayout *showlayout = new QHBoxLayout();
    showlayout->addWidget(m_ui->pte_show,0);

    QVBoxLayout *mainlayout = new QVBoxLayout();
    mainlayout->addLayout(hlayout);
    mainlayout->addWidget(m_ui->pte_show);

    this->centralWidget()->setLayout(mainlayout);

    QObject::connect(m_ui->btn_search, SIGNAL(clicked()), this, SLOT(search_btn_slot()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::search_btn_slot()
{
    Search search;
    QStringList match = search.SearchWord("Z:/Study/english/words/word-190111.md", m_ui->le_input->text());
    //m_ui->pte_show->setPlainText(match.first());
}
