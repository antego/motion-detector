#include "mogcapturer.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <QThread>
#include <stdexcept>

MogCapturer::MogCapturer(int camId, int frameWidth, int frameHeight, bool showBg) :
    camId(camId),
    frameWidth(frameWidth),
    frameHeight(frameHeight),
    showBg(showBg)
{

}

void MogCapturer::init(int history, int nmixtures, double backroundRatio, double noiseSigma)
{
    subtractor = cv::BackgroundSubtractorMOG(history, nmixtures, backroundRatio, noiseSigma);
}

MogCapturer::~MogCapturer()
{

}

void MogCapturer::process()
{
    try
    {
        cv::VideoCapture camera(camId);
        if (!camera.isOpened())
            throw QString("Can't open camera");
        camera.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
        camera.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);

        const std::string maskWindow = "Mask";
        const std::string bgWindow = "Background";
        cv::namedWindow(maskWindow);
        cv::namedWindow(bgWindow);
        while (!QThread::currentThread()->isInterruptionRequested())
        {
            cv::Mat frame;
            bool read = camera.read(frame);
            if (!read)
                continue;
            cv::Mat fore;
            cv::Mat bg;
            subtractor(frame, fore);
            cv::imshow(maskWindow, fore);

            if (showBg)
            {
                subtractor.getBackgroundImage(bg);
                cv::imshow(bgWindow, bg);
            }
            cv::waitKey(1);
        }
        cv::destroyWindow(maskWindow);
        cv::destroyWindow(bgWindow);
        camera.release();
    }
    catch (const QString& errorStr)
    {
        emit error(errorStr);
    }
    emit cameraStopped();
    emit finished();
}
