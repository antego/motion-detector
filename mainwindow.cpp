#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDir>
#include <QFormLayout>

#include <opencv2/highgui/highgui.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    cameraEnabled(false),
    closeRequested(false)
{
    ui.setupUi(this);

    connect(ui.button, SIGNAL (released()), this, SLOT (handleCamButton()));
}

void MainWindow::handleCamButton()
{
    if (!cameraEnabled)
    {
        startCamera();
    }
    else
    {
        stopCamera();
    }
}

void MainWindow::startCamera()
{
    mogCapturer = new MogCapturer(ui.camSpin->value(),
                                  ui.widthSpin->value(),
                                  ui.heightSpin->value(),
                                  ui.showBg->checkState() == Qt::Checked);
    capThread = new QThread();

    mogCapturer->moveToThread(capThread);
    connect(capThread, SIGNAL (started()), mogCapturer, SLOT (process()));
    connect(mogCapturer, SIGNAL (error(QString)), this, SLOT (handleError(QString)));
    connect(mogCapturer, SIGNAL (cameraStopped()), this, SLOT (confirmCameraStop()));
    connect(mogCapturer, SIGNAL (shotTaken()), this, SLOT (ackShot()));
    connect(mogCapturer, SIGNAL (finished()), this, SLOT (closeIfNeeded()));
    connect(mogCapturer, SIGNAL (finished()), capThread, SLOT (quit()));
    connect(mogCapturer, SIGNAL (finished()), mogCapturer, SLOT (deleteLater()));
    connect(capThread, SIGNAL (finished()), capThread, SLOT (deleteLater()));
    capThread->start();
    ui.button->setText("Stop camera");
    cameraEnabled = true;
}

void MainWindow::stopCamera()
{
    capThread->requestInterruption();
}

void MainWindow::confirmCameraStop()
{
    ui.button->setText("Start camera");
    cameraEnabled = false;
}

void MainWindow::closeIfNeeded()
{
    if (closeRequested)
        close();
}

void MainWindow::ackShot()
{
    ui.statusBar->showMessage("Picture saved", 1000);
}

void MainWindow::handleError(QString errorCaption)
{
    QMessageBox::information(this, "Info", errorCaption, QMessageBox::Ok);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (cameraEnabled)
    {
        closeRequested = true;
        stopCamera();
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

MainWindow::~MainWindow()
{
}
