#ifndef QLISTWIDGETCM_H
#define QLISTWIDGETCM_H

#include <QListWidget>

struct ui;

class QListWidgetCm : public QListWidget
{
    Q_OBJECT

public:
    QListWidgetCm(QWidget *parent = Q_NULLPTR);
    ~QListWidgetCm();

    QStringList TextItems();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void ContextMenuAction_Slot(bool checked);

private:
    struct ui *m_ui;

    QListWidgetItem * m_cursor_focuson_item;
};

#endif // QLISTWIDGETCM_H
