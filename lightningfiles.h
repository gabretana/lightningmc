#ifndef LIGHTNINGFILES_H
#define LIGHTNINGFILES_H

#include <QObject>
#include <QFileInfo>

class LightningFiles : public QObject
{
    Q_OBJECT
public:
    explicit LightningFiles(QObject *parent = 0);
    void setFilesToChangeSuffix(QStringList);
    void setFilesSubffix(QString);
    QStringList filesWithNewSuffix() {return pFilesWithNewSuffix;}
    QString newSuffix() {return newsuffix;}
    QStringList addNewSuffix(QStringList, QString, QString);

signals:

public slots:

private:

    //voids
    void getFileBaseName(QStringList);

    //variables
    QStringList filesWithOldSuffix, pFilesWithNewSuffix, fileNames;
    QString newsuffix;
    QFileInfoList fileInfoList;
};

#endif // LIGHTNINGFILES_H
