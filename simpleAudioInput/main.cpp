#include <QCoreApplication>
#include <QtCore>
#include "myaudioinput.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    myAudioInput testInput;
    return a.exec();
}
