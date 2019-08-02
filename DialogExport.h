#ifndef DIALOGEXPORT_H
#define DIALOGEXPORT_H

#include <QObject>

struct ui;

class DialogExport : public QObject
{
    Q_OBJECT

public:
    static const QString GROUP_EXPORT;
    static const QString KEY_EXPORT_FILE_ON_HOT;
    static const QString KEY_EXPORT_FILE_ON_TIMESTAMP;
    static const QString KEY_EXPORT_FILE_ON_TIMESTAMP_SCOPE;

public:
    DialogExport();
    ~DialogExport() { }

    void Open();
    void Close();

private:
    void Init();
    void Layout();

private slots:
    void Export_Btn_Slot();

private:
    struct ui *m_ui;
};

#endif // DIALOGEXPORT_H
