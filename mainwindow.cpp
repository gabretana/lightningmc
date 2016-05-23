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
    checkConverter();
    createComboBoxes();
    createLabels();
    createListWidget();
    ui->actionConvert_Files->setEnabled(false);
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
    targetFolderLb->setText(tr("Target folder: %1").arg(targetFolder));
}

void MainWindow::about()
{
    QMessageBox::about(this, "About LightningMC", tr("<h2>LightningMC v0.6</h2>"
                                                   "<p>Gabriel Retana, Copyleft 2016</p>"
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
        addedFilesLW->item(i)->setIcon(QIcon::fromTheme("emblem-urgent", "://img/m_time.svg"));
    }
    ui->actionConvert_Files->setEnabled(true);
    ui->actionClear_Files->setEnabled(true);
    ui->actionRemove_File->setEnabled(true);
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
}

void MainWindow::convertFiles()
{
    convertPrB->setMaximum(files.size());
    convertPrB->setVisible(true);
    convertPrB->setValue(1);
    addedFilesLW->item(filesconverted)->setIcon(QIcon::fromTheme("media-playback-start", "://img/m_play.svg"));
    filesconverted = 0;
    ui->statusBar->showMessage(tr("Converting: %1").arg(files[0]));

    lFiles->setFilesSuffix(codecCB->currentText()); //add new file suffix
    codec = formats[codecCB->currentText()];
    lFiles->setPath(targetFolder);
    lFiles->addNewSuffix();

    QStringList args;
    args << "-i" << "-c:a" << codec << "-r:a" << rate << "-b:a" << bitrate << "-ac" << "2" << "-vn";

    convertion->setFiles(files);
    convertion->setConvertedFileNames(lFiles->filesWithNewSuffix());
    convertion->setArguments(args);

    ui->actionAddFiles->setEnabled(false);
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
    move(settings.value("Position", QPoint(200, 200)).toPoint());
    resize(settings.value("WinSize", QSize(600, 400)).toSize());
    targetFolder = settings.value("PathToSave", QDir::homePath()+ "/").toString();
    settings.endGroup();

    settings.beginGroup("Codec");
    codec = settings.value("Codec", QString("libvorbis")).toString();
    bitrate = settings.value("Bitrate", QString("160k")).toString();
    rate = settings.value("Rate", QString("48k")).toString();
    settings.endGroup();

    codecCB->setCurrentText(formats.key(codec));
    targetFolderLb->setText(tr("Target folder: %1").arg(targetFolder));
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
    addedFilesLW->item(filesconverted)->setIcon(QIcon::fromTheme("emblem-default", "://img/m_check.svg"));
    ++filesconverted;
    convertPrB->setValue(file + 1);
    ui->statusBar->showMessage(tr("Converting: %1").arg(files[file]));
    addedFilesLW->item(filesconverted)->setIcon(QIcon::fromTheme("media-playback-start", "://img/m_play.svg"));
}

void MainWindow::convertionFinished()
{
    qWarning() << "Convertion Finished";
    convertPrB->setVisible(false);
    ui->statusBar->showMessage(tr("Convertion finished"), 10);
    files.clear();

    ui->actionAddFiles->setEnabled(true);
    ui->actionClear_Files->setEnabled(true);
    ui->actionRemove_File->setEnabled(true);
}


void MainWindow::setIcons()
{
    ui->actionAddFiles->setIcon(QIcon::fromTheme("list-add", QIcon("://img/m_add.svg")));
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
            addedFilesLW->addItem(url.toLocalFile());
            files << url.toLocalFile();
            lFiles->addFile(url.toLocalFile());
            addedFilesLW->item(files.size() - 1)->setIcon(QIcon("emblem-urgent", "://img/m_time.svg"));
        }
    }
    if(!files.isEmpty())
        ui->actionConvert_Files->setEnabled(true);
}
