#include "convert.h"
#include <QDebug>

Convert::Convert(QObject *parent) : QObject(parent)
{
    ffmpeg = new QProcess(this);
    connectActions();
    ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
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
    connect(ffmpeg, SIGNAL(finished(int)), this, SLOT(finished()));
    connect(ffmpeg, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
}

void Convert::convert()
{
    QStringList args;
    if(g_files.size() > 0) {
        args << g_arguments << g_convertedfilenames[0];
        args.insert(1, g_files[0]);
        ffmpeg->start(pProcess, args);
        ffmpeg->waitForStarted();
        ffmpeg->waitForFinished();
        ffmpeg->close();
    } else {
        qWarning() << "Lightning Convert: there are no files";
        filesconverted = 0;
        emit convertionFinished();
    }
    args.clear();
}

void Convert::setFiles(QStringList files)
{
    g_files = files;
}


void Convert::setConvertedFileNames(QStringList fileNames)
{
    g_convertedfilenames = fileNames;
}

void Convert::setArguments(QStringList arguments)
{
    g_arguments = arguments;
}

void Convert::finished()
{
        if(g_files.size() > 0) {
            filesconverted++;
            emit fileConvertionFinished(filesconverted);
            g_files.removeFirst();
            g_convertedfilenames.removeFirst();
            convert();
        } else
            emit convertionFinished();
}

void Convert::error()
{

}

void Convert::startConvertion()
{
    filesconverted = 0;
    convert();
}

void Convert::readStandardOutput()
{
    qDebug() << ffmpeg->readAllStandardOutput();
}
