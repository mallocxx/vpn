#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QProcess>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleVPN(const QString &vpnName);

private:
    QPushButton *btnToggleVPN_UK;
    QPushButton *btnToggleVPN_FR;
    QPushButton *btnToggleVPN_JP;
    QProcess *process;

    bool vpnUKEnabled;
    bool vpnFREnabled;
    bool vpnJPEnabled;
};

#endif
