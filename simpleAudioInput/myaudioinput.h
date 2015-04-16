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
private:
    QAudioInput *audio;
    QIODevice *myIODevice;
    QBuffer *myBuffer;
    QByteArray *myByteArray;
    QDataStream *myDataStream;
    QVector<float> *myVector;
    QFile myFile;
};

class myInputIODevice : public QIODevice
{
public:


    myInputIODevice (const QAudioFormat &format, QObject *parent);
    ~myInputIODevice();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    QVector myAudioData;


private:
    quint32 maxAmplitude;
    QVector myAudioData;

    const QAudioFormat myAudioFormat;

};

#endif // MYAUDIOINPUT_H
