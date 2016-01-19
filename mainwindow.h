#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "convert.h"
#include "lightningfiles.h"

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

protected:
    void closeEvent(QCloseEvent *event);

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
    void readSettings();
    void writeSettins();
    QComboBox *codecCB, *bitrateCB, *rateCB;
    QLabel *filesHeaderLb, *targetFolderLb;
    QListWidget *addedFilesLW;
    QProgressBar *convertPrB;
    QProcess *converter;
    QString pCommand, targetFolder, theme, codec, bitrate, rate;
    QStringList fileNames, files;
    QMap<QString, QString> formats;

    //convert
    Convert *convertion;
    LightningFiles *lFiles;
};

#endif // MAINWINDOW_H
