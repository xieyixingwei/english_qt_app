#include "DialogEditWord.h"
#include "Word.h"
#include "Sentence.h"
#include "Search.h"
#include "Setting.h"
#include "DialogSet.h"

#include "QListWidgetCm.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QList>
#include <QString>
#include <QListWidgetItem>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QRegExp>
#include <QListWidget>
#include <QPushButton>
#include <QDialog>
#include <QComboBox>
#include <QGroupBox>

struct ui {
    QDialog * dialog;

    QLineEdit *ledit_word;
    QLineEdit *ledit_soundmark;
    QLineEdit *ledit_hot;
    QLineEdit *ledit_timestamp;
    QLineEdit *ledit_pattern;
    QLineEdit *ledit_tense;

    QLineEdit *ledit_pathfile;

    QLineEdit *ledit_mean;

    QLineEdit *ledit_example_a;
    QLineEdit *ledit_example_b;

    QComboBox *comb_pattern;
    QComboBox *comb_tense;

    QComboBox *comb_means;

    QPushButton *btn_search;
    QPushButton *btn_addpattern;
    QPushButton *btn_addtense;
    QPushButton *btn_addexample;

    QPushButton *btn_recordword;
    QPushButton *btn_cancel;

    QListWidgetCm *lwdg_interpretation;

    QGroupBox *groupbox_example;
};

DialogEditWord::DialogEditWord() :
    m_wd(new Word)
{
    m_ui = new struct ui;

    m_ui->dialog = new QDialog(nullptr, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    m_ui->dialog->setWindowTitle(tr("Editting Word"));

    m_ui->ledit_word = new QLineEdit;
    m_ui->ledit_soundmark = new QLineEdit;
    m_ui->ledit_hot = new QLineEdit;
    m_ui->ledit_timestamp = new QLineEdit;
    m_ui->ledit_pattern = new QLineEdit;
    m_ui->ledit_tense = new QLineEdit;
    m_ui->ledit_pathfile = new QLineEdit;
    m_ui->ledit_mean = new QLineEdit;
    m_ui->ledit_example_a = new QLineEdit;
    m_ui->ledit_example_b = new QLineEdit;

    m_ui->comb_pattern = new QComboBox;
    m_ui->comb_tense = new QComboBox;
    m_ui->comb_means = new QComboBox;

    m_ui->btn_search = new QPushButton(tr("Search"));
    m_ui->btn_addpattern = new QPushButton(tr("Add Pattern"));
    m_ui->btn_addtense = new QPushButton(tr("Add Tense"));
    m_ui->btn_addexample = new QPushButton(tr("Add Example"));

    m_ui->btn_recordword = new QPushButton(tr("Record Word"));
    m_ui->btn_cancel = new QPushButton(tr("Cancel"));

    m_ui->lwdg_interpretation = new QListWidgetCm;
    m_ui->groupbox_example = new QGroupBox(tr("Example Sentence"));

    connect(m_ui->btn_addpattern, SIGNAL(clicked()), this, SLOT(Add_Btn_Slot()));
    connect(m_ui->btn_addtense, SIGNAL(clicked()), this, SLOT(Add_Btn_Slot()));
    connect(m_ui->btn_addexample, SIGNAL(clicked()), this, SLOT(Add_Example_Btn_Slot()));

    connect(m_ui->btn_recordword, SIGNAL(clicked()), this, SLOT(RecordWord_Btn_Slot()));
    connect(m_ui->btn_cancel, SIGNAL(clicked()), m_ui->dialog, SLOT(close()));
    connect(m_ui->btn_search, SIGNAL(clicked()), this, SLOT(Search_Btn_Slot()));

    Init();
    Layout();

    m_ui->dialog->close();
}

DialogEditWord::~DialogEditWord()
{

}

void DialogEditWord::Init()
{
    const char* const KEY_SENTENCE_PATTERN = "Sentence_Pattern";
    m_ui->comb_pattern->addItems(SETS[KEY_SENTENCE_PATTERN].toString().split(","));

    const char* const KEY_SENTENCE_TENSE = "Sentence_Tense";
    m_ui->comb_tense->addItems(SETS[KEY_SENTENCE_TENSE].toString().split(","));

    const char* const KEY_WORD_MEANS("Part_Of_Speech");
    m_ui->comb_means->addItems(SETS[KEY_WORD_MEANS].toString().split(","));
}

void DialogEditWord::Layout()
{
    QHBoxLayout *hlayout_a = new QHBoxLayout;
    hlayout_a->addWidget(new QLabel(tr("Word:")), 0);
    hlayout_a->addWidget(m_ui->ledit_word, 0);
    hlayout_a->addWidget(m_ui->btn_search, 0);
    hlayout_a->addWidget(m_ui->ledit_pathfile, 0);

    QHBoxLayout *hlayout_b = new QHBoxLayout;
    hlayout_b->addWidget(new QLabel(tr("SoundMark:")), 0);
    hlayout_b->addWidget(m_ui->ledit_soundmark, 0);
    hlayout_b->addWidget(new QLabel(tr("Hot:")), 0);
    hlayout_b->addWidget(m_ui->ledit_hot, 0);
    hlayout_b->addWidget(new QLabel(tr("TimeStamp:")), 0);
    hlayout_b->addWidget(m_ui->ledit_timestamp, 0);

    QHBoxLayout *hlayout_c = new QHBoxLayout;
    hlayout_c->addWidget(new QLabel(tr("Mean:")), 0);
    hlayout_c->addWidget(m_ui->ledit_mean, 0);
    hlayout_c->addWidget(m_ui->comb_means, 0);

    QHBoxLayout *hlayout_d = new QHBoxLayout;
    hlayout_d->addWidget(new QLabel(tr("Pattern:")), 0);
    hlayout_d->addWidget(m_ui->ledit_pattern, 0);
    hlayout_d->addWidget(m_ui->comb_pattern, 0);
    hlayout_d->addWidget(m_ui->btn_addpattern, 0);

    QHBoxLayout *hlayout_e = new QHBoxLayout;
    hlayout_e->addWidget(new QLabel(tr("Tense:")), 0);
    hlayout_e->addWidget(m_ui->ledit_tense, 0);
    hlayout_e->addWidget(m_ui->comb_tense, 0);
    hlayout_e->addWidget(m_ui->btn_addtense, 0);

    QHBoxLayout *hlayout_f = new QHBoxLayout;
    hlayout_f->addWidget(m_ui->btn_recordword, 0);
    hlayout_f->addWidget(m_ui->btn_cancel, 0);

    QVBoxLayout *vlayout_a= new QVBoxLayout;
    vlayout_a->addLayout(hlayout_d, 0);
    vlayout_a->addLayout(hlayout_e, 0);
    vlayout_a->addWidget(m_ui->ledit_example_a, 0);
    vlayout_a->addWidget(m_ui->ledit_example_b, 0);

    m_ui->groupbox_example->setLayout(vlayout_a);

    QHBoxLayout *hlayout_g = new QHBoxLayout;
    hlayout_g->addWidget(m_ui->lwdg_interpretation, 0);
    hlayout_g->addWidget(m_ui->groupbox_example, 0);

    QVBoxLayout *vlayout = new QVBoxLayout(m_ui->dialog);
    vlayout->addLayout(hlayout_a, 0);
    vlayout->addLayout(hlayout_b, 0);
    vlayout->addLayout(hlayout_c, 0);
    vlayout->addLayout(hlayout_g, 0);
    vlayout->addLayout(hlayout_f, 0);
}

void DialogEditWord::Open()
{
    m_ui->dialog->open();
}

void DialogEditWord::Close()
{
    m_ui->dialog->close();
}

void DialogEditWord::SetWord(Word &wd)
{
    *m_wd = wd;
    DisplayWord();
}

void DialogEditWord::DisplayWord()
{
    m_ui->ledit_word->setText(m_wd->GetWord());
    m_ui->ledit_soundmark->setText(m_wd->GetSoundMark());
    m_ui->ledit_hot->setText(m_wd->GetHot());
    m_ui->ledit_timestamp->setText(m_wd->GetTimeStamp());
    m_ui->lwdg_interpretation->clear();
    m_ui->lwdg_interpretation->addItems(m_wd->ToInerpretionDisplayString().remove(QRegExp("\\n$")).split("\n"));
    m_ui->ledit_pathfile->setText(m_wd->GetPathfile());
}

void DialogEditWord::SnapshotWord()
{
    m_wd->SetWord(m_ui->ledit_word->text().trimmed());
    m_wd->SetSoundMark(m_ui->ledit_soundmark->text().trimmed().remove("/").remove("[").remove("]"));
    m_wd->SetHot(m_ui->ledit_hot->text().trimmed());
    m_wd->SetTimeStamp(m_ui->ledit_timestamp->text().trimmed());
    m_wd->SetInerpretion(WordInterpretation::ToWordInterpretationList(ToInterpretationString()));
    m_wd->SetPathfile(m_ui->ledit_pathfile->text().trimmed());
}

QStringList DialogEditWord::ToInterpretationString()
{
    QStringList itps;

    for(int i = 0; i < m_ui->lwdg_interpretation->count(); i++)
    {
        QString line = m_ui->lwdg_interpretation->item(i)->text().remove(QRegExp("\\(\\d*\\)"));

        if(line.contains(QRegExp("^    [a-zA-Z]+.*")))
        {
            line = "    + " + line.trimmed();
        }
        else
        {
            line = "        * " + line.trimmed();
        }

        itps << line;
    }

    return itps;
}

void DialogEditWord::Add_Btn_Slot()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());

    if(m_ui->btn_addpattern == btn)
    {
        if(m_ui->ledit_pattern->text().trimmed().isEmpty())
        {
            m_ui->ledit_pattern->setText(m_ui->comb_pattern->currentText());
        }
        else
        {
            m_ui->ledit_pattern->setText(m_ui->ledit_pattern->text() + "," + m_ui->comb_pattern->currentText());
        }
    }
    else if(m_ui->btn_addtense == btn)
    {
        if(m_ui->ledit_tense->text().trimmed().isEmpty())
        {
            m_ui->ledit_tense->setText(m_ui->comb_tense->currentText());
        }
        else
        {
            m_ui->ledit_tense->setText(m_ui->ledit_tense->text() + "," + m_ui->comb_tense->currentText());
        }
    }
}

void DialogEditWord::Add_Example_Btn_Slot()
{
    QString mean = m_ui->ledit_mean->text().section(".", 0, 0).trimmed();
    for(int i = 0; i < m_ui->comb_means->count(); i++)
    {
        if(mean == m_ui->comb_means->itemText(i))
        {
             m_ui->comb_means->setCurrentIndex(i);
        }
    }

    QStringList textipt;
    textipt << QString("+ %1. %2").arg(m_ui->comb_means->currentText()).arg(m_ui->ledit_mean->text().remove(QRegExp("[a-zA-Z]+\\.")).replace("；", "; ").replace("，",","));
    textipt << QString("    * %1").arg(m_ui->ledit_example_b->text().trimmed().remove("\n").remove("\r").remove("\t"));

    SnapshotWord();
    m_wd->MergeInerpretion(WordInterpretation(textipt));
    DisplayWord();
}

void DialogEditWord::RecordWord_Btn_Slot()
{
    if(m_ui->ledit_word->text().trimmed().isEmpty())
    {
        return;
    }

    SnapshotWord();
    emit Apply_Signal(*m_wd);
}

void DialogEditWord::Ok_Btn_Slot()
{
    m_ui->dialog->close();
}

void DialogEditWord::Search_Btn_Slot()
{
    QString searchstr = m_ui->ledit_word->text().trimmed();
    if(searchstr.isEmpty())
    {
        return;
    }

    m_ui->ledit_mean->clear();
    m_ui->ledit_example_a->clear();
    m_ui->ledit_example_b->clear();

    m_wd->Clear();
    m_wd->SetWord(searchstr);
    DisplayWord();

    Search search;
    QList<SearchResult *> results = search.SearchTarget(SETS.GetGroupAllValue(DialogSet::GROUP_SEARCH_PATH_FILE), searchstr);

    for(QList<SearchResult *>::iterator it = results.begin(); it != results.end(); it++)
    {
        if((*it)->GetType() == "word")
        {
            *m_wd = *(dynamic_cast<Word*>(*it));
            DisplayWord();
            break;
        }
    }
}
