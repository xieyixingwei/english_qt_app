#ifndef DIALOGEDITWORD_H
#define DIALOGEDITWORD_H

#include "Word.h"

#include <QObject>

struct ui;

class DialogEditWord : public QObject
{
    Q_OBJECT

public:
    DialogEditWord();
    ~DialogEditWord();

    void SetWord(Word &wd);

Q_SIGNALS:
    void Apply_Signal(Word wd);

public slots:
    void Open();
    void Close();

private slots:
    void Add_Btn_Slot();
    void Add_Example_Btn_Slot();
    void RecordWord_Btn_Slot();
    void Ok_Btn_Slot();
    void Search_Btn_Slot();

private:
    void Init();
    void Layout();
    void DisplayWord();
    void SnapshotWord();
    QStringList ToInterpretationString();

private:
    struct ui *m_ui;
    Word *m_wd;
};

#endif // DialogEditWord_H
