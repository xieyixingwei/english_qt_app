#ifndef DIALOGSET_H
#define DIALOGSET_H

#include <QObject>

struct ui;

class DialogSet : public QObject
{
    Q_OBJECT
public:
    DialogSet();
    ~DialogSet();

    void Open();
    void Close();

private slots:
    void Ok_Btn_Slot();

private:
    void Init();
    void Layout();

private:
    struct ui *m_ui;
};


#endif // DIALOGSET_H
