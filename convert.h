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
    void processUsed();
    QString process() {return pProcess;}
    void setFiles(QStringList files);
    void setArguments(QStringList arguments);
    void startConvertion(QStringList files);
    void startConvertion();

signals:
    void processFinished(int, QProcess::ExitStatus);
    void processCrash(int);
    void processErrorReport(QString);
    void convertionFinished();

private slots:
    void finished(int, QProcess::ExitStatus);
    void error(QProcess::ProcessError);
    void convert();

private:
    void connectActions();
    QProcess *ffmpeg;
    QString pProcess;
    int ffmpeges, libaves;
    QStringList g_files, backupfiles, g_arguments;
};

#endif // CONVERT_H
