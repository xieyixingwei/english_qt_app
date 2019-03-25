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

struct ui {
    QDialog * dialog;
    QLineEdit *ledit_word;
    QLineEdit *ledit_soundmark;
    QLineEdit *ledit_hot;
    QLineEdit *ledit_timestamp;
    QLineEdit *ledit_tag;
    QLineEdit *ledit_sort;

    QLineEdit *ledit_pathfile;

    QLineEdit *ledit_mean;

    QLineEdit *ledit_example_a;
    QLineEdit *ledit_example_tag;
    QLineEdit *ledit_example_b;

    QLineEdit *ledit_sentence_a;
    QLineEdit *ledit_sentence_b;

    QComboBox *comb_tags;
    QComboBox *comb_sorts;

    QComboBox *comb_means;
    QComboBox *comb_example_tags;

    QPushButton *btn_search;
    QPushButton *btn_addtag;
    QPushButton *btn_addsort;
    QPushButton *btn_addexampletag;
    QPushButton *btn_addexample;

    QPushButton *btn_recordsentence;
    QPushButton *btn_recordword;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;

    QListWidgetCm *lwdg_interpretation;
};

DialogEditWord::DialogEditWord() :
    m_wd(new Word)
{
    m_ui = new struct ui;

    m_ui->dialog = new QDialog(nullptr, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    m_ui->dialog->setWindowTitle(tr("Editting Word"));

    m_ui->ledit_word = new QLineEdit;
    m_ui->ledit_soundmark = new QLineEdit;
    m_ui->ledit_hot = new QLineEdit;
    m_ui->ledit_timestamp = new QLineEdit;
    m_ui->ledit_tag = new QLineEdit;
    m_ui->ledit_sort = new QLineEdit;
    m_ui->ledit_pathfile = new QLineEdit;
    m_ui->ledit_mean = new QLineEdit;
    m_ui->ledit_example_a = new QLineEdit;
    m_ui->ledit_example_tag = new QLineEdit;
    m_ui->ledit_example_b = new QLineEdit;

    m_ui->ledit_sentence_a = new QLineEdit;
    m_ui->ledit_sentence_b = new QLineEdit;

    m_ui->comb_tags = new QComboBox;
    m_ui->comb_sorts = new QComboBox;
    m_ui->comb_means = new QComboBox;
    m_ui->comb_example_tags = new QComboBox;

    m_ui->btn_search = new QPushButton(tr("Search"));
    m_ui->btn_addtag = new QPushButton(tr("Add Tag"));
    m_ui->btn_addsort = new QPushButton(tr("Add Sort"));
    m_ui->btn_addexampletag = new QPushButton(tr("Add"));
    m_ui->btn_addexample = new QPushButton(tr("Add Example"));

    m_ui->btn_recordsentence = new QPushButton(tr("Record Sentence"));
    m_ui->btn_recordword = new QPushButton(tr("Record Word"));
    m_ui->btn_ok = new QPushButton(tr("Ok"));
    m_ui->btn_cancel = new QPushButton(tr("Cancel"));

    m_ui->lwdg_interpretation = new QListWidgetCm;

    connect(m_ui->btn_addtag, SIGNAL(clicked()), this, SLOT(Add_Btn_Slot()));
    connect(m_ui->btn_addsort, SIGNAL(clicked()), this, SLOT(Add_Btn_Slot()));
    connect(m_ui->btn_addexampletag, SIGNAL(clicked()), this, SLOT(Add_Btn_Slot()));
    connect(m_ui->btn_addexample, SIGNAL(clicked()), this, SLOT(Add_Example_Btn_Slot()));

    connect(m_ui->btn_recordword, SIGNAL(clicked()), this, SLOT(RecordWord_Btn_Slot()));
    connect(m_ui->btn_ok, SIGNAL(clicked()), this, SLOT(Ok_Btn_Slot()));
    connect(m_ui->btn_cancel, SIGNAL(clicked()), m_ui->dialog, SLOT(close()));
    connect(m_ui->btn_search, SIGNAL(clicked()), this, SLOT(Search_Btn_Slot()));
    connect(m_ui->btn_recordsentence, SIGNAL(clicked()), this, SLOT(RecordSentence_Btn_Slot()));

    Init();
    Layout();

    m_ui->dialog->close();
}

DialogEditWord::~DialogEditWord()
{

}

void DialogEditWord::Init()
{
    QStringList tags;
    tags << "a" << "b" << "c";
    m_ui->comb_tags->addItems(tags);

    QStringList sorts;
    sorts << "animal" << "fruits" << "vegetables";
    m_ui->comb_sorts->addItems(sorts);

    QStringList means;
    means << "n" << "vi" << "vt" << "adj" << "adv";
    m_ui->comb_means->addItems(means);

    QStringList exptags;
    exptags << "phrase" << "tongue" << "argot";
    m_ui->comb_example_tags->addItems(exptags);
}

void DialogEditWord::Layout()
{
    QHBoxLayout *hlayout_a = new QHBoxLayout;
    hlayout_a->addWidget(new QLabel(tr("Word:")), 0);
    hlayout_a->addWidget(m_ui->ledit_word, 0);
    hlayout_a->addWidget(new QLabel(tr("SoundMark:")), 0);
    hlayout_a->addWidget(m_ui->ledit_soundmark, 0);
    hlayout_a->addWidget(new QLabel(tr("Hot:")), 0);
    hlayout_a->addWidget(m_ui->ledit_hot, 0);
    hlayout_a->addWidget(new QLabel(tr("TimeStamp:")), 0);
    hlayout_a->addWidget(m_ui->ledit_timestamp, 0);

    QHBoxLayout *hlayout_b = new QHBoxLayout;
    hlayout_b->addWidget(new QLabel(tr("Tag:")), 0);
    hlayout_b->addWidget(m_ui->ledit_tag, 0);
    hlayout_b->addWidget(m_ui->comb_tags, 0);
    hlayout_b->addWidget(m_ui->btn_addtag, 0);

    QHBoxLayout *hlayout_c = new QHBoxLayout;
    hlayout_c->addWidget(new QLabel(tr("Sort:")), 0);
    hlayout_c->addWidget(m_ui->ledit_sort, 0);
    hlayout_c->addWidget(m_ui->comb_sorts, 0);
    hlayout_c->addWidget(m_ui->btn_addsort, 0);

    QVBoxLayout *vlayout_aa = new QVBoxLayout;
    vlayout_aa->addWidget(m_ui->btn_search, 0);
    vlayout_aa->addWidget(m_ui->ledit_pathfile, 0);

    QVBoxLayout *vlayout_ab = new QVBoxLayout;
    vlayout_ab->addLayout(hlayout_b, 0);
    vlayout_ab->addLayout(hlayout_c, 0);

    QHBoxLayout *hlayout_aa = new QHBoxLayout;
    hlayout_aa->addLayout(vlayout_aa, 0);
    hlayout_aa->addLayout(vlayout_ab, 0);

    QHBoxLayout *hlayout_d = new QHBoxLayout;
    hlayout_d->addWidget(new QLabel(tr("Mean:")), 0);
    hlayout_d->addWidget(m_ui->ledit_mean, 0);
    hlayout_d->addWidget(m_ui->comb_means, 0);

    QHBoxLayout *hlayout_e = new QHBoxLayout;
    hlayout_e->addWidget(new QLabel(tr("Example Tag:")), 0);
    hlayout_e->addWidget(m_ui->ledit_example_tag, 0);
    hlayout_e->addWidget(m_ui->comb_example_tags, 0);
    hlayout_e->addWidget(m_ui->btn_addexampletag, 0);

    QHBoxLayout *hlayout_g = new QHBoxLayout;
    hlayout_g->addWidget(m_ui->btn_recordword, 0);
    hlayout_g->addWidget(m_ui->btn_recordsentence, 0);
    hlayout_g->addWidget(m_ui->btn_ok, 0);
    hlayout_g->addWidget(m_ui->btn_cancel, 0);

    QVBoxLayout *vlayout_a = new QVBoxLayout;
    vlayout_a->addLayout(hlayout_d, 0);
    vlayout_a->addLayout(hlayout_e, 0);
    vlayout_a->addWidget(m_ui->ledit_example_a, 0);
    vlayout_a->addWidget(m_ui->ledit_example_b, 0);
    vlayout_a->addWidget(m_ui->btn_addexample, 0);

    QHBoxLayout *hlayout_h = new QHBoxLayout;
    hlayout_h->addWidget(m_ui->lwdg_interpretation);
    hlayout_h->addLayout(vlayout_a, 0);

    QVBoxLayout *vlayout = new QVBoxLayout(m_ui->dialog);
    vlayout->addLayout(hlayout_a, 0);
    vlayout->addLayout(hlayout_aa, 0);
    vlayout->addLayout(hlayout_h, 0);
    vlayout->addWidget(m_ui->ledit_sentence_a, 0);
    vlayout->addWidget(m_ui->ledit_sentence_b, 0);
    vlayout->addLayout(hlayout_g, 0);
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
    m_ui->ledit_tag->setText(m_wd->GetTags().join(","));
    m_ui->ledit_sort->setText(m_wd->GetSorts().join(","));
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
    m_wd->SetTags(m_ui->ledit_tag->text().trimmed().remove(" ").split(","));
    m_wd->SetSorts(m_ui->ledit_sort->text().trimmed().remove(" ").split(","));
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

    if(m_ui->btn_addtag == btn)
    {
        if(m_ui->ledit_tag->text().trimmed().isEmpty())
        {
            m_ui->ledit_tag->setText(m_ui->comb_tags->currentText());
        }
        else
        {
            m_ui->ledit_tag->setText(m_ui->ledit_tag->text() + "," + m_ui->comb_tags->currentText());
        }
    }
    else if(m_ui->btn_addsort == btn)
    {
        if(m_ui->ledit_sort->text().trimmed().isEmpty())
        {
            m_ui->ledit_sort->setText(m_ui->comb_sorts->currentText());
        }
        else
        {
            m_ui->ledit_sort->setText(m_ui->ledit_sort->text() + "," + m_ui->comb_sorts->currentText());
        }
    }
    else if(m_ui->btn_addexampletag == btn)
    {
        if(m_ui->ledit_example_tag->text().trimmed().isEmpty())
        {
            m_ui->ledit_example_tag->setText(m_ui->comb_example_tags->currentText());
        }
        else
        {
            m_ui->ledit_example_tag->setText(m_ui->ledit_example_tag->text() + "," + m_ui->comb_example_tags->currentText());
        }
    }
}

void DialogEditWord::Add_Example_Btn_Slot()
{
    QStringList textipt;
    textipt << QString("+ %1. %2").arg(m_ui->comb_means->currentText()).arg(m_ui->ledit_mean->text());
    textipt << QString("    * %1 <tag:%2>").arg(m_ui->ledit_example_a->text().trimmed().remove("\n").remove("\r").remove("\t")).arg(m_ui->ledit_example_tag->text());
    textipt << QString("    * %1").arg(m_ui->ledit_example_b->text().trimmed().remove("\n").remove("\r").remove("\t"));

    SnapshotWord();
    m_wd->MergeInerpretion(WordInterpretation(textipt));
    DisplayWord();
}

void DialogEditWord::RecordWord_Btn_Slot()
{
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

void DialogEditWord::RecordSentence_Btn_Slot()
{
    Sentence stc(m_ui->ledit_sentence_a->text().trimmed().remove("\n").remove("\r").remove("\t")
                 , m_ui->ledit_sentence_b->text().trimmed().remove("\n").remove("\r").remove("\t"));
    stc.Record(SETS[DialogSet::KEY_SENTENCE_FILE].toString());
}
