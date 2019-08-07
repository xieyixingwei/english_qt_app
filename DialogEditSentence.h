#ifndef DIALOGEDITSENTENCE_H
#define DIALOGEDITSENTENCE_H

#include "Sentence.h"
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
    void Add_Btn_Slot();
    void RecordSentence_Btn_Slot();
    void Clear_Btn_Slot();

private:
    void Init();
    void Layout();

private:
    struct ui *m_ui;
    Sentence *m_sentence;
};

#endif // DIALOGEDITSENTENCE_H
