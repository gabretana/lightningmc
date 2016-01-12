#include "lightningfiles.h"
#include <QDebug>

LightningFiles::LightningFiles(QObject *parent) : QObject(parent)
{

}

void LightningFiles::setFilesSubffix(QString suffix)
{
    newsuffix = suffix;
}

void LightningFiles::setFilesToChangeSuffix(QStringList files)
{
    filesWithOldSuffix = files;
}

QStringList LightningFiles::addNewSuffix(QStringList files, QString newSuffix, QString path)
{
    filesWithOldSuffix << files;
    addFileInfoToList(files);
    QString tmp = "";
    int count = 0;
    foreach (tmp, filesNames) {
        pFilesWithNewSuffix << path + tmp.remove(fileInfoList[count].suffix(), Qt::CaseInsensitive) + newSuffix.toLower();
        ++count;
        qDebug() << pFilesWithNewSuffix[count];
    }
}


void LightningFiles::addFileInfoToList(QStringList files)
{
    for(int i = 0; i < files.size(); ++i) {
        fileInfoList << QFileInfo(files[i]);
        filesNames << QFileInfo(files[i]).fileName();
        qDebug() << fileNames[i];
    }
}
