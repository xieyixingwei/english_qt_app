#include "DialogEditSentence.h"
#include "Sentence.h"
#include "Setting.h"
#include "DialogSet.h"

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

    QLineEdit *ledit_pattern;
    QLineEdit *ledit_tense;

    QLineEdit *ledit_sentence_a;
    QLineEdit *ledit_sentence_b;

    QComboBox *comb_pattern;
    QComboBox *comb_tense;

    QPushButton *btn_search;
    QPushButton *btn_add_pattern;
    QPushButton *btn_add_tense;

    QPushButton *btn_recordsentence;
    QPushButton *btn_cancel;
};

DialogEditSentence::DialogEditSentence()
{
    m_ui = new struct ui;

    m_ui->dialog = new QDialog(nullptr, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    m_ui->dialog->setWindowTitle(tr("Editting Sentence"));

    m_ui->ledit_pattern = new QLineEdit;
    m_ui->ledit_tense = new QLineEdit;

    m_ui->ledit_sentence_a = new QLineEdit;
    m_ui->ledit_sentence_b = new QLineEdit;

    m_ui->comb_pattern = new QComboBox;
    m_ui->comb_tense = new QComboBox;

    m_ui->btn_add_pattern = new QPushButton(tr("Add Pattern"));
    m_ui->btn_add_tense = new QPushButton(tr("Add Tense"));

    m_ui->btn_recordsentence = new QPushButton(tr("Record Sentence"));
    m_ui->btn_cancel = new QPushButton(tr("Cancel"));

    connect(m_ui->btn_add_pattern, SIGNAL(clicked()), this, SLOT(Add_Pattern_Btn_Slot()));
    connect(m_ui->btn_add_tense, SIGNAL(clicked()), this, SLOT(Add_Tense_Btn_Slot()));

    connect(m_ui->btn_recordsentence, SIGNAL(clicked()), this, SLOT(RecordSentence_Btn_Slot()));
    connect(m_ui->btn_cancel, SIGNAL(clicked()), m_ui->dialog, SLOT(close()));

    Init();
    Layout();

    m_ui->dialog->close();
}

DialogEditSentence::~DialogEditSentence()
{

}

void DialogEditSentence::Init()
{
    const char* const KEY_SENTENCE_PATTERN = "Sentence_Pattern";
    m_ui->comb_pattern->addItems(SETS[KEY_SENTENCE_PATTERN].toString().split(","));

    const char* const KEY_SENTENCE_TENSE = "Sentence_Tense";
    m_ui->comb_tense->addItems(SETS[KEY_SENTENCE_TENSE].toString().split(","));
}

void DialogEditSentence::Layout()
{
    QHBoxLayout *hlayout_a = new QHBoxLayout;
    hlayout_a->addWidget(new QLabel(tr("Pattern:")), 0);
    hlayout_a->addWidget(m_ui->ledit_pattern, 0);
    hlayout_a->addWidget(m_ui->comb_pattern, 0);
    hlayout_a->addWidget(m_ui->btn_add_pattern, 0);

    QHBoxLayout *hlayout_b = new QHBoxLayout;
    hlayout_b->addWidget(new QLabel(tr("Tense:")), 0);
    hlayout_b->addWidget(m_ui->ledit_tense, 0);
    hlayout_b->addWidget(m_ui->comb_tense, 0);
    hlayout_b->addWidget(m_ui->btn_add_tense, 0);

    QHBoxLayout *hlayout_c = new QHBoxLayout;
    hlayout_c->addWidget(m_ui->btn_recordsentence, 0);
    hlayout_c->addWidget(m_ui->btn_cancel, 0);

    QVBoxLayout *vlayout_a = new QVBoxLayout(m_ui->dialog);
    vlayout_a->addLayout(hlayout_a, 0);
    vlayout_a->addLayout(hlayout_b, 0);
    vlayout_a->addWidget(m_ui->ledit_sentence_a, 0);
    vlayout_a->addWidget(m_ui->ledit_sentence_b, 0);
    vlayout_a->addLayout(hlayout_c, 0);
}

void DialogEditSentence::Open()
{
    m_ui->dialog->open();
}

void DialogEditSentence::Close()
{
    m_ui->dialog->close();
}

void DialogEditSentence::Add_Pattern_Btn_Slot()
{

}

void DialogEditSentence::Add_Tense_Btn_Slot()
{

}

void DialogEditSentence::RecordSentence_Btn_Slot()
{
    QStringList text;
    text << m_ui->ledit_sentence_a->text();
    text << m_ui->ledit_sentence_b->text();
    Sentence stc(text);
    stc.Record(SETS[DialogSet::KEY_SENTENCE_FILE].toString());
}

