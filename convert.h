#ifndef CONVERT_H
#define CONVERT_H
#include <QObject>

class convert : public QObject
{
    Q_OBJECT
public:
    explicit convert(QObject *parent = 0);
    void setFiles(QStringList*);
    QStringList files() {return files;}
    void removeFile(int);
    void removeAllFiles();

signals:

public slots:

private:
    void setNameFiles();
    QStringList *files;
};

#endif // CONVERT_H
