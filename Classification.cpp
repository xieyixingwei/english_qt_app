#include "Classification.h"
#include "TextEdit.h"

#include <QRegularExpression>
#include <QFile>
#include <QDebug>

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

Classification::Classification()
{
    m_treeWdg = new QTreeWidget();
    m_treeWdg->setColumnCount(1);

    TextEdit text("classification.txt");
    m_rootItem = new Item(nullptr, &text.Buf());
    m_rootItem->Unfold(m_treeWdg, nullptr);
}

void Classification::Open()
{
    m_treeWdg->show();
}

void Classification::Close()
{
    m_treeWdg->hide();
}
