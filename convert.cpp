#include "convert.h"
#include <QDebug>

Convert::Convert(QObject *parent) : QObject(parent)
{
    ffmpeg = new QProcess(this);
}

void Convert::processUsed()
{
    ffmpeges = QProcess::execute("ffmpeg", QStringList() << "-version");
    libaves = QProcess::execute("avconv", QStringList() << "-version");
    if(ffmpeges == 0)
        pProcess = "ffmpeg";
    else
        pProcess = "avconv";
    qDebug() << "Lightning Convert: using " + pProcess;
}

void Convert::connectActions()
{
    //QProcess
    connect(ffmpeg, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    connect(ffmpeg, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
}

void Convert::convert()
{
    qInfo() << "Lightning Convert: starting";
    if(g_files.size() > 0)
        ffmpeg->start(pProcess, g_arguments << g_files[0]);
    else {
        qWarning() << "Lightning Convert: there are no files";
        emit convertionFinished();
    }
}

void Convert::setFiles(QStringList files)
{
    g_files = files;
}

void Convert::setArguments(QStringList arguments)
{
    g_arguments = arguments;
}

void Convert::finished(int i, QProcess::ExitStatus exitStatus)
{
    qWarning() << "Lightning Convert: process exit code " + QString::number(i) + " exit status " + QString::number(exitStatus);
    if(exitStatus != 0) {
        emit convertionFinished();
        g_files.removeFirst();
        convert();
    } else
        emit processCrash(exitStatus);
}

void Convert::error(QProcess::ProcessError e)
{
    QString processError = QString::number(e);
    qWarning() << "Lightning process " + pProcess + " exit with error " + ffmpeg->errorString(); + " " + processError;
    emit processErrorReport(ffmpeg->errorString());
}

void Convert::startConvertion(QStringList files)
{
    g_files = files;
    convert();
}

void Convert::startConvertion()
{
    convert();
}
