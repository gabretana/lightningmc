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

    void lightTheme();
    void darkTheme();

    //void showCodecConfigDialog();
    void valuesFromConfigDialog(QString qbitrate, QString qrate);

    void codecConfig();

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
    void setIcons(QString theme = "light");

    QComboBox *codecCB;
    QLabel *filesHeaderLb, *targetFolderLb;
    QListWidget *addedFilesLW;
    QProgressBar *convertPrB;

    QMenu *editMenu, *fileMenu, *helpMenu, *themeMenu;
    QAction *codecConfigAct, *aboutAct, *aboutQtAct, *exitAct, *selectTargFolderAct, *darkThemeAct, *lightThemeAct;
    QActionGroup *themeActGroup;


    QProcess *converter;

    QString pCommand, targetFolder, theme, codec, bitrate, rate;
    QStringList fileNames, files;
    int filesconverted;

    QMap<QString, QString> formats;

    //convert
    Convert *convertion;
    LightningFiles *lFiles;
    CodecConfigDialog *ccd;
};

#endif // MAINWINDOW_H
