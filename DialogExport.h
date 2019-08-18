#ifndef DIALOGEXPORT_H
#define DIALOGEXPORT_H

#include <QObject>

struct ui;

class DialogExport : public QObject
{
    Q_OBJECT

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
    void Cliked_RdBtn_Slot();
    void Ok_Slot();

private:
    struct ui *m_ui;
};

#endif // DIALOGEXPORT_H
