#ifndef DIALOGEDITSENTENCE_H
#define DIALOGEDITSENTENCE_H

#include <QObject>

class DialogEditSentence : public QObject
{
    Q_OBJECT

public:
    DialogEditSentence();
    ~DialogEditSentence();

public slots:
    void Open();
    void Close();

private slots:
    void Add_Pattern_Btn_Slot();
    void Add_Tense_Btn_Slot();
    void RecordSentence_Btn_Slot();

private:
    void Init();
    void Layout();

private:
    struct ui *m_ui;
};

#endif // DIALOGEDITSENTENCE_H
