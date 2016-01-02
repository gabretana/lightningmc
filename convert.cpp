#include "convert.h"
#include <QDebug>

convert::convert(QObject *parent) : QObject(parent)
{
    ffmpeg = new QProcess(this);
}

void convert::processUsed()
{
    ffmpeges = QProcess::execute("ffmpeg", QStringList() << "-version");
    libaves = QProcess::execute("avconv", QStringList() << "-version");
    if(ffmpeges == 0)
        pProcess = "ffmpeg";
    else
        pProcess = "avconv";
    qDebug() << "Lightning Convert: using " + pProcess;
}

void convert::connectActions()
{
    //QProcess
    //connect(ffmpeg, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
}
