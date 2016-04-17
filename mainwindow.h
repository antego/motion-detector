#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mogcapturer.h"

#include <QMainWindow>
#include <QDir>
#include <ui_mainwindow.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void confirmCameraStop();
    void handleError(QString errorCaption);

private slots:
    void handleCamButton();
    void closeIfNeeded();
    void ackShot();

private:
    Ui_MainWindow ui;
    MogCapturer* mogCapturer;
    QThread* capThread;
    bool cameraEnabled;
    bool closeRequested;
    void startCamera();
    void stopCamera();
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
