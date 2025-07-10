#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    vpnUKEnabled(false), vpnFREnabled(false), vpnJPEnabled(false)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    setMinimumSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(central);

    QLabel *label = new QLabel("VPN", this);
    label->setStyleSheet("font-size: 50px; font-weight: bold; color: #FFFFFF;");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    btnToggleVPN_UK = new QPushButton("United Kingdom", this);
    btnToggleVPN_FR = new QPushButton("France", this);
    btnToggleVPN_JP = new QPushButton("Japan", this);

    btnToggleVPN_UK->setStyleSheet("QPushButton {"
                                   "font-size: 18px;"
                                   "padding: 10px 20px;"
                                   "border-radius: 15px;"
                                   "background-color: #9E9E9E;"
                                   "color: white;"
                                   "border: none;"
                                   "}");

    btnToggleVPN_FR->setStyleSheet("QPushButton {"
                                   "font-size: 18px;"
                                   "padding: 10px 20px;"
                                   "border-radius: 15px;"
                                   "background-color: #9E9E9E;"
                                   "color: white;"
                                   "border: none;"
                                   "}");

    btnToggleVPN_JP->setStyleSheet("QPushButton {"
                                   "font-size: 18px;"
                                   "padding: 10px 20px;"
                                   "border-radius: 15px;"
                                   "background-color: #9E9E9E;"
                                   "color: white;"
                                   "border: none;"
                                   "}");

    layout->addWidget(btnToggleVPN_UK);
    layout->addWidget(btnToggleVPN_FR);
    layout->addWidget(btnToggleVPN_JP);

    process = new QProcess(this);

    connect(btnToggleVPN_UK, &QPushButton::clicked, this, [=]()
            { toggleVPN("uk3"); });
    connect(btnToggleVPN_FR, &QPushButton::clicked, this, [=]()
            { toggleVPN("fr3"); });
    connect(btnToggleVPN_JP, &QPushButton::clicked, this, [=]()
            { toggleVPN("jp3"); });

    connect(process, &QProcess::readyReadStandardOutput, [=]()
            { qDebug() << process->readAllStandardOutput(); });
    connect(process, &QProcess::readyReadStandardError, [=]()
            { qDebug() << process->readAllStandardError(); });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus status)
            {
                if (exitCode == 0)
                {
                    QMessageBox::information(this, "WireGuard", "VPN успешно подключен.");
                }
                else
                {
                    QMessageBox::critical(this, "WireGuard", "Ошибка при подключении VPN.");

                    // В случае ошибки, меняем кнопку обратно на серый цвет и сбрасываем состояние
                    if (vpnUKEnabled)
                    {
                        vpnUKEnabled = false;
                        btnToggleVPN_UK->setStyleSheet("QPushButton {"
                                                       "font-size: 18px;"
                                                       "padding: 10px 20px;"
                                                       "border-radius: 15px;"
                                                       "background-color: #9E9E9E;" // Серый
                                                       "color: white;"
                                                       "border: none;"
                                                       "}");
                    }
                    if (vpnFREnabled)
                    {
                        vpnFREnabled = false;
                        btnToggleVPN_FR->setStyleSheet("QPushButton {"
                                                       "font-size: 18px;"
                                                       "padding: 10px 20px;"
                                                       "border-radius: 15px;"
                                                       "background-color: #9E9E9E;" // Серый
                                                       "color: white;"
                                                       "border: none;"
                                                       "}");
                    }
                    if (vpnJPEnabled)
                    {
                        vpnJPEnabled = false;
                        btnToggleVPN_JP->setStyleSheet("QPushButton {"
                                                       "font-size: 18px;"
                                                       "padding: 10px 20px;"
                                                       "border-radius: 15px;"
                                                       "background-color: #9E9E9E;" // Серый
                                                       "color: white;"
                                                       "border: none;"
                                                       "}");
                    }
                }
            });
}

MainWindow::~MainWindow() {}

void MainWindow::toggleVPN(const QString &vpnName)
{
    QString program = "sudo";
    QStringList arguments;

    QString offStyle = "QPushButton {"
                       "font-size: 18px;"
                       "padding: 10px 20px;"
                       "border-radius: 15px;"
                       "background-color: #9E9E9E;" // Серый
                       "color: white;"
                       "border: none;"
                       "}";

    QString onStyle = "QPushButton {"
                      "font-size: 18px;"
                      "padding: 10px 20px;"
                      "border-radius: 15px;"
                      "background-color: #2196F3;" // Синий
                      "color: white;"
                      "border: none;"
                      "}";

    if (vpnName == "uk3" && !vpnFREnabled && !vpnJPEnabled)
    {
        if (vpnUKEnabled)
        {
            arguments << "wg-quick" << "down" << "wireguard-uk3";
            vpnUKEnabled = false;
            btnToggleVPN_UK->setText("United Kingdom");
            btnToggleVPN_UK->setStyleSheet(offStyle);
        }
        else
        {
            arguments << "wg-quick" << "up" << "wireguard-uk3";
            vpnUKEnabled = true;
            btnToggleVPN_UK->setText("United Kingdom");
            btnToggleVPN_UK->setStyleSheet(onStyle);
        }
    }
    else if (vpnName == "fr3" && !vpnUKEnabled && !vpnJPEnabled)
    {
        if (vpnFREnabled)
        {
            arguments << "wg-quick" << "down" << "wireguard-fr3";
            vpnFREnabled = false;
            btnToggleVPN_FR->setText("France");
            btnToggleVPN_FR->setStyleSheet(offStyle);
        }
        else
        {
            arguments << "wg-quick" << "up" << "wireguard-fr3";
            vpnFREnabled = true;
            btnToggleVPN_FR->setText("France");
            btnToggleVPN_FR->setStyleSheet(onStyle);
        }
    }
    else if (vpnName == "jp3" && !vpnUKEnabled && !vpnFREnabled)
    {
        if (vpnJPEnabled)
        {
            arguments << "wg-quick" << "down" << "wireguard-jp3";
            vpnJPEnabled = false;
            btnToggleVPN_JP->setText("Japan");
            btnToggleVPN_JP->setStyleSheet(offStyle);
        }
        else
        {
            arguments << "wg-quick" << "up" << "wireguard-jp3";
            vpnJPEnabled = true;
            btnToggleVPN_JP->setText("Japan");
            btnToggleVPN_JP->setStyleSheet(onStyle);
        }
    }

    process->start(program, arguments);
}
