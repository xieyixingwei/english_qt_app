
#include "DialogSet.h"
#include "Setting.h"
#include "QListWidgetCm.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QList>
#include <QString>
#include <QRadioButton>
#include <QDir>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QDialog>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>

const QString DialogSet::GROUP_SETTING = "sets";
const QString DialogSet::KEY_UPDATE_HOT = GROUP_SETTING + "/Update_Hot";
const QString DialogSet::KEY_UPDATE_TIMESTAMP = GROUP_SETTING + "/Update_Timestamp";
const QString DialogSet::KEY_AUTO_ADD_WORD = GROUP_SETTING + "/Auto_Add_Word" ;
const QString DialogSet::KEY_UNNOTE_WORD_FILE = GROUP_SETTING + "/Unnote_Word_Trace_File";
const QString DialogSet::KEY_SENTENCE_FILE = GROUP_SETTING + "/Sentence_File";
const QString DialogSet::KEY_SOUND_DIR = GROUP_SETTING + "/Sound_Directory";
const QString DialogSet::KEY_SOUND_VOLUME = GROUP_SETTING + "/Sound_Volume";
const QString DialogSet::GROUP_SEARCH_PATH_FILE = "searchfiles";

struct ui
{
    QDialog *dialog;
    QCheckBox *cbox_updatehot;
    QCheckBox *cbox_updatetimestamp;
    QCheckBox *cbox_autoaddword;
    QLineEdit *ledit_autoaddwordfile;
    QLineEdit *ledit_sentencefile;
    QLineEdit *ledit_sounddir;

    QSpinBox  *spinbox_volume;

    QListWidgetCm *lwdg_searchfiles;

    QPushButton *btn_ok;
    QPushButton *btn_cancel;
};

DialogSet::DialogSet()
{
    m_ui = new struct ui;

    m_ui->dialog = new QDialog(nullptr, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    m_ui->dialog->setWindowTitle(tr("Settings"));

    m_ui->cbox_updatehot = new QCheckBox(tr("Update Hot"));
    m_ui->cbox_updatetimestamp = new QCheckBox(tr("Update Timestamp"));

    m_ui->cbox_autoaddword = new QCheckBox(tr("Auto Add Word"));
    m_ui->ledit_autoaddwordfile = new QLineEdit;
    m_ui->ledit_sentencefile = new QLineEdit;
    m_ui->ledit_sounddir = new QLineEdit;
    m_ui->spinbox_volume = new QSpinBox;
    m_ui->lwdg_searchfiles = new QListWidgetCm;
    //m_ui->lwdg_searchfiles->addItems(SETS.GetGroupAllValue(GROUP_SEARCH_PATH_FILE));

    m_ui->btn_ok = new QPushButton(tr("Ok"));
    m_ui->btn_cancel = new QPushButton(tr("Cancel"));

    connect(m_ui->btn_ok, SIGNAL(clicked()), this, SLOT(Ok_Btn_Slot()));
    connect(m_ui->btn_cancel, SIGNAL(clicked()), m_ui->dialog, SLOT(close()));

    Init();
    Layout();

    m_ui->dialog->close();
}

DialogSet::~DialogSet()
{

}

void DialogSet::Init()
{
    QDir curdir = QDir::current();
    curdir.cdUp();
/*
    if(SETS[KEY_UNNOTE_WORD_FILE].isNull())
    {
        SETS.SetValue(KEY_UNNOTE_WORD_FILE, curdir.path() + "/thesaurus/words/word-unnote.md");
    }

    if(SETS[KEY_SENTENCE_FILE].isNull())
    {
        SETS.SetValue(KEY_SENTENCE_FILE, curdir.path() + "/thesaurus/sentences/sentences.md");
    }

    if(SETS[KEY_SOUND_DIR].isNull())
    {
        SETS.SetValue(KEY_SOUND_DIR, curdir.path() + "/thesaurus/sounds/");
    }

    if(SETS[KEY_SOUND_VOLUME].isNull())
    {
        SETS.SetValue(KEY_SOUND_VOLUME, 30);
    }

    m_ui->ledit_autoaddwordfile->setText(SETS[KEY_UNNOTE_WORD_FILE].toString());
    m_ui->ledit_sentencefile->setText(SETS[KEY_SENTENCE_FILE].toString());
    m_ui->ledit_sounddir->setText(SETS[KEY_SOUND_DIR].toString());
    m_ui->spinbox_volume->setValue(SETS[KEY_SOUND_VOLUME].toInt());
    m_ui->cbox_updatehot->setCheckState(SETS[KEY_UPDATE_HOT].toBool() ? Qt::Checked : Qt::Unchecked);
    m_ui->cbox_updatetimestamp->setCheckState(SETS[KEY_UPDATE_TIMESTAMP].toBool() ?  Qt::Checked : Qt::Unchecked);
    m_ui->cbox_autoaddword->setCheckState(SETS[KEY_AUTO_ADD_WORD].toBool() ?  Qt::Checked : Qt::Unchecked);
*/
}

void DialogSet::Layout()
{
    QHBoxLayout *thlayout = new QHBoxLayout;
    thlayout->addWidget(m_ui->cbox_updatehot, 0);
    thlayout->addWidget(m_ui->cbox_updatetimestamp, 0);

    QHBoxLayout *bhlayout = new QHBoxLayout;
    bhlayout->addWidget(m_ui->btn_ok, 0);
    bhlayout->addWidget(m_ui->btn_cancel, 0);

    QHBoxLayout *chlayout = new QHBoxLayout;
    chlayout->addWidget(new QLabel(tr("UnNote Word File:")), 0);
    chlayout->addWidget(m_ui->ledit_autoaddwordfile, 0);
    chlayout->addWidget(m_ui->cbox_autoaddword, 0);

    QHBoxLayout *dhlayout = new QHBoxLayout;
    dhlayout->addWidget(new QLabel(tr("Sentence File:")), 0);
    dhlayout->addWidget(m_ui->ledit_sentencefile, 0);

    QHBoxLayout *ehlayout = new QHBoxLayout;
    ehlayout->addWidget(new QLabel(tr("Sound Directory:")), 0);
    ehlayout->addWidget(m_ui->ledit_sounddir, 0);
    ehlayout->addWidget(new QLabel(tr("Volume:")), 0);
    ehlayout->addWidget(m_ui->spinbox_volume, 0);

    QVBoxLayout *vlayout = new QVBoxLayout(m_ui->dialog);
    vlayout->addLayout(thlayout, 0);
    vlayout->addLayout(chlayout, 0);
    vlayout->addLayout(dhlayout, 0);
    vlayout->addLayout(ehlayout, 0);
    vlayout->addWidget(m_ui->lwdg_searchfiles, 0);
    vlayout->addLayout(bhlayout, 0);
}

void DialogSet::Open()
{
    m_ui->dialog->open();
}

void DialogSet::Close()
{
    m_ui->dialog->close();
}

void DialogSet::Ok_Btn_Slot()
{/*
    SETS.SetValue(KEY_UPDATE_HOT, m_ui->cbox_updatehot->isChecked());
    SETS.SetValue(KEY_UPDATE_TIMESTAMP, m_ui->cbox_updatetimestamp->isChecked());
    SETS.SetValue(KEY_AUTO_ADD_WORD, m_ui->cbox_autoaddword->isChecked());
    SETS.SetValue(KEY_UNNOTE_WORD_FILE, m_ui->ledit_autoaddwordfile->text().trimmed());
    SETS.SetValue(KEY_SENTENCE_FILE, m_ui->ledit_sentencefile->text().trimmed());
    SETS.SetValue(KEY_SOUND_DIR, m_ui->ledit_sounddir->text().trimmed());
    SETS.SetValue(KEY_SOUND_VOLUME, m_ui->spinbox_volume->value());

    SETS.RemoveGroupAllValue(GROUP_SEARCH_PATH_FILE);
    for(int i = 0; i < m_ui->lwdg_searchfiles->count(); i++)
    {
        QString pathfile = m_ui->lwdg_searchfiles->item(i)->text().trimmed();
        if(!pathfile.isEmpty())
        {
            SETS.SetValue(GROUP_SEARCH_PATH_FILE + "/" + QString::number(i), pathfile);
        }
    }

    SETS.Sync();
    m_ui->dialog->close();*/
}

