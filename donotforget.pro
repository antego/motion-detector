QT       += core gui opengl testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = donotforget
TEMPLATE = app


SOURCES += main.cpp\
            mainwindow.cpp \
    mogcapturer.cpp

HEADERS += mainwindow.h \
    mogcapturer.h

FORMS += mainwindow.ui

RESOURCES = appresource.qrc

INCLUDEPATH += C:\Users\anton\opencv2.4.11\opencv\build\include
LIBS += -LC:\Users\anton\opencv2.4.11\opencv\build\x64\vc12\lib
LIBS += -lopencv_core2411d -lopencv_highgui2411d -lopencv_imgproc2411d -lopencv_video2411d
