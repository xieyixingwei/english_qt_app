#ifndef DIALOGSET_H
#define DIALOGSET_H

#include <QObject>

struct ui;

class DialogSet : public QObject
{
    Q_OBJECT

public:
    static const QString GROUP_SETTING;
    static const QString KEY_UPDATE_HOT;
    static const QString KEY_UPDATE_TIMESTAMP;
    static const QString KEY_AUTO_ADD_WORD;
    static const QString KEY_UNNOTE_WORD_FILE;
    static const QString KEY_SENTENCE_FILE;
    static const QString KEY_SOUND_DIR;
    static const QString KEY_SOUND_VOLUME;

    static const QString GROUP_SEARCH_PATH_FILE;

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
