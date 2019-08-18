#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H

#include <QTextStream>
#include <QTreeWidget>
#include <QVector>
#include <QPushButton>
#include <QDialog>
#include <QObject>

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


class Classification : public QObject
{
    Q_OBJECT

public:
    ~Classification();
    static Classification *&Instance();
    inline QTreeWidget *&TreeWidget() { return m_treeWdg; }
    QString CurrentContent();
    inline void SetFlag(int flag) { m_flag = flag; }
    inline int Flag() { return m_flag; }

    Classification();

signals:
    void Ok_Signal();

public slots:
    void Open();
    void Close();
    void Ok_Btn_Slot();

private:
    void Init();
    void Layout();

private:
    static Classification *m_instance;
    QTreeWidget *m_treeWdg;
    Item *m_rootItem;
    QDialog * m_dialog;
    QPushButton *m_btn_ok;
    QPushButton *m_btn_cancel;
    int m_flag;
};

#define CLASSIFICATION   Classification::Instance()

#endif // CLASSIFICATION_H
