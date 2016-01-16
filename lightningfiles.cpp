#include "lightningfiles.h"
#include <QDebug>

LightningFiles::LightningFiles(QObject *parent) : QObject(parent)
{
    newsuffix = ".ogg";
}

void LightningFiles::setFilesSuffix(QString suffix)
{
    newsuffix = suffix;
    qDebug() << "LightningFiles new suffix" + newsuffix;
}

void LightningFiles::setPath(QString p)
{
    newPath = p;
    qDebug() << "LightningFiles new path" + newPath;
}

void LightningFiles::addFilesToChangeSuffix(QStringList files)
{
    filesWithOldSuffix = files;
    addFileInfoToList(files);
}

void LightningFiles::addNewSuffix()
{
    QString tmp = "";
    int count = 0;
    foreach (tmp, fileNames) {
        pFilesWithNewSuffix << newPath + tmp.remove(fileInfoList[count].suffix(), Qt::CaseInsensitive) + newsuffix.toLower();
        ++count;
        qDebug() << "LightningFiles: new suffix " + pFilesWithNewSuffix[count];
    }
}


void LightningFiles::addFileInfoToList(QStringList files)
{
    for(int i = 0; i < files.size(); ++i) {
        fileInfoList << QFileInfo(files[i]);
        fileNames << QFileInfo(files[i]).fileName();
        qDebug() << "LightningFiles add file: " + fileNames[i];
    }
}

void LightningFiles::removeFileAt(int f)
{
    QString file(filesWithOldSuffix[f]);
    filesWithOldSuffix.removeAt(f);
    pFilesWithNewSuffix.removeAt(f);
    fileNames.removeAt(f);
    qInfo() << "LightningFiles removed file: " + file;
}

void LightningFiles::clearAllData()
{
    filesWithOldSuffix.clear();
    pFilesWithNewSuffix.clear();
    fileNames.clear();
    newPath = "";
    newsuffix = ".ogg";
    fileInfoList.clear();
    qInfo() << "LightningFiles clear all";
}
