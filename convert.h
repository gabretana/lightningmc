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

signals:
    void processFinished(int, QProcess::ExitStatus);

public slots:
    //void finished(int, QProcess::ExitStatus);

private:
    void connectActions();
    QProcess *ffmpeg;
    QString pProcess;
    int ffmpeges, libaves;
    LightningFiles *files;
};

#endif // CONVERT_H
