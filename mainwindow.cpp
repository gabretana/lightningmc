#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon::fromTheme("lightningmc", QIcon(QString::fromUtf8("://img/lightning.png"))));
    setWindowTitle("LightningMC");
    convertion = new Convert(this);
    lFiles = new LightningFiles(this);
    checkConverter();
    targetFolder = QDir::homePath() + "/";
    createComboBoxes();
    createLabels();
    createListWidget();
    ui->actionConvert_Files->setEnabled(false);
    addFormats();
    converter = new QProcess(this);
    createActions();
    createMenus();
    createProgressBar();
    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkConverter()
{
    convertion->processUsed();
    pCommand = convertion->process();
    ui->statusBar->showMessage("Using: " + pCommand, 10000);
}

void MainWindow::createComboBoxes()
{
    codecCB = new QComboBox(this);
    codecCB->addItems(QStringList() << "OGG" << "M4A" << "MP3");
    ui->mainToolBar->addWidget(codecCB);
    ui->mainToolBar->setMovable(false);
}


void MainWindow::createLabels()
{
    filesHeaderLb = new QLabel("Files: ", this);
    ui->filesVLayout->addWidget(filesHeaderLb);

    targetFolderLb = new QLabel("Target Path: " + targetFolder, this);
    targetFolderLb->setMaximumWidth(350);
    ui->miscHLayout->addWidget(targetFolderLb);
    ui->miscHLayout->addStretch();
}

void MainWindow::createListWidget()
{
    addedFilesLW = new QListWidget(this);
    ui->filesVLayout->addWidget(addedFilesLW);
}

void MainWindow::createProgressBar()
{
    convertPrB = new QProgressBar(this);
    convertPrB->setVisible(false);
    ui->miscHLayout->addWidget(convertPrB);
}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(selectTargFolderAct);
    fileMenu->addAction(exitAct);

    editMenu = new QMenu(tr("&Edit"), this);
    editMenu->addAction(codecConfigAct);

    themeMenu = editMenu->addMenu(tr("&Theme"));
    themeMenu->addAction(lightThemeAct);
    themeMenu->addAction(darkThemeAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    ui->menuBar->addMenu(fileMenu);
    ui->menuBar->addMenu(editMenu);
    ui->menuBar->addMenu(helpMenu);
}

void MainWindow::createActions()
{
    codecConfigAct = new QAction(tr("Codec Configuration"),this);
    codecConfigAct->setShortcut(QKeySequence("Ctrl+Shift+D"));
    connect(codecConfigAct, SIGNAL(triggered(bool)), this, SLOT(codecConfig()));

    selectTargFolderAct = new QAction(tr("Select Target Folder"), this);
    selectTargFolderAct->setShortcut(QKeySequence("Ctrl+T"));
    connect(selectTargFolderAct, SIGNAL(triggered(bool)), this, SLOT(selectTargetFolder())); //select folder

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered(bool)), this, SLOT(close())); //close window

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setShortcut(QKeySequence("Ctrl+F1"));
    connect(aboutAct, SIGNAL(triggered(bool)), this, SLOT(about())); //about

    aboutQtAct = new QAction(tr("About Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered(bool)), this, SLOT(aboutQt())); //about qt

    connect(ui->actionAddFiles, SIGNAL(triggered(bool)), this, SLOT(addFiles())); //add files
    connect(ui->actionRemove_File, SIGNAL(triggered(bool)), this, SLOT(removeFile())); //remove file
    connect(ui->actionClear_Files, SIGNAL(triggered(bool)), this, SLOT(clearFiles())); //clear files
    connect(ui->actionConvert_Files, SIGNAL(triggered(bool)), this, SLOT(convertFiles())); //convert files

    connect(convertion, SIGNAL(fileConvertionFinished(int)), this, SLOT(fileConvertionFinished(int)));
    connect(convertion, SIGNAL(convertionFinished()), this, SLOT(convertionFinished()));

    themeActGroup = new QActionGroup(this);

    lightThemeAct = new QAction(tr("Light"), themeActGroup);
    lightThemeAct->setCheckable(true);
    connect(lightThemeAct, SIGNAL(triggered(bool)), this, SLOT(lightTheme()));

    darkThemeAct = new QAction(tr("Dark"), themeActGroup);
    darkThemeAct->setCheckable(true);
    connect(darkThemeAct, SIGNAL(triggered(bool)), this, SLOT(darkTheme()));

}


void MainWindow::addFormats()
{
    formats["OGG"] = "libvorbis";
    formats["M4A"] = "aac";
    formats["MP3"] = "libmp3lame";
}

void MainWindow::selectTargetFolder()
{
    targetFolder = QFileDialog::getExistingDirectory(this, "LightningMC", QDir::homePath());
    if(targetFolder.isEmpty()) {
        targetFolder = QDir::homePath() + "/";
        lFiles->setPath(targetFolder);
    } else {
        targetFolder = targetFolder + "/";
        lFiles->setPath(targetFolder);
    }
    targetFolderLb->setText("Target Folder: " + targetFolder);
}

void MainWindow::about()
{
    QMessageBox::about(this, "About LightningMC", tr("<h2>LightningMC v0.4</h2>"
                                                   "<p>Gabriel Retana, Copyleft 2015</p>"
                                                   "<p>GNU General Public License v3</p>"));
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::addFiles()
{
    fileNames.clear();
    fileNames = QFileDialog::getOpenFileNames(this, "Add Files", QDir::homePath(),
                                              "Audio Files (*.ogg *.oga *.mp3 *.m4a *.wma *.flac *.aiff)");
    if(!fileNames.isEmpty()){
        addedFilesLW->addItems(fileNames);
        ui->actionConvert_Files->setEnabled(true);
        files << fileNames;
        lFiles->addFilesToChangeSuffix(fileNames);
    }
    for(int i = 0; i < addedFilesLW->count(); ++i){
        addedFilesLW->item(i)->setIcon(QIcon::fromTheme("emblem-urgent"));
    }
}

void MainWindow::removeFile()
{
    if(addedFilesLW->count() > 0) {
        files.removeAt(addedFilesLW->currentRow());
        lFiles->removeFileAt(addedFilesLW->currentRow());
        delete addedFilesLW->currentItem();
        if(addedFilesLW->count() == 0)
            ui->actionConvert_Files->setEnabled(false);
    }
}

void MainWindow::clearFiles()
{
    if(addedFilesLW->count() > 0) {
        addedFilesLW->clear();
        files.clear();
        ui->actionConvert_Files->setEnabled(false);
        lFiles->clearAllData();
    }
}

void MainWindow::convertFiles()
{
    convertPrB->setMaximum(files.size());
    convertPrB->setVisible(true);
    filesconverted = 0;
    lFiles->setFilesSuffix(codecCB->currentText()); //add new file suffix
    lFiles->setPath(targetFolder);
    lFiles->addNewSuffix();

    QStringList args;
    args << "-i" << "-c:a" << codec << "-r:a" << rate << "-b:a" << bitrate << "-ac" << "2" << "-vn";

    if(codec != "aac")
        args << "-strict" << "-2";

    convertion->setFiles(files);
    convertion->setConvertedFileNames(lFiles->filesWithNewSuffix());
    convertion->setArguments(args);

    convertion->startConvertion();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettins();
}

void MainWindow::writeSettins()
{
    QSettings settings("GXA Software", "LightningMC");
    settings.beginGroup("General");
    settings.setValue("Theme", theme);
    settings.setValue("Position", pos());
    settings.setValue("WinSize", size());
    settings.setValue("PathToSave", targetFolder);
    settings.endGroup();

    codec = formats[codecCB->currentText()];
    settings.beginGroup("Codec");
    settings.setValue("Codec", codec);
    settings.setValue("Bitrate", bitrate);
    settings.setValue("Rate", rate);
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings("GXA Software", "LightningMC");
    settings.beginGroup("General");
    theme = settings.value("Theme", "light").toString();
    move(settings.value("Position", QPoint(200, 200)).toPoint());
    resize(settings.value("WinSize", QSize(600, 400)).toSize());
    targetFolder = settings.value("PathToSave", QDir::homePath()+ "/").toString();
    lFiles->setPath(targetFolder);
    settings.endGroup();

    settings.beginGroup("Codec");
    codec = settings.value("Codec", QString("libvorbis")).toString();
    bitrate = settings.value("Bitrate", QString("160k")).toString();
    rate = settings.value("Rate", QString("48k")).toString();
    settings.endGroup();

    codecCB->setCurrentText(formats.key(codec));
    setTheme(theme);
}

void MainWindow::codecConfig()
{
    ccd = new CodecConfigDialog(this);
    ccd->setWindowTitle(tr("%1 Configuration").arg(codecCB->currentText()));
    ccd->setCurrentConfig(bitrate, rate);
    connect(ccd, SIGNAL(accepted(QString,QString)), this, SLOT(valuesFromConfigDialog(QString,QString))); //configuration dialog
    ccd->exec();
}

void MainWindow::valuesFromConfigDialog(QString qbitrate, QString qrate)
{
    this->bitrate = qbitrate;
    this->rate = qrate;
}


void MainWindow::fileConvertionFinished(int file)
{
        ++filesconverted;
        convertPrB->setValue(file);
        ui->statusBar->showMessage("Converting: " + files[0]);
}

void MainWindow::convertionFinished()
{
    qWarning() << "Convertion Finished";
    convertPrB->setVisible(false);
    ui->statusBar->showMessage(tr("Convertions finished"), 10);
    files.clear();
    ui->actionConvert_Files->setEnabled(false);
}


void MainWindow::setTheme(QString stheme)
{
    if(stheme == "light") {
        QFile file("://themes/light.qss");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            setStyleSheet(file.readAll());
            file.close();
            setIcons(stheme);
        }
    } else {
        QFile file("://themes/dark.qss");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            setStyleSheet(file.readAll());
            file.close();
            setIcons(stheme);
        }
    }
}

void MainWindow::setIcons(QString stheme)
{
    if(stheme == "light") {
        ui->actionAddFiles->setIcon(QIcon(QString::fromUtf8("://img/ic_add_black_36px.svg")));
        ui->actionRemove_File->setIcon(QIcon(QString::fromUtf8("://img/ic_remove_black_36px.svg")));
        ui->actionClear_Files->setIcon(QIcon(QString::fromUtf8("://img/ic_clear_black_36px.svg")));
        ui->actionConvert_Files->setIcon(QIcon(QString::fromUtf8("://img/ic_sync_black_32px.svg")));
        lightThemeAct->setChecked(true);
    } else {
        ui->actionAddFiles->setIcon(QIcon(QString::fromUtf8("://img/ic_add_white_36px.svg")));
        ui->actionRemove_File->setIcon(QIcon(QString::fromUtf8("://img/ic_remove_white_36px.svg")));
        ui->actionClear_Files->setIcon(QIcon(QString::fromUtf8("://img/ic_clear_white_36px.svg")));
        ui->actionConvert_Files->setIcon(QIcon(QString::fromUtf8("://img/ic_sync_white_32px.svg")));
        darkThemeAct->setChecked(true);
    }
}

void MainWindow::lightTheme()
{
    theme = "light";
    setTheme(theme);
}

void MainWindow::darkTheme()
{
    theme = "dark";
    setTheme(theme);
}
