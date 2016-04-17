#ifndef MOGCAPTURER_H
#define MOGCAPTURER_H

#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>
#include <QObject>

class MogCapturer : public QObject
{
    Q_OBJECT

public:
    MogCapturer(int camId, int frameWidth, int frameHeight, bool showBg);
    ~MogCapturer();
    void init(int history, int nmixtures, double backroundRatio, double noiseSigma);

public slots:
    void process();

signals:
    void finished();
    void cameraStopped();
    void error(QString caption);
    void shotTaken();

private:
    const int camId, frameWidth, frameHeight;
    QString pictureName;
    bool showBg;
    cv::BackgroundSubtractor subtractor;
};

#endif // MOGCAPTURER_H
