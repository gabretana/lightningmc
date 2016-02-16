#ifndef CODECCONFIGDIALOG_H
#define CODECCONFIGDIALOG_H

#include <QDialog>

class QComboBox;
class QLabel;
class QPushButton;

namespace Ui {
class CodecConfigDialog;
}

class CodecConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CodecConfigDialog(QWidget *parent = 0);
    ~CodecConfigDialog();
    void setCurrentConfig(QString cbitrate, QString crate);

signals:
    void accepted(QString bitrate, QString rate);

private slots:
    void acceptValues();

private:
    Ui::CodecConfigDialog *ui;

    void addButtons();
    void addComboBoxes();
    void addLabels();
    void createActions();

    QPushButton *okBt, *cancelBt;
    QLabel *bitrateLb, *rateLb;
    QComboBox *bitrateCb, *rateCb;
};

#endif // CODECCONFIGDIALOG_H
