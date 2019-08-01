#include "QListWidgetCm.h"

#include <QMenu>
#include <QAction>
#include <QListWidgetItem>
#include <QDebug>

struct ui {
    QMenu *contextmenu;

    QAction *act_add;
    QAction *act_edit;
    QAction *act_delete;
};

QListWidgetCm::QListWidgetCm(QWidget *parent) :
    QListWidget(parent),
    m_cursor_focuson_item(nullptr)
{
    m_ui = new struct ui;

    m_ui->contextmenu = new QMenu(this);
    m_ui->act_add = m_ui->contextmenu->addAction(tr("Add"));
    m_ui->act_edit = m_ui->contextmenu->addAction(tr("Edit"));
    m_ui->act_delete = m_ui->contextmenu->addAction(tr("Delete"));

    this->setContextMenuPolicy(Qt::DefaultContextMenu);

    connect(m_ui->act_add, SIGNAL(triggered(bool)), this, SLOT(ContextMenuAction_Slot(bool)));
    connect(m_ui->act_edit, SIGNAL(triggered(bool)), this, SLOT(ContextMenuAction_Slot(bool)));
    connect(m_ui->act_delete, SIGNAL(triggered(bool)), this, SLOT(ContextMenuAction_Slot(bool)));
}

QListWidgetCm::~QListWidgetCm()
{

}

QStringList QListWidgetCm::TextItems()
{
    QStringList lines;
    for(int i = 0; i < this->count(); i++)
    {
        lines << this->item(i)->text();
    }

    return lines;
}

void QListWidgetCm::contextMenuEvent(QContextMenuEvent *event)
{
    m_cursor_focuson_item = this->itemAt(this->mapFromGlobal(QCursor::pos()));

    if(nullptr == m_cursor_focuson_item)
    {
        m_ui->act_add->setEnabled(true);
        m_ui->act_edit->setEnabled(false);
        m_ui->act_delete->setEnabled(false);
    }
    else
    {
        m_ui->act_add->setEnabled(false);
        m_ui->act_edit->setEnabled(true);
        m_ui->act_delete->setEnabled(true);
    }

    m_ui->contextmenu->exec(QCursor::pos());
}

void QListWidgetCm::ContextMenuAction_Slot(bool checked)
{
    QAction *act = qobject_cast<QAction *>(this->sender());

    if(m_ui->act_add == act)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setFlags(item->flags() | Qt::ItemIsEditable);

        this->addItem(item);
        this->setCurrentItem(item);
    }
    else if(m_ui->act_edit == act && nullptr != m_cursor_focuson_item)
    {
        m_cursor_focuson_item->setFlags(m_cursor_focuson_item->flags() | Qt::ItemIsEditable);
    }
    else if(m_ui->act_delete == act)
    {
        if(nullptr != m_cursor_focuson_item)
        {
            this->removeItemWidget(m_cursor_focuson_item);
            delete m_cursor_focuson_item;
            m_cursor_focuson_item = nullptr;
        }
    }
}
