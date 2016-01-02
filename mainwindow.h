#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "convert.h"

class QComboBox;
class QLabel;
class QListWidget;
class QProcess;
class QProgressBar;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void selectTargetFolder();
    void about();
    void aboutQt();
    void addFiles();
    void removeFile();
    void clearFiles();
    void convertFiles();

private:
    Ui::MainWindow *ui;
    void checkConverter();
    void createActions();
    void createComboBoxes();
    void createLabels();
    void createListWidget();
    void createProgressBar();
    void addFormats();
    QComboBox *codecCB, *bitrateCB, *rateCB;
    QLabel *filesHeaderLb, *targetFolderLb;
    QListWidget *addedFilesLW;
    QProgressBar *convertPrB;
    QProcess *converter;
    QString pCommand, targetFolder;
    QStringList fileNames, files;
    QMap<QString, QString> formats;

    //convert
    convert *convertion;
};

#endif // MAINWINDOW_H
