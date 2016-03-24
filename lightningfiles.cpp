#include "lightningfiles.h"
#include <QDebug>

LightningFiles::LightningFiles(QObject *parent) : QObject(parent)
{
    newsuffix = ".ogg";
}

void LightningFiles::setFilesSuffix(QString suffix)
{
    newsuffix = suffix;
    qInfo() << "LightningFiles new suffix: " + newsuffix;
}

void LightningFiles::setPath(QString p)
{
    newPath = p;
    qInfo() << "LightningFiles new path: " + newPath;
}

void LightningFiles::addFilesToChangeSuffix(QStringList files)
{
    filesWithOldSuffix = files;
    addFileInfoToList(files);
}

void LightningFiles::addNewSuffix()
{
    QString tmp = "", filetmp = "";
    int count = 0;
    foreach (tmp, fileNames) {
        //filetmp = newPath + tmp.remove(fileInfoList[count].suffix(), Qt::CaseInsensitive) + newsuffix.toLower();
        filetmp = newPath + tmp.remove(QFileInfo(tmp).suffix(), Qt::CaseInsensitive) + newsuffix.toLower();

        qDebug() << "*** Lightning Files: " + filetmp;

        if(QFile(filetmp).exists()) {
            qWarning() << "LightningFiles file exists: " + filetmp;
            emit fileExists(tmp, filetmp);
        } else
            pFilesWithNewSuffix << filetmp;
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

void LightningFiles::addFile(QString filename)
{
    filesWithOldSuffix << filename;
    if(filename != "") {
        fileInfoList << QFileInfo(filename);
        fileNames << QFileInfo(filename).fileName();
        qDebug() << "LightningFiles add file: " + fileNames[fileNames.size() - 1];
    }
}
