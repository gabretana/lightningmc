#include "convert.h"
#include <QDebug>

Convert::Convert(QObject *parent) : QObject(parent)
{
    ffmpeg = new QProcess(this);
    connectActions();
    ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
}

void Convert::connectActions()
{
    //QProcess
    connect(ffmpeg, SIGNAL(finished(int)), this, SLOT(finish()));
    connect(ffmpeg, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
}

void Convert::convert()
{
    QStringList args;
    if(filesconverted < g_files.size()) {
        args << g_arguments << g_convertedfilenames[filesconverted];
        args.insert(1, g_files[filesconverted]);
        ffmpeg->close();
        ffmpeg->start(pProcess, args);
        ffmpeg->waitForStarted();
        ffmpeg->waitForFinished();
    }
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

void Convert::finish() {
    filesconverted++;

    if(filesconverted < g_files.size()) {
        /*g_files.removeAt(0);
        g_convertedfilenames.removeAt(0);*/
        emit fileConvertionFinished(filesconverted);
        convert();
    } else {
        qWarning() << "Lightning Convert: there are no files";
        filesconverted = 0;
        g_files.clear();
        g_convertedfilenames.clear();
        emit convertionFinished();
    }

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
