#include "DialogExport.h"
#include "Search.h"
#include "Setting.h"
#include "TextEdit.h"
#include "Word.h"
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
#include <QCheckBox>
#include <QCalendarWidget>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QDir>

struct ui
{
    QDialog *dialog;

    QCheckBox *cbox_tag_export;
    QCheckBox *cbox_hot_export;
    QCheckBox *cbox_timestamp_export;
    QCheckBox *cbox_timestamp_scope_export;

    QLineEdit *ledit_tag_export;
    QSpinBox *spinbox_hot_export;
    QSpinBox *spinbox_timestamp_export;
    QDateTimeEdit *dtedit_timestamp_scope_a_export;
    QDateTimeEdit *dtedit_timestamp_scope_b_export;

    QComboBox *combox_tags;

    QLineEdit *ledit_tag_export_filename;
    QLineEdit *ledit_hot_export_filename;
    QLineEdit *ledit_timestamp_export_filename;
    QLineEdit *ledit_timestamp_scope_export_filename;

    QPushButton *btn_export;
    QPushButton *btn_cancel;
};

DialogExport::DialogExport()
{
    m_ui = new struct ui;

    m_ui->dialog = new QDialog(nullptr, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    m_ui->dialog->setWindowTitle(tr("Export"));

    m_ui->cbox_tag_export = new QCheckBox(tr("On Tag"));
    m_ui->cbox_hot_export = new QCheckBox(tr("On Hot"));
    m_ui->cbox_timestamp_export = new QCheckBox(tr("On Timestamp"));
    m_ui->cbox_timestamp_scope_export = new QCheckBox(tr("On Timestamp Scope"));

    m_ui->ledit_tag_export = new QLineEdit;
    m_ui->spinbox_hot_export = new QSpinBox;
    m_ui->spinbox_timestamp_export = new QSpinBox;

    m_ui->dtedit_timestamp_scope_a_export = new QDateTimeEdit;
    m_ui->dtedit_timestamp_scope_b_export = new QDateTimeEdit;

    m_ui->combox_tags = new QComboBox;

    m_ui->ledit_tag_export_filename = new QLineEdit;
    m_ui->ledit_hot_export_filename = new QLineEdit;
    m_ui->ledit_timestamp_export_filename = new QLineEdit;
    m_ui->ledit_timestamp_scope_export_filename = new QLineEdit;

    m_ui->btn_export = new QPushButton(tr("Export"));
    m_ui->btn_cancel = new QPushButton(tr("Cancel"));

    connect(m_ui->btn_export, SIGNAL(clicked()), this, SLOT(Export_Btn_Slot()));
    connect(m_ui->btn_cancel, SIGNAL(clicked()), m_ui->dialog, SLOT(close()));

    Init();
    Layout();
}

void DialogExport::Layout()
{
    QHBoxLayout *hlayout_a = new QHBoxLayout;
    hlayout_a->addWidget(m_ui->cbox_tag_export, 0);
    hlayout_a->addWidget(m_ui->ledit_tag_export, 0);
    hlayout_a->addWidget(m_ui->combox_tags, 0);
    hlayout_a->addWidget(m_ui->ledit_tag_export_filename, 0);

    QHBoxLayout *hlayout_c = new QHBoxLayout;
    hlayout_c->addWidget(m_ui->cbox_hot_export, 0);
    hlayout_c->addWidget(m_ui->spinbox_hot_export, 0);
    hlayout_c->addWidget(m_ui->ledit_hot_export_filename, 0);

    QHBoxLayout *hlayout_d = new QHBoxLayout;
    hlayout_d->addWidget(m_ui->cbox_timestamp_export, 0);
    hlayout_d->addWidget(m_ui->spinbox_timestamp_export, 0);
    hlayout_d->addWidget(m_ui->ledit_timestamp_export_filename, 0);

    QHBoxLayout *hlayout_e = new QHBoxLayout;
    hlayout_e->addWidget(m_ui->cbox_timestamp_scope_export, 0);
    hlayout_e->addWidget(m_ui->dtedit_timestamp_scope_a_export, 0);
    hlayout_e->addWidget(m_ui->dtedit_timestamp_scope_b_export, 0);
    hlayout_e->addWidget(m_ui->ledit_timestamp_scope_export_filename, 0);

    QHBoxLayout *hlayout_f = new QHBoxLayout;
    hlayout_f->addWidget(m_ui->btn_export, 0);
    hlayout_f->addWidget(m_ui->btn_cancel, 0);

    QVBoxLayout *vlayout = new QVBoxLayout(m_ui->dialog);
    vlayout->addLayout(hlayout_a, 0);
    vlayout->addLayout(hlayout_c, 0);
    vlayout->addLayout(hlayout_d, 0);
    vlayout->addLayout(hlayout_e, 0);
    vlayout->addLayout(hlayout_f, 0);
}

void DialogExport::Init()
{
    m_ui->combox_tags->addItems(SETS[KEY_SENTENCE_PATTERN].toString().split(","));
    m_ui->combox_tags->addItems(SETS[KEY_SENTENCE_TENSE].toString().split(","));
    m_ui->combox_tags->addItems(SETS[KEY_WORD_TAGS].toString().split(","));

    m_ui->spinbox_hot_export->setRange(1, 100000);
    m_ui->spinbox_hot_export->setValue(50);

    m_ui->spinbox_timestamp_export->setRange(1, 100000);
    m_ui->spinbox_timestamp_export->setValue(50);

    m_ui->dtedit_timestamp_scope_a_export->setDateTime(QDateTime::currentDateTime());
    m_ui->dtedit_timestamp_scope_b_export->setDateTime(QDateTime::currentDateTime());

    QDir curdir = QDir::current();
    curdir.cdUp();

    m_ui->ledit_hot_export_filename->setText(SETS[KEY_EXPORT_FILE_ON_HOT].toString());
    m_ui->ledit_timestamp_export_filename->setText(SETS[KEY_EXPORT_FILE_ON_TIMESTAMP].toString());
    m_ui->ledit_timestamp_scope_export_filename->setText(SETS[KEY_EXPORT_FILE_ON_TIMESTAMP_SCOPE].toString());
}

void DialogExport::Open()
{
    m_ui->dialog->open();
}

void DialogExport::Close()
{
    m_ui->dialog->close();
}

void DialogExport::Export_Btn_Slot()
{
    Search search;

    if(m_ui->cbox_tag_export->isChecked())
    {

    }

    if(m_ui->cbox_hot_export->isChecked())
    {
        search.FilterWordsAccordingHot(search.FindPathFileFromFilter(Settings::ToStringList(SETS.GetGroup(GROUP_SEARCH_FILES))),
                    m_ui->ledit_hot_export_filename->text().trimmed(),
                    m_ui->spinbox_hot_export->value());
    }

    if(m_ui->cbox_timestamp_export->isChecked())
    {
        search.FilterWordsAccordingTimeStamp(search.FindPathFileFromFilter(Settings::ToStringList(SETS.GetGroup(GROUP_SEARCH_FILES))),
                    m_ui->ledit_timestamp_export_filename->text().trimmed(),
                    m_ui->spinbox_timestamp_export->value());
    }

    if(m_ui->cbox_timestamp_scope_export->isChecked())
    {
        search.FilterWordsAccordingTimeStamp(search.FindPathFileFromFilter(Settings::ToStringList(SETS.GetGroup(GROUP_SEARCH_FILES))),
                    m_ui->ledit_timestamp_scope_export_filename->text().trimmed(),
                    m_ui->dtedit_timestamp_scope_a_export->dateTime(),
                    m_ui->dtedit_timestamp_scope_b_export->dateTime());
    }

    SETS.SetValue(KEY_EXPORT_FILE_ON_HOT, m_ui->ledit_hot_export_filename->text().trimmed());
    SETS.SetValue(KEY_EXPORT_FILE_ON_TIMESTAMP, m_ui->ledit_timestamp_export_filename->text().trimmed());
    SETS.SetValue(KEY_EXPORT_FILE_ON_TIMESTAMP_SCOPE, m_ui->ledit_timestamp_scope_export_filename->text().trimmed());
    SETS.Sync();
}
