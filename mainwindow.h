#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

struct ui;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void search_btn_slot();

protected:
    struct ui *m_ui;
};

#endif // MAINWINDOW_H
