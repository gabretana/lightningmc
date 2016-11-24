#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);
    setWindowIcon(QIcon::fromTheme("lightningmc", QIcon(QString::fromUtf8("://img/lightningmc.svg"))));
    setWindowTitle("LightningMC");
    setIcons();
    convertion = new Convert();
    lFiles = new LightningFiles(this);
    createComboBoxes();
    createLabels();
    createListWidget();
    createToolButton();
    ui->actionConvert_Files->setEnabled(false);
    ui->actionClear_Files->setEnabled(false);
    ui->actionRemove_File->setEnabled(false);
    addFormats();
    ffmpegThread = new QThread;
    createActions();
    createMenus();
    connectThreadActions();
    createProgressBar();
    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createComboBoxes()
{
    codecCB = new QComboBox(this);
    ui->mainToolBar->addWidget(codecCB);
    ui->mainToolBar->setMovable(false);
    comboBoxAddItems();
}


void MainWindow::createLabels()
{
    filesHeaderLb = new QLabel(tr("Files: "), this);
    ui->filesVLayout->addWidget(filesHeaderLb);

    targetFolderLb = new QLabel(tr("Save in: %1").arg(targetFolder), this);
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

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    themeMenu = editMenu->addMenu(tr("&Theme"));
    themeMenu->addAction(lightThemeAct);
    themeMenu->addAction(darkThemeAct);
    themeMenu->addAction(sysThemeAct);

    convMenu = new QMenu(this);
    convMenu->addAction(addAudioAct);
    convMenu->addAction(addImageAct);



    addFilesTB->setMenu(convMenu);
    ui->menuBar->addMenu(fileMenu);
    ui->menuBar->addMenu(editMenu);
    ui->menuBar->addMenu(helpMenu);
}

void MainWindow::createActions()
{
    codecConfigAct = new QAction(tr("Codec Settings"),this);
    codecConfigAct->setShortcut(QKeySequence("Ctrl+Shift+D"));
    connect(codecConfigAct, SIGNAL(triggered(bool)), this, SLOT(codecConfig()));

    selectTargFolderAct = new QAction(tr("Save in..."), this);
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

    addAudioAct = new QAction(tr("Add Audio"));
    connect(addAudioAct, SIGNAL(triggered(bool)), this, SLOT(addAudio())); // add audio files

    addImageAct = new QAction(tr("Add Images"));
    connect(addImageAct, SIGNAL(triggered(bool)), this, SLOT(addImage())); // add image files

    //connect(ui->actionAddFiles, SIGNAL(triggered(bool)), this, SLOT(addFiles())); //add files
    connect(ui->actionRemove_File, SIGNAL(triggered(bool)), this, SLOT(removeFile())); //remove file
    connect(ui->actionClear_Files, SIGNAL(triggered(bool)), this, SLOT(clearFiles())); //clear files
    connect(ui->actionConvert_Files, SIGNAL(triggered(bool)), this, SLOT(convertFiles())); //convert files

    connect(convertion, SIGNAL(fileConvertionFinished(int)), this, SLOT(fileConvertionFinished(int)));
    connect(convertion, SIGNAL(convertionFinished()), this, SLOT(convertionFinished()));

    //theme actions
    themeActGroup = new QActionGroup(this);
    lightThemeAct = new QAction(tr("&Light"), themeActGroup);
    lightThemeAct->setCheckable(true);
    connect(lightThemeAct, &QAction::triggered, this, &MainWindow::lightTheme);

    darkThemeAct = new QAction(tr("&Dark"), themeActGroup);
    darkThemeAct->setCheckable(true);
    connect(darkThemeAct, &QAction::triggered, this, &MainWindow::darkTheme);

    sysThemeAct = new QAction(tr("&System (Needs Restart)"), themeActGroup);
    sysThemeAct->setCheckable(true);
    connect(sysThemeAct, &QAction::triggered, this, &MainWindow::systemTheme);
}

void MainWindow::connectThreadActions()
{
    connect(ffmpegThread, SIGNAL(started()), convertion, SLOT(startConvertion()));
    connect(convertion, SIGNAL(convertionFinished()), ffmpegThread, SLOT(quit()));
    //connect(convertion, SIGNAL(convertionFinished()), ffmpegThread, SLOT(deleteLater()));
    //connect(ffmpegThread, SIGNAL(finished()), ffmpegThread, SLOT(deleteLater()));
}

void MainWindow::addFormats()
{
    formats["OGG"] = "libvorbis";
    formats["M4A"] = "aac";
    formats["MP3"] = "libmp3lame";
    formats["JPG"] = "jpg";
    formats["PNG"] = "png";
    formats["TIFF"] = "tiff";
    formats["WEBP"] = "webp";
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
    targetFolderLb->setText(tr("Save in: %1").arg(targetFolder));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About LightningMC"), tr("<h2>LightningMC v0.7.1</h2>"
                                                   "<p>Gabriel Retana, Copyleft 2016</p>"
                                                   "<p>GNU General Public License v3</p>"));
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::addFiles(QString filetype)
{
    QString types;

    if(filetype == "image") {
        types = tr("Image files: %1").arg("(*.jpeg *.jpg *.png *.tiff *.webp)");
        codecConfigAct->setEnabled(false);
        
    } else {
        types = tr("Audio files: %1").arg("(*.ogg *.oga *.mp3 *.m4a *.wma *.flac *.aiff)");
        codecConfigAct->setEnabled(true);
    }

    fileNames.clear();
    fileNames = QFileDialog::getOpenFileNames(this, tr("Add Files"), QDir::homePath(), types);
    if(!fileNames.isEmpty()){
        addedFilesLW->addItems(fileNames);
        ui->actionConvert_Files->setEnabled(true);
        files << fileNames;
        lFiles->addFilesToChangeSuffix(fileNames);
        
        ui->actionConvert_Files->setEnabled(true);
        ui->actionClear_Files->setEnabled(true);
        ui->actionRemove_File->setEnabled(true);
        
    }
    for(int i = 0; i < addedFilesLW->count(); ++i){
        addedFilesLW->item(i)->setIcon(QIcon::fromTheme("emblem-urgent", QIcon("://img/m_time.svg")));
    }
    
}

void MainWindow::removeFile()
{
    if(addedFilesLW->count() > 0) {
        files.removeAt(addedFilesLW->currentRow());
        lFiles->removeFileAt(addedFilesLW->currentRow());
        delete addedFilesLW->currentItem();
        if(addedFilesLW->count() == 0) {
            ui->actionConvert_Files->setEnabled(false);
            ui->actionClear_Files->setEnabled(false);
            ui->actionRemove_File->setEnabled(false);
            addAudioAct->setEnabled(true);
            addImageAct->setEnabled(true);
        }
    }
}

void MainWindow::clearFiles()
{
    if(addedFilesLW->count() > 0) {
        addedFilesLW->clear();
        ui->actionConvert_Files->setEnabled(false);
        lFiles->clearAllData();
    }
    if(files.isEmpty())
        files.clear();

    ui->actionClear_Files->setEnabled(false);
    ui->actionRemove_File->setEnabled(false);
    addAudioAct->setEnabled(true);
    addImageAct->setEnabled(true);
}

void MainWindow::convertFiles()
{
    convertPrB->setMaximum(files.size());
    convertPrB->setVisible(true);
    convertPrB->setValue(1);
    filesconverted = 0;
    addedFilesLW->item(filesconverted)->setIcon(QIcon::fromTheme("media-playback-start", QIcon("://img/m_play.svg")));
    ui->statusBar->showMessage(tr("Converting: %1").arg(files[0]));

    lFiles->setPath(targetFolder);
    lFiles->setFilesSuffix(codecCB->currentText()); //add new file suffix
    codec = formats[codecCB->currentText()];
    lFiles->addNewSuffix();

    QStringList args;
    if(convType == "audio")
        args << "-i" << "-c:a" << codec << "-r:a" << rate << "-b:a" << bitrate << "-ac" << "2" << "-vn";
    else
        args << "-i";

    convertion->setFiles(files);
    convertion->setConvertedFileNames(lFiles->filesWithNewSuffix());
    convertion->setArguments(args);

    //ui->actionAddFiles->setEnabled(false);
    ui->actionConvert_Files->setEnabled(false);
    ui->actionClear_Files->setEnabled(false);
    ui->actionRemove_File->setEnabled(false);

    //convertion->startConvertion();
    convertion->moveToThread(ffmpegThread);
    ffmpegThread->start();
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
    settings.setValue("ConvertionType", convType);
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
    theme = settings.value("Theme", "system").toString();
    move(settings.value("Position", QPoint(200, 200)).toPoint());
    resize(settings.value("WinSize", QSize(600, 400)).toSize());
    targetFolder = settings.value("PathToSave", QDir::homePath()+ "/").toString();
    convType = settings.value("ConvertionType", "audio").toString();
    settings.endGroup();

    settings.beginGroup("Codec");
    codec = settings.value("Codec", QString("libvorbis")).toString();
    bitrate = settings.value("Bitrate", QString("160k")).toString();
    rate = settings.value("Rate", QString("48k")).toString();
    settings.endGroup();

    codecCB->setCurrentText(formats.key(codec));
    targetFolderLb->setText(tr("Save in: %1").arg(targetFolder));
    setTheme(theme);
    
    if(convType == "image")
        codecConfigAct->setEnabled(false);
}

void MainWindow::codecConfig()
{
    ccd = new CodecConfigDialog(this);
    ccd->setWindowTitle(tr("%1 Settings").arg(codecCB->currentText()));
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
    addedFilesLW->item(filesconverted)->setIcon(QIcon::fromTheme("emblem-default", QIcon("://img/m_check.svg")));
    ++filesconverted;
    convertPrB->setValue(file + 1);
    ui->statusBar->showMessage(tr("Converting: %1").arg(files[file]));
    addedFilesLW->item(file)->setIcon(QIcon::fromTheme("media-playback-start", QIcon("://img/m_play.svg")));
}

void MainWindow::convertionFinished()
{
    qWarning() << "Convertion Finished";
    convertPrB->setVisible(false);
    ui->statusBar->showMessage(tr("Convertion finished"), 10);
    files.clear();
    addedFilesLW->item(addedFilesLW->count() - 1)->setIcon(QIcon::fromTheme("emblem-default", QIcon("://img/m_check.svg")));
    //ui->actionAddFiles->setEnabled(true);
    ui->actionClear_Files->setEnabled(true);
    ui->actionRemove_File->setEnabled(true);
}


void MainWindow::setIcons()
{
    //ui->actionAddFiles->setIcon(QIcon::fromTheme("list-add", QIcon("://img/m_add.svg")));
    ui->actionRemove_File->setIcon(QIcon::fromTheme("list-remove", QIcon("://img/m_remove.svg")));
    ui->actionClear_Files->setIcon(QIcon::fromTheme("edit-clear-all", QIcon("://img/m_clear.svg")));
    ui->actionConvert_Files->setIcon(QIcon::fromTheme("emblem-synchronizing", QIcon("://img/m_sync.svg")));
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QMimeDatabase db;
    foreach (QUrl url, event->mimeData()->urls()) {
        QMimeType mime = db.mimeTypeForFile(url.toLocalFile());
        if(mime.inherits("audio/ogg") || mime.inherits("audio/mp4") || mime.inherits("audio/mpeg") ||
                mime.inherits("audio/x-aiff") || mime.inherits("audio/x-ms-wma")) {
            if(convType == "audio") {
                addedFilesLW->addItem(url.toLocalFile());
                files << url.toLocalFile();
                lFiles->addFile(url.toLocalFile());
                addedFilesLW->item(files.size() - 1)->setIcon(QIcon::fromTheme("emblem-urgent", QIcon("://img/m_time.svg")));
            }
        } else if(mime.inherits("image/jpeg") || mime.inherits("image/png") || mime.inherits("image/tiff") ||
                mime.inherits("image/webp")) {
            if(convType == "image") {
                addedFilesLW->addItem(url.toLocalFile());
                files << url.toLocalFile();
                lFiles->addFile(url.toLocalFile());
                addedFilesLW->item(files.size() - 1)->setIcon(QIcon::fromTheme("emblem-urgent", QIcon("://img/m_time.svg")));
            }
       }
    }
    if(!files.isEmpty()) {
        ui->actionConvert_Files->setEnabled(true);
        ui->actionRemove_File->setEnabled(true);
        ui->actionClear_Files->setEnabled(true);
    }    
}

void MainWindow::setTheme(QString stheme)
{
    if(stheme == "light") {
        QFile file(":/themes/themes/light.qss");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            setStyleSheet(file.readAll());
            file.close();
            lightThemeAct->setChecked(true);
        }
    } else if(stheme == "dark") {
        QFile file(":/themes/themes/dark.qss");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            setStyleSheet(file.readAll());
            file.close();
            darkThemeAct->setChecked(true);
        }
    } else {
        setStyleSheet(this->styleSheet());
        sysThemeAct->setChecked(true);
    }
}

void MainWindow::darkTheme()
{
    theme = "dark";
    setTheme(theme);
}

void MainWindow::lightTheme()
{
    theme = "light";
    setTheme(theme);
}

void MainWindow::systemTheme()
{
    theme = "system";
    setTheme(theme);
}

void MainWindow::addAudio()
{
    qInfo() << "Add Audio Files";
    addImageAct->setEnabled(false);
    addFiles("audio");
    convType = "audio";
    comboBoxAddItems();
}

void MainWindow::addImage()
{
    qInfo() << "Add Image Files";
    addAudioAct->setEnabled(false);
    addFiles("image");
    convType = "image";
    comboBoxAddItems();
}

void MainWindow::createToolButton()
{
    addFilesTB = new QToolButton(this);
    addFilesTB->setPopupMode(QToolButton::InstantPopup);
    addFilesTB->setIcon(QIcon::fromTheme("list-add", QIcon("://img/m_add.svg")));
    ui->mainToolBar->insertWidget(ui->actionRemove_File, addFilesTB);
}

void MainWindow::comboBoxAddItems()
{
    codecCB->clear();
    codecCB->update();
    if(convType == "audio")
        codecCB->addItems(QStringList() << "OGG" << "M4A" << "MP3");
    else
        codecCB->addItems(QStringList() << "JPG" << "PNG" << "TIFF" << "WEBP");
}
