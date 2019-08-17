#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H

#include <QTextStream>
#include <QTreeWidget>
#include <QVector>

class Item
{
public:
    Item(Item *parent, QStringList *lines);
    ~Item() = default;
    inline const QString &Level() { return m_level; }
    inline const QString &Content() { return m_content; }
    inline Item *Parent() { return m_parent; }
    inline bool IsMyChild(const QString &level) { return level.startsWith(m_level); }
    inline bool IsMyParent(const QString &level) { return m_level.startsWith(level); }
    inline void AddChild(Item *item) { m_children.append(item); }
    void Unfold(QTreeWidget *wdg, QTreeWidgetItem * wdgItem);

private:
    QString GetLevel(const QString &line);

private:
    Item *m_parent;
    QString m_level;
    QString m_content;
    QVector<Item *> m_children;
};

class Classification
{
public:
    ~Classification() = default;
    static Classification *&Instance();

protected:
    Classification();

public slots:
    void Open();
    void Close();

private:
    static Classification *m_instance;
    QTreeWidget *m_treeWdg;
    Item *m_rootItem;
};

#define CLASSIFICATION

#endif // CLASSIFICATION_H
