#include "codecconfigdialog.h"
#include "ui_codecconfigdialog.h"

#include <QtWidgets>

CodecConfigDialog::CodecConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodecConfigDialog)
{
    ui->setupUi(this);

    addLabels();
    addComboBoxes();
    addButtons();
    createActions();
}

CodecConfigDialog::~CodecConfigDialog()
{
    delete ui;
}

void CodecConfigDialog::addLabels()
{
    bitrateLb = new QLabel(tr("Bitrate"), this);
    bitrateLb->setObjectName("bitrateLb");
    rateLb = new QLabel(tr("Rate"), this);
    rateLb->setObjectName("rateLb");

    ui->bitrateHLayout->addWidget(bitrateLb);
    ui->rateHLayout->addWidget(rateLb);
}

void CodecConfigDialog::addComboBoxes()
{
    bitrateCb = new QComboBox(this);
    bitrateCb->addItems(QStringList() << "96k" << "128k" << "160k" << "192k" << "256k" << "320k");
    bitrateCb->setObjectName("bitrateCb");
    bitrateCb->setCurrentIndex(2);
    bitrateCb->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    rateCb = new QComboBox(this);
    rateCb->addItems(QStringList() << "48k" << "44k" << "32k");
    rateCb->setObjectName("rateCb");
    rateCb->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    ui->bitrateHLayout->addWidget(bitrateCb);
    ui->rateHLayout->addWidget(rateCb);
}

void CodecConfigDialog::addButtons()
{
    okBt = new QPushButton(tr("OK"), this);
    okBt->setObjectName("okBt");

    cancelBt = new QPushButton(tr("Cancel"), this);
    cancelBt->setObjectName("cancelBt");

    ui->buttonsHLayout->addWidget(cancelBt);
    ui->buttonsHLayout->addWidget(okBt);
}

void CodecConfigDialog::createActions()
{
    connect(cancelBt, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(okBt, SIGNAL(clicked(bool)), this, SLOT(acceptValues()));
}

void CodecConfigDialog::acceptValues()
{
    emit accepted(bitrateCb->currentText(), rateCb->currentText());
    close();
}

void CodecConfigDialog::setCurrentConfig(QString cbitrate, QString crate)
{
    bitrateCb->setCurrentText(cbitrate);
    rateCb->setCurrentText(crate);
}
