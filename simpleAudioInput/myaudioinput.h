#ifndef MYAUDIOINPUT_H
#define MYAUDIOINPUT_H

#include <QObject>
#include <QAudioInput>
#include <QVector>
#include <QByteArray>
#include <QDataStream>
#include <QTimer>
#include <QAudioDeviceInfo>
#include <QDebug>
#include <QIODevice>
#include <QFile>
#include <QBuffer>
#include <QtEndian>

class myInputIODevice : public QIODevice
{
public:


    myInputIODevice (const QAudioFormat &format, QObject *parent);
    ~myInputIODevice();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
  //  QVector<float> myAudioData;
    const QAudioFormat myAudioFormat;


private:
    quint32 myMaxAmplitude;
    QVector<float> myAudioData;


};

class myAudioInput : public QObject
{
    Q_OBJECT
public:
    explicit myAudioInput(QObject *parent = 0);

signals:

public slots:
    void stopRecording();
    void handleStateChanged(QAudio::State);
    void readData();
    void notified();
private:
    QAudioInput *audio;
    QIODevice *myIODevice;
  //  QBuffer *myBuffer;
    QByteArray *myByteArray;
    QDataStream *myDataStream;
    QVector<float> *myVector;
    QFile myFile;
    myInputIODevice *myDevice;
};


#endif // MYAUDIOINPUT_H
