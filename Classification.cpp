#include "Classification.h"
#include "TextEdit.h"

#include <QRegularExpression>
#include <QFile>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

Item::Item(Item *parent, QStringList *lines) :
    m_parent(parent)
{
    if(lines->isEmpty())
    {
        return;
    }

    m_level = GetLevel(lines->first());
    if(m_level.isEmpty())
    {
        return;
    }

    m_content = lines->takeFirst();

    if(lines->isEmpty())
    {
        return;
    }

    QString nextLevel = GetLevel(lines->first());
    if(nextLevel.isEmpty())
    {
        return;
    }

    Item *pItem = this;
    while(pItem != nullptr)
    {
        if(pItem->IsMyChild(nextLevel) ||
                pItem->Parent() == nullptr)
        {
            pItem->AddChild(new Item(pItem, lines));
            break;
        }

        pItem = pItem->Parent();
    }
}

QString Item::GetLevel(const QString &line)
{
    QRegularExpression rex(QString("(?P<level>[0-9. ]+)"));
    QRegularExpressionMatch matched = rex.match(line);
    if(matched.hasMatch())
    {
        return matched.captured("level").trimmed().remove(" ");
    }
    return "";
}

void Item::Unfold(QTreeWidget *wdg, QTreeWidgetItem * parent)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(wdg);
    treeItem->setText(0, m_content);

    if(parent != nullptr)
    {
        parent->addChild(treeItem);
    }

    for(int i = m_children.count() - 1; i >= 0; i--)
    {
        m_children.at(i)->Unfold(nullptr, treeItem);
    }
}

Classification *Classification::m_instance = nullptr;

Classification *&Classification::Instance()
{
    if(m_instance == nullptr)
    {
        m_instance = new Classification();
    }

    return m_instance;
}

Classification::Classification() :
    m_flag(0)
{
    m_dialog = new QDialog(nullptr, Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    m_dialog->setWindowTitle(tr("Select Classification"));

    m_btn_ok = new QPushButton(tr("Ok"));
    m_btn_cancel = new QPushButton(tr("Cancel"));

    m_treeWdg = new QTreeWidget(nullptr);
    m_treeWdg->setWindowTitle(tr("Classification"));
    m_treeWdg->setColumnCount(1);

    connect(m_btn_ok, SIGNAL(clicked()), this, SLOT(Ok_Btn_Slot()));
    connect(m_btn_cancel, SIGNAL(clicked()), this, SLOT(Close()));

    Init();
    Layout();

    m_dialog->close();
}

Classification::~Classification()
{

}

void Classification::Init()
{
    TextEdit text("classification.txt");
    text.RemoveSpaceLines();
    m_rootItem = new Item(nullptr, &text.Buf());
    m_rootItem->Unfold(m_treeWdg, nullptr);
}

void Classification::Layout()
{
    QHBoxLayout *hlayout_a = new QHBoxLayout;
    hlayout_a->addWidget(m_btn_ok, 0);
    hlayout_a->addWidget(m_btn_cancel, 0);

    QVBoxLayout *vlayout_a = new QVBoxLayout(m_dialog);
    vlayout_a->addWidget(m_treeWdg, 0);
    vlayout_a->addLayout(hlayout_a, 0);
}

QString Classification::CurrentContent()
{
    QRegularExpression rex(QString("(?P<pattern>(?<=\\().*(?=\\)))"));
    QRegularExpressionMatch matched = rex.match(m_treeWdg->currentItem()->text(0).trimmed());
    if(matched.hasMatch())
    {
        return matched.captured("pattern").trimmed();
    }
    return "";
}

void Classification::Open()
{
    m_dialog->open();
}

void Classification::Close()
{
    m_dialog->close();
}

void Classification::Ok_Btn_Slot()
{
    emit Ok_Signal();
}
