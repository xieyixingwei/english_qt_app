#include "DialogEditWord.h"
#include "Word.h"
#include "Sentence.h"
#include "Search.h"
#include "Setting.h"
#include "DialogSet.h"
#include "TextEdit.h"
#include "DialogEditSentence.h"
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
    QLineEdit *ledit_tags;
    QLineEdit *ledit_mean;
    QLineEdit *ledit_example_a;
    QLineEdit *ledit_example_b;

    QComboBox *comb_tags;
    QComboBox *comb_means;
    QComboBox *comb_pattern;
    QComboBox *comb_tense;

    QPushButton *btn_search;
    QPushButton *btn_addtag;
    QPushButton *btn_addpattern;
    QPushButton *btn_addtense;
    QPushButton *btn_addexample;
    QPushButton *btn_clear;

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
    m_ui->ledit_tags = new QLineEdit;
    m_ui->ledit_soundmark = new QLineEdit;
    m_ui->ledit_hot = new QLineEdit;
    m_ui->ledit_timestamp = new QLineEdit;
    m_ui->ledit_pattern = new QLineEdit;
    m_ui->ledit_tense = new QLineEdit;
    m_ui->ledit_pathfile = new QLineEdit;
    m_ui->ledit_pathfile->setReadOnly(true);
    m_ui->ledit_mean = new QLineEdit;
    m_ui->ledit_example_a = new QLineEdit;
    m_ui->ledit_example_b = new QLineEdit;

    m_ui->comb_tags = new QComboBox;
    m_ui->comb_pattern = new QComboBox;
    m_ui->comb_tense = new QComboBox;
    m_ui->comb_means = new QComboBox;

    m_ui->btn_search = new QPushButton(tr("Search"));
    m_ui->btn_addtag = new QPushButton(tr("Add Tag"));
    m_ui->btn_addpattern = new QPushButton(tr("Add Pattern"));
    m_ui->btn_addtense = new QPushButton(tr("Add Tense"));
    m_ui->btn_addexample = new QPushButton(tr("Add Example"));
    m_ui->btn_clear = new QPushButton(tr("Clear"));
    m_ui->btn_recordword = new QPushButton(tr("Record Word"));
    m_ui->btn_cancel = new QPushButton(tr("Cancel"));

    m_ui->lwdg_interpretation = new QListWidgetCm;
    m_ui->groupbox_example = new QGroupBox(tr("Example Sentence"));

    connect(m_ui->btn_addtag, SIGNAL(clicked()), this, SLOT(Add_Btn_Slot()));
    connect(m_ui->btn_addpattern, SIGNAL(clicked()), this, SLOT(Add_Btn_Slot()));
    connect(m_ui->btn_addtense, SIGNAL(clicked()), this, SLOT(Add_Btn_Slot()));
    connect(m_ui->btn_addexample, SIGNAL(clicked()), this, SLOT(Add_Example_Btn_Slot()));
    connect(m_ui->btn_clear, SIGNAL(clicked()), this, SLOT(Clear_Btn_Slot()));

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
    m_ui->comb_pattern->addItems(SETS[KEY_SENTENCE_PATTERN].toString().split(","));
    m_ui->comb_tense->addItems(SETS[KEY_SENTENCE_TENSE].toString().split(","));
    m_ui->comb_means->addItems(SETS[KEY_PART_OF_SPEECH].toString().split(","));
    m_ui->comb_tags->addItems(SETS[KEY_WORD_TAGS].toString().split(","));
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
    hlayout_c->addWidget(new QLabel(tr("Tags:")), 0);
    hlayout_c->addWidget(m_ui->ledit_tags, 0);
    hlayout_c->addWidget(m_ui->comb_tags, 0);
    hlayout_c->addWidget(m_ui->btn_addtag, 0);

    QHBoxLayout *hlayout_d = new QHBoxLayout;
    hlayout_d->addWidget(new QLabel(tr("Mean:")), 0);
    hlayout_d->addWidget(m_ui->ledit_mean, 0);
    hlayout_d->addWidget(m_ui->comb_means, 0);

    QHBoxLayout *hlayout_e = new QHBoxLayout;
    hlayout_e->addWidget(new QLabel(tr("Pattern:")), 0);
    hlayout_e->addWidget(m_ui->ledit_pattern, 0);
    hlayout_e->addWidget(m_ui->comb_pattern, 0);
    hlayout_e->addWidget(m_ui->btn_addpattern, 0);

    QHBoxLayout *hlayout_f = new QHBoxLayout;
    hlayout_f->addWidget(new QLabel(tr("Tense:")), 0);
    hlayout_f->addWidget(m_ui->ledit_tense, 0);
    hlayout_f->addWidget(m_ui->comb_tense, 0);
    hlayout_f->addWidget(m_ui->btn_addtense, 0);

    QHBoxLayout *hlayout_g = new QHBoxLayout;
    hlayout_g->addWidget(m_ui->btn_addexample, 0);
    hlayout_g->addWidget(m_ui->btn_clear, 0);

    QHBoxLayout *hlayout_h = new QHBoxLayout;
    hlayout_h->addWidget(m_ui->btn_recordword, 0);
    hlayout_h->addWidget(m_ui->btn_cancel, 0);

    QVBoxLayout *vlayout_a= new QVBoxLayout;
    vlayout_a->addLayout(hlayout_d, 0);
    vlayout_a->addLayout(hlayout_e, 0);
    vlayout_a->addLayout(hlayout_f, 0);
    vlayout_a->addWidget(m_ui->ledit_example_a, 0);
    vlayout_a->addWidget(m_ui->ledit_example_b, 0);
    vlayout_a->addLayout(hlayout_g, 0);

    m_ui->groupbox_example->setLayout(vlayout_a);

    QHBoxLayout *hlayout_i = new QHBoxLayout;
    hlayout_i->addWidget(m_ui->lwdg_interpretation, 0);
    hlayout_i->addWidget(m_ui->groupbox_example, 0);

    QVBoxLayout *vlayout = new QVBoxLayout(m_ui->dialog);
    vlayout->addLayout(hlayout_a, 0);
    vlayout->addLayout(hlayout_b, 0);
    vlayout->addLayout(hlayout_c, 0);
    vlayout->addLayout(hlayout_i, 0);
    vlayout->addLayout(hlayout_h, 0);
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
    Refresh();
}

void DialogEditWord::Refresh()
{
    m_ui->ledit_word->setText(m_wd->GetWord());
    m_ui->ledit_pathfile->setText(m_wd->GetPathfile());
    m_ui->ledit_soundmark->setText(m_wd->GetSoundMark());
    m_ui->ledit_hot->setText(m_wd->GetHot());
    m_ui->ledit_timestamp->setText(m_wd->GetTimeStamp());
    m_ui->ledit_tags->setText(m_wd->GetTags().join(","));

    m_ui->lwdg_interpretation->clear();
    QStringList iters = m_wd->ToRecordString(0).remove(QRegularExpression("\\n$")).split("\n");
    iters.removeFirst();
    m_ui->lwdg_interpretation->addItems(iters);
}

void DialogEditWord::Add_Btn_Slot()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());

    if(m_ui->btn_addpattern == btn)
    {
        TextEdit text(m_ui->ledit_pattern->text().split(","));
        text.RemoveSpaceLines();
        text << m_ui->comb_pattern->currentText();
        m_ui->ledit_pattern->setText(text.Buf().join(","));
    }
    else if(m_ui->btn_addtense == btn)
    {
        TextEdit text(m_ui->ledit_tense->text().split(","));
        text.RemoveSpaceLines();
        text << m_ui->comb_tense->currentText();
        m_ui->ledit_tense->setText(text.Buf().join(","));
    }
    else if(m_ui->btn_addtag == btn)
    {
        TextEdit text(m_ui->ledit_tags->text().split(","));
        text.RemoveSpaceLines();
        text << m_ui->comb_tags->currentText();
        m_ui->ledit_tags->setText(text.Buf().join(","));
    }
}

void DialogEditWord::Add_Example_Btn_Slot()
{
    QString pos = m_ui->ledit_mean->text().section(".", 0, 0).trimmed();
    if(-1 != m_ui->comb_means->findText(pos))
    {
         m_ui->comb_means->setCurrentText(pos);
    }

    Sentence sent(m_ui->ledit_example_a->text(), m_ui->ledit_example_b->text());
    if(!m_ui->ledit_pattern->text().trimmed().isEmpty())
    {
        sent.AddPattern(m_ui->ledit_pattern->text().trimmed().split(","));
    }
    if(!m_ui->ledit_tense->text().trimmed().isEmpty())
    {
        sent.AddTense(m_ui->ledit_tense->text().trimmed().split(","));
    }

    WordInterpretation interp;
    interp.SetPos(m_ui->comb_means->currentText().trimmed());
    interp.SetMean(m_ui->ledit_mean->text().remove(QRegularExpression("[a-zA-Z]+\\.")).replace("；", "; ").replace("，",",").split(";"));
    interp.AddExample(sent);

    QList<WordInterpretation> interpretations = WordInterpretation::WordInterpretationList(m_ui->lwdg_interpretation->TextItems());

    int i = 0;
    for(i = 0; i < interpretations.count(); i++)
    {
        if(interp.GetPos() == interpretations[i].GetPos())
        {
            interpretations[i].AddMean(interp.GetMean());
            interpretations[i].AddExample(interp.GetExample());
            break;
        }
    }

    if(i == interpretations.count())
    {
        interpretations << interp;
    }

    QString display;
    for(i = 0; i < interpretations.count(); i++)
    {
        display += interpretations[i].ToRecordString(0);
    }

    m_ui->lwdg_interpretation->clear();
    m_ui->lwdg_interpretation->addItems(display.remove(QRegularExpression("\n$")).split("\n"));
}

void DialogEditWord::RecordWord_Btn_Slot()
{
    QString word = m_ui->ledit_word->text().trimmed();
    if(word.isEmpty())
    {
        return;
    }

    m_wd->Clear();
    m_wd->SetWord(word);
    m_wd->SetSoundMark(m_ui->ledit_soundmark->text().trimmed().remove('/'));
    m_wd->SetHot(m_ui->ledit_hot->text().trimmed());
    m_wd->SetTimeStamp(m_ui->ledit_timestamp->text().trimmed());
    m_wd->SetTag(m_ui->ledit_tags->text().trimmed().split(","));
    m_wd->SetInerpretion(WordInterpretation::WordInterpretationList(m_ui->lwdg_interpretation->TextItems()));

    emit Apply_Signal(*m_wd);
}

void DialogEditWord::Ok_Btn_Slot()
{
    m_ui->dialog->close();
}

void DialogEditWord::Clear_Btn_Slot()
{
    m_ui->ledit_mean->clear();
    m_ui->ledit_pattern->clear();
    m_ui->ledit_tense->clear();
    m_ui->ledit_example_a->clear();
    m_ui->ledit_example_b->clear();
}

void DialogEditWord::Search_Btn_Slot()
{
    QString searchStr = m_ui->ledit_word->text().trimmed();
    if(searchStr.isEmpty())
    {
        return;
    }

    Clear_Btn_Slot();
    m_ui->ledit_soundmark->clear();
    m_ui->ledit_hot->clear();
    m_ui->ledit_pathfile->clear();
    m_ui->ledit_timestamp->clear();
    m_ui->ledit_tags->clear();
    m_ui->lwdg_interpretation->clear();

    Search search;
    QList<SearchResult *> results = search.SearchTarget(Settings::ToStringList(SETS.GetGroup(GROUP_SEARCH_FILES)), searchStr);

    for(QList<SearchResult *>::iterator it = results.begin(); it != results.end(); it++)
    {
        if((*it)->Type() == SearchResult::E_TYPE_WORD)
        {
            *m_wd = *(dynamic_cast<Word*>(*it));
            Refresh();
            break;
        }
    }
}
