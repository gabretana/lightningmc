#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon::fromTheme("lightningmc", QIcon(QString::fromUtf8("://img/lightning.png"))));
    setWindowTitle("Lightning");
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
    createProgressBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkConverter()
{
    /*int ffExitState, avExitState;
    ffExitState = QProcess::execute("ffmpeg", QStringList() << "-version");
    avExitState = QProcess::execute("avconv", QStringList() << "-version");
    if(ffExitState != 0)
        pCommand = "avconv";
    else if(avExitState != 0)
        pCommand = "ffmpeg";*/
    convertion->processUsed();
    pCommand = convertion->process();
    ui->statusBar->showMessage("Using: " + pCommand, 10000);
}

void MainWindow::createComboBoxes()
{
    /*bitrateCB = new QComboBox(this);
    bitrateCB->addItems(QStringList() << "96k" << "128k" << "160k" << "192k" << "256k" << "320k");
    bitrateCB->setCurrentIndex(1);
    bitrateCB->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    ui->optionsHLayout->addWidget(bitrateCB);*/

    codecCB = new QComboBox(this);
    codecCB->addItems(QStringList() << "OGG" << "M4A" << "MP3");
    //codecCB->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    //ui->optionsHLayout->addWidget(codecCB);
    ui->mainToolBar->addWidget(codecCB);
    ui->mainToolBar->setMovable(false);

    /*rateCB = new QComboBox(this);
    rateCB->addItems(QStringList() << "48k" << "44k" << "32k");
    rateCB->setCurrentIndex(1);
    rateCB->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    ui->optionsHLayout->addWidget(rateCB);
    ui->optionsHLayout->addStretch();*/
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

void MainWindow::createActions()
{
    connect(ui->actionTarget_Folder, SIGNAL(triggered(bool)), this, SLOT(selectTargetFolder())); //select folder
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close())); //close window}
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about())); //about
    connect(ui->actionAbout_Qt, SIGNAL(triggered(bool)), this, SLOT(aboutQt())); //about qt
    connect(ui->actionAddFiles, SIGNAL(triggered(bool)), this, SLOT(addFiles())); //add files
    connect(ui->actionRemove_File, SIGNAL(triggered(bool)), this, SLOT(removeFile())); //remove file
    connect(ui->actionClear_Files, SIGNAL(triggered(bool)), this, SLOT(clearFiles())); //clear files
    connect(ui->actionConvert_Files, SIGNAL(triggered(bool)), this, SLOT(convertFiles())); //convert files
}


void MainWindow::addFormats()
{
    formats["OGG"] = "libvorbis";
    formats["M4A"] = "aac";
    formats["MP3"] = "libmp3lame";
}

void MainWindow::selectTargetFolder()
{
    targetFolder = QFileDialog::getExistingDirectory(this, "Lightning", QDir::homePath());
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
    QMessageBox::about(this, "About Lightning", tr("<h2>Lightning v0.3</h2>"
                                                   "<p>Issei / GX (isseigx@xmpp.jp), 2015</p>"
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
    QStringList args1;
    QFileInfoList finfo;
    QString cdc = formats[codecCB->currentText()];
    QString frate, fbit, fname, baseName;
    frate = rateCB->currentText();
    fbit = bitrateCB->currentText();
    int count = 0;
    convertPrB->setMaximum(files.size());
    for(int i = 0; i < files.size(); ++i)
        finfo << QFileInfo(files[i]);

    if(!files.isEmpty()) {
        convertPrB->setVisible(true);
        while(files.size() > 0) {
            baseName = finfo[0].fileName();
            fname = targetFolder + baseName.remove(finfo[0].suffix(), Qt::CaseInsensitive) + codecCB->currentText().toLower();
            args1 << "-i" << files[0] << "-c:a" << cdc << "-r:a" << frate << "-b:a"
                  << fbit << "-ac" << "2" << "-vn";
            if((cdc == "aac") & (pCommand == "ffmpeg"))
                args1 << "-strict" << "-2";
            args1 << fname;
            addedFilesLW->item(count)->setIcon(QIcon::fromTheme("media-playback-start"));
            ui->statusBar->showMessage("Converting: " + files[0]);
            converter->execute(pCommand, args1);
            addedFilesLW->item(count)->setIcon(QIcon::fromTheme("emblem-default"));
            files.removeFirst();
            finfo.removeFirst();
            args1.clear();
            ++count;
            qDebug() << count;
            convertPrB->setValue(count);

        }
        convertPrB->setVisible(false);
    }
}
