#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Search.h"
#include "Word.h"
#include "DialogSet.h"
#include "DialogEditWord.h"
#include "DialogExport.h"
#include "TextHighLighter.h"
#include <QMainWindow>

struct ui;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void Layout();
    void SetupMenu();
    Word* GetWordFromSearchResults();

private slots:
    void Search_Btn_Slot();
    void Edit_Btn_Slot();
    void MenuBarTrigger_Slot(QAction *act);
    void Record_Word_Edited_Slot(Word wd);
    void Play_Btn_Slot();

private:
    struct ui *m_ui;
    DialogSet *m_settings;
    DialogEditWord *m_diaglogeditword;
    DialogExport *m_dialogexport;
    QList<SearchResult *> m_results;
    TextHighLighter *m_highlighter;
};

#endif // MAINWINDOW_H
