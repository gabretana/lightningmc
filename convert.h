#ifndef CONVERT_H
#define CONVERT_H
#include <QObject>
#include <QProcess>
#include <lightningfiles.h>

class QProcess;

class Convert : public QObject
{
    Q_OBJECT
public:
    explicit Convert(QObject *parent = 0);
    void setFiles(QStringList files);

    void setArguments(QStringList arguments);
    void setConvertedFileNames(QStringList fileNames);

public slots:

    void startConvertion();

signals:
    void processFinished(int, QProcess::ExitStatus);
    void convertionFinished();
    void fileConvertionFinished(int nfiles);

private slots:
    void finish();
    void readStandardOutput();
    void convert();

private:
    void connectActions();
    QProcess *ffmpeg;
    QString pProcess = "ffmpeg";
    int filesconverted, ffmpeges, libaves, g_size;
    QStringList g_files, backupfiles, g_arguments, g_convertedfilenames;
};

#endif // CONVERT_H
