#include "DialogExport.h"

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

struct ui
{
    QDialog *dialog;

    QCheckBox *cbox_tag_export;
    QCheckBox *cbox_sort_export;
    QCheckBox *cbox_hot_export;
    QCheckBox *cbox_timestamp_export;
    QCheckBox *cbox_timestamp_scope_export;

    QLineEdit *ledit_tag_export;
    QLineEdit *ledit_sort_export;
    QSpinBox *spinbox_hot_export;
    QSpinBox *spinbox_timestamp_export;
    QDateTimeEdit *dtedit_timestamp_scope_a_export;
    QDateTimeEdit *dtedit_timestamp_scope_b_export;

    QComboBox *combox_tags;
    QComboBox *combox_sorts;

    QLineEdit *ledit_tag_export_filename;
    QLineEdit *ledit_sort_export_filename;
    QLineEdit *ledit_hot_export_filename;
    QLineEdit *ledit_timestamp_export_filename;
    QLineEdit *ledit_timestamp_scope_export_filename;

    QPushButton *btn_export;
    QPushButton *btn_cancel;
};

DialogExport::DialogExport()
{
    m_ui = new struct ui;

    m_ui->dialog = new QDialog(nullptr, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    m_ui->dialog->setWindowTitle(tr("Export"));

    m_ui->cbox_tag_export = new QCheckBox(tr("On Tag"));
    m_ui->cbox_sort_export = new QCheckBox(tr("On Sort"));
    m_ui->cbox_hot_export = new QCheckBox(tr("On Hot"));
    m_ui->cbox_timestamp_export = new QCheckBox(tr("On Timestamp"));
    m_ui->cbox_timestamp_scope_export = new QCheckBox(tr("On Timestamp Scope"));

    m_ui->ledit_tag_export = new QLineEdit;
    m_ui->ledit_sort_export = new QLineEdit;
    m_ui->spinbox_hot_export = new QSpinBox;
    m_ui->spinbox_timestamp_export = new QSpinBox;

    m_ui->dtedit_timestamp_scope_a_export = new QDateTimeEdit;
    m_ui->dtedit_timestamp_scope_b_export = new QDateTimeEdit;

    m_ui->combox_tags = new QComboBox;
    m_ui->combox_sorts = new QComboBox;

    m_ui->ledit_tag_export_filename = new QLineEdit;
    m_ui->ledit_sort_export_filename = new QLineEdit;
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

    QHBoxLayout *hlayout_b = new QHBoxLayout;
    hlayout_b->addWidget(m_ui->cbox_sort_export, 0);
    hlayout_b->addWidget(m_ui->ledit_sort_export, 0);
    hlayout_b->addWidget(m_ui->combox_sorts, 0);
    hlayout_b->addWidget(m_ui->ledit_sort_export_filename, 0);

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
    vlayout->addLayout(hlayout_b, 0);
    vlayout->addLayout(hlayout_c, 0);
    vlayout->addLayout(hlayout_d, 0);
    vlayout->addLayout(hlayout_e, 0);
    vlayout->addLayout(hlayout_f, 0);
}

void DialogExport::Init()
{
    m_ui->spinbox_hot_export->setRange(1, 100000);
    m_ui->spinbox_hot_export->setValue(50);

    m_ui->spinbox_timestamp_export->setRange(1, 100000);
    m_ui->spinbox_timestamp_export->setValue(50);

    m_ui->dtedit_timestamp_scope_a_export->setDateTime(QDateTime::currentDateTime());
    m_ui->dtedit_timestamp_scope_b_export->setDateTime(QDateTime::currentDateTime());
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
    if(m_ui->cbox_tag_export->isChecked())
    {

    }

    if(m_ui->cbox_sort_export->isChecked())
    {

    }

    if(m_ui->cbox_hot_export->isChecked())
    {

    }

    if(m_ui->cbox_timestamp_export->isChecked())
    {

    }

}
