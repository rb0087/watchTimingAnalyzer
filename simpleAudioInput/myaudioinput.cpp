#include "myaudioinput.h"
myAudioInput::myAudioInput(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Creating object.";
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::Float);
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    qDebug() << "Just created instance of qaudiodeviceinfo.";
    qDebug() << "Defualt is " << info.deviceName();
    if(!info.isFormatSupported(format)){
        qDebug() << "Format unsupported! Trying to use nearest.";
        format = info.nearestFormat(format);
        qDebug() << "Format set to sample at " << format.sampleRate() ;
    }
    else
        qDebug() << "Format supported!\n";
    audio = new QAudioInput(format,this);
    audio->setBufferSize(1024);
    connect(audio,SIGNAL(stateChanged(QAudio::State)),this,SLOT(handleStateChanged(QAudio::State)));
    //myIODevice = new QIODevice(this);
   // myIODevice->open(QIODevice::ReadWrite);
    myByteArray = new QByteArray();
    myByteArray->resize(44100*4);
    myByteArray->fill('\0');
    myBuffer = new QBuffer(myByteArray);
    myBuffer->open(QIODevice::ReadWrite);
   // connect(audio,SIGNAL(notify()),this,SLOT(readData()));
    audio->setNotifyInterval(500);
    audio->start(&myDevice);
    QTimer::singleShot(1000,this,SLOT(stopRecording()));
}

void myAudioInput::readData()
{
    qDebug()<<"In read data function.";
    myBuffer->waitForReadyRead(4000);
    char* myData = (char*)malloc(sizeof(float)*44100);
    myBuffer->peek(myData,44100);
    for(int ii = 0; ii<1;ii++)
        printf("%f    \n",*((float*)(myData+2*ii)));

    qDebug() << myData;
    free(myData);
    myBuffer->seek(0);
    qDebug() << myBuffer->size();
    QDataStream myStream(myBuffer);
    //QDataStream myStream(myByteArray,QIODevice::ReadOnly);
    //QVector<float> data(44110;
    //qDebug() << myStream;
    //qDebug() << *myByteArray;

    //myStream >> data;
    //qDebug() << "Data length is " << data.length() ;
   // for(int ii = 0; ii< data.length();ii++)
    //    qDebug() << data[ii];
    //for(int ii = 0; ii< 10;ii++)
     //   qDebug() << myByteArray->at(ii);

}

void myAudioInput::stopRecording()
{
    qDebug() << "reached stop recording class.";
    readData();
    myBuffer->close();
    audio->stop();
    delete audio;
}

void myAudioInput::handleStateChanged(QAudio::State state)
{
    switch (state) {
     case QAudio::StoppedState:
        if(audio->error() != QAudio::NoError)
            qDebug() << "There was an error. ";
        else
            qDebug() << "Finished recording.";
        break;
     case QAudio::ActiveState:
        qDebug() << "Started recording.";
        break;
    case QAudio::SuspendedState:
        qDebug() <<"Suspended state.";
        break;
    case QAudio::IdleState:
        qDebug() <<"Idle state.";
        break;
     default:
        qDebug() << "some other state change happened.";
        break;
    }
}

myInputIODevice::myInputIODevice (const QAudioFormat &format, QObject *parent)
{

            switch (format.sampleSize()) {
            case 8:
                switch (format.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    maxAmplitude = 255;
                    break;
                case QAudioFormat::SignedInt:
                    maxAmplitude = 127;
                    break;
                default:
                    break;
                }
                break;
            case 16:
                switch (format.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    maxAmplitude = 65535;
                    break;
                case QAudioFormat::SignedInt:
                    maxAmplitude = 32767;
                    break;
                default:
                    break;
                }
                break;

            case 32:
                switch (format.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    maxAmplitude = 0xffffffff;
                    break;
                case QAudioFormat::SignedInt:
                    maxAmplitude = 0x7fffffff;
                    break;
                case QAudioFormat::Float:
                    maxAmplitude = 0x7fffffff; // Kind of
                default:
                    break;
                }
                break;

            default:
                break;
            }
    myAudioData.resize(44100);

}

myInputIODevice::~myInputIODevice()
{
    close();

}

void myInputIODevice::start()
{
    open(QIODevice::WriteOnly);

}

qint64 myInputIODevice::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    return 0;
}

qint64 myInputIODevice::writeData(const char *data, qint64 len)
{
    if (maxAmplitude) {
        Q_ASSERT(myAudioFormat.sampleSize() % 8 == 0);
        const int channelBytes = myAudioFormat.sampleSize() / 8;
        const int sampleBytes = myAudioFormat.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const int numSamples = len / sampleBytes;

        quint32 maxValue = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
     // const unsigned char *ptr = data;
        for (int i = 0; i < numSamples; ++i) {
            for (int j = 0; j < myAudioFormat.channelCount(); ++j) {
                quint32 value = 0;

                if (myAudioFormat.sampleSize() == 8 && myAudioFormat.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } else if (myAudioFormat.sampleSize() == 8 && myAudioFormat.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                } else if (myAudioFormat.sampleSize() == 16 && myAudioFormat.sampleType() == QAudioFormat::UnSignedInt) {
                    if (myAudioFormat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (myAudioFormat.sampleSize() == 16 && myAudioFormat.sampleType() == QAudioFormat::SignedInt) {
                    if (myAudioFormat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                } else if (myAudioFormat.sampleSize() == 32 && myAudioFormat.sampleType() == QAudioFormat::UnSignedInt) {
                    if (myAudioFormat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint32>(ptr);
                    else
                        value = qFromBigEndian<quint32>(ptr);
                } else if (myAudioFormat.sampleSize() == 32 && myAudioFormat.sampleType() == QAudioFormat::SignedInt) {
                    if (myAudioFormat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint32>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint32>(ptr));
                } else if (myAudioFormat.sampleSize() == 32 && myAudioFormat.sampleType() == QAudioFormat::Float) {
                    value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
                }

                maxValue = qMax(value, maxValue);
                ptr += channelBytes;
            }
        }

        maxValue = qMin(maxValue, maxAmplitude);
     //   m_level = qreal(maxValue) / maxAmplitude;
    }

    return 0;
}
