
#include "mainwindow.h"
#include "Search.h"
#include "TextEdit.h"
#include "Setting.h"

#include <QFrame>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QObject>
#include <QDebug>
#include <QMenuBar>

struct menu {
    QMenuBar *menubar;
    QMenu *menu_options;
    QAction *act_settings;
    QAction *act_export;
};

struct ui {
    QLineEdit *ledit_input;
    QPushButton *btn_search;
    QPushButton *btn_play;
    QPushButton *btn_add;
    QPushButton *btn_add_sentence;
    QTextEdit *tedit_display;

    struct menu *menu;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_ui = new struct ui;
    m_ui->ledit_input = new QLineEdit();
    m_ui->btn_search = new QPushButton(QIcon(":/images/images/search1.ico"), tr("Search"));
    m_ui->btn_play = new QPushButton(QIcon(":/images/images/sound.ico"), tr("Play"));
    m_ui->btn_add = new QPushButton(QIcon(":/images/images/edit.ico"), tr("Edit Word"));
    m_ui->btn_add_sentence = new QPushButton(QIcon(":/images/images/edit.ico"), tr("Edit Sentence"));
    m_ui->tedit_display = new QTextEdit();

    m_highlighter = new TextHighLighter(m_ui->tedit_display->document());

    SetupMenu();

    m_settings = new DialogSet;
    m_diaglogeditword = new DialogEditWord;
    m_diaglogeditsentence = new DialogEditSentence;
    m_dialogexport = new DialogExport;

    connect(m_ui->btn_search, SIGNAL(clicked()), this, SLOT(Search_Btn_Slot()));
    connect(m_ui->btn_play, SIGNAL(clicked()), this, SLOT(Play_Btn_Slot()));
    connect(m_ui->btn_add, SIGNAL(clicked()), this, SLOT(Edit_Btn_Slot()));
    connect(m_ui->btn_add_sentence, SIGNAL(clicked()), m_diaglogeditsentence, SLOT(Open()));
    connect(m_diaglogeditword, SIGNAL(Apply_Signal(Word)), this, SLOT(Record_Word_Edited_Slot(Word)));

    Layout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::Layout()
{
    QFrame *frame = new QFrame(this);
    this->setCentralWidget(frame);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(m_ui->ledit_input, 0);
    hlayout->addWidget(m_ui->btn_search, 0);
    hlayout->addWidget(m_ui->btn_play, 0);
    hlayout->addWidget(m_ui->btn_add, 0);
    hlayout->addWidget(m_ui->btn_add_sentence, 0);

    QHBoxLayout *showlayout = new QHBoxLayout();
    showlayout->addWidget(m_ui->tedit_display,0);

    QVBoxLayout *mainlayout = new QVBoxLayout();
    mainlayout->addLayout(hlayout);
    mainlayout->addWidget(m_ui->tedit_display);

    this->centralWidget()->setLayout(mainlayout);
}

void MainWindow::SetupMenu()
{
    m_ui->menu = new struct menu;
    m_ui->menu->menubar = this->menuBar();
    m_ui->menu->menu_options = m_ui->menu->menubar->addMenu(tr("Options"));
    m_ui->menu->act_settings = m_ui->menu->menu_options->addAction(tr("Settings"));
    m_ui->menu->act_export = m_ui->menu->menu_options->addAction(tr("Export"));

    connect(m_ui->menu->menubar, SIGNAL(triggered(QAction *)), this, SLOT(MenuBarTrigger_Slot(QAction *)));
}

void MainWindow::MenuBarTrigger_Slot(QAction *act)
{
    if(act == m_ui->menu->act_settings)
    {
        m_settings->Open();
    }
    else if(act == m_ui->menu->act_export)
    {
        m_dialogexport->Open();
    }
}

void MainWindow::Search_Btn_Slot()
{
    m_ui->tedit_display->clear();

    QString searchword = m_ui->ledit_input->text().trimmed().toLower();
    if(searchword.isEmpty())
    {
        m_results.clear();
        return;
    }

    QTextCharFormat fmt_word;
    fmt_word.setForeground(Qt::red);
    m_highlighter->Clear();
    m_highlighter->AddRule(QRegularExpression(searchword), fmt_word);

    Search search;
    m_results = search.SearchTarget(Settings::ToStringList(SETS.GetGroup(GROUP_SEARCH_FILES)), searchword);

    for(QList<SearchResult *>::iterator it = m_results.begin(); it != m_results.end(); it++)
    {
        (*it)->Display(m_ui->tedit_display);

        if((*it)->Type() == SearchResult::E_TYPE_WORD && searchword == (dynamic_cast<Word *>(*it))->GetWord())
        {
            (*it)->Update();
        }
    }

    if(nullptr == GetWordFromSearchResults() && Word::IsEnglishWord(searchword))
    {
        Word wd;
        wd.SetWord(searchword);
        wd.Record(SETS[KEY_UNNOTE_WORD_FILE].toString());
    }
}

void MainWindow::Edit_Btn_Slot()
{
    Word *pwd = GetWordFromSearchResults();

    if(nullptr == pwd)
    {
        Word wd;
        wd.SetWord(m_ui->ledit_input->text().trimmed());
        m_diaglogeditword->SetWord(wd);
    }
    else
    {
        m_diaglogeditword->SetWord(*pwd);
    }

    m_diaglogeditword->Open();
}

void MainWindow::Record_Word_Edited_Slot(Word wd)
{
    Word *pwd = GetWordFromSearchResults();

    if(nullptr != pwd && pwd->GetWord() == wd.GetWord())
    {
        wd.Record(pwd->GetPathfile());
        return;
    }

    Search search;
    QList<SearchResult *> results = search.SearchTarget(Settings::ToStringList(SETS.GetGroup(GROUP_SEARCH_FILES)), wd.GetWord());

    for(QList<SearchResult *>::iterator it = results.begin(); it != results.end(); it++)
    {
        if((*it)->Type() == SearchResult::E_TYPE_WORD && dynamic_cast<Word*>(*it)->GetWord() == wd.GetWord())
        {
             wd.Record((*it)->GetPathfile());
             return;
        }
    }

    wd.Record(SETS[KEY_UNNOTE_WORD_FILE].toString());
}

Word *MainWindow::GetWordFromSearchResults()
{
    for(QList<SearchResult *>::iterator it = m_results.begin(); it != m_results.end(); it++)
    {
        if((*it)->Type() == SearchResult::E_TYPE_WORD)
        {
            return dynamic_cast<Word*>(*it);
        }
    }

    return nullptr;
}

void MainWindow::Play_Btn_Slot()
{
    Word *pwd = GetWordFromSearchResults();

    if(nullptr != pwd)
    {
        pwd->Play(SETS[KEY_SOUND_VOLUME].toInt());
    }
}
