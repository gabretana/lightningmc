#ifndef LIGHTNINGFILES_H
#define LIGHTNINGFILES_H

#include <QObject>
#include <QFileInfo>

class LightningFiles : public QObject
{
    Q_OBJECT
public:
    explicit LightningFiles(QObject *parent = 0);
    void addFilesToChangeSuffix(QStringList);
    void setFilesSuffix(QString);
    void setPath(QString p);
    QStringList filesWithNewSuffix() {return pFilesWithNewSuffix;}
    QString newSuffix() {return newsuffix;}
    QString path() {return newPath;}
    void addNewSuffix();
    void clearAllData();

signals:

public slots:

private:

    //voids
    //void getFileBaseName(QStringList);
    void addFileInfoToList(QStringList files);

    //variables
    QStringList filesWithOldSuffix, pFilesWithNewSuffix, fileNames;
    QString newsuffix, newPath;
    QFileInfoList fileInfoList;
};

#endif // LIGHTNINGFILES_H
