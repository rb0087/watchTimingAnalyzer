#include <QCoreApplication>
#include <QtCore>
#include "myaudioinput.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() <<"What the fuck. ";
    myAudioInput testInput;
    return a.exec();
}
