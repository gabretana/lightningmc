#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "convert.h"
#include "lightningfiles.h"
#include "codecconfigdialog.h"

class QComboBox;
class QLabel;
class QListWidget;
class QProcess;
class QProgressBar;
class QMenu;
class QAction;
class QActionGroup;

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
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void selectTargetFolder();
    void about();
    void aboutQt();

    void addFiles();
    void removeFile();
    void clearFiles();
    void convertFiles();

    void fileConvertionFinished(int file);
    void convertionFinished();

    void valuesFromConfigDialog(QString qbitrate, QString qrate);

    void codecConfig();

    void darkTheme();
    void lightTheme();
    void systemTheme();

private:
    Ui::MainWindow *ui;
    void checkConverter();
    void createActions();
    void createMenus();
    void createComboBoxes();
    void createLabels();
    void createListWidget();
    void createProgressBar();
    void addFormats();
    void readSettings();
    void writeSettins();
    void setIcons();
    void connectThreadActions();
    void addFileToLists(QStringList g_files);
    void setTheme(QString stheme);

    QComboBox *codecCB;
    QLabel *filesHeaderLb, *targetFolderLb;
    QListWidget *addedFilesLW;
    QProgressBar *convertPrB;

    QMenu *editMenu, *fileMenu, *helpMenu, *themeMenu;
    QAction *codecConfigAct, *aboutAct, *aboutQtAct, *exitAct, *selectTargFolderAct, *darkThemeAct, *lightThemeAct, *sysThemeAct;
    QActionGroup *themeActGroup;

    QString pCommand, targetFolder, theme, codec, bitrate, rate;
    QStringList fileNames, files;
    int filesconverted;

    QMap<QString, QString> formats;

    //convert
    Convert *convertion;
    LightningFiles *lFiles;
    CodecConfigDialog *ccd;

    //Thread
    QThread* ffmpegThread;
};

#endif // MAINWINDOW_H
