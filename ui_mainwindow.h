/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionRemove_File;
    QAction *actionClear_Files;
    QAction *actionConvert_Files;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *miscHLayout;
    QVBoxLayout *filesVLayout;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(600, 400);
        QIcon icon;
        QString iconThemeName = QStringLiteral("lightningmc");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        MainWindow->setWindowIcon(icon);
        actionRemove_File = new QAction(MainWindow);
        actionRemove_File->setObjectName(QStringLiteral("actionRemove_File"));
        QIcon icon1;
        iconThemeName = QStringLiteral("remove");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionRemove_File->setIcon(icon1);
        actionClear_Files = new QAction(MainWindow);
        actionClear_Files->setObjectName(QStringLiteral("actionClear_Files"));
        QIcon icon2;
        iconThemeName = QStringLiteral("edit-clear");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionClear_Files->setIcon(icon2);
        actionConvert_Files = new QAction(MainWindow);
        actionConvert_Files->setObjectName(QStringLiteral("actionConvert_Files"));
        QIcon icon3;
        iconThemeName = QStringLiteral("view-refresh");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon3 = QIcon::fromTheme(iconThemeName);
        } else {
            icon3.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionConvert_Files->setIcon(icon3);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        miscHLayout = new QHBoxLayout();
        miscHLayout->setSpacing(6);
        miscHLayout->setObjectName(QStringLiteral("miscHLayout"));

        gridLayout->addLayout(miscHLayout, 2, 0, 1, 1);

        filesVLayout = new QVBoxLayout();
        filesVLayout->setSpacing(6);
        filesVLayout->setObjectName(QStringLiteral("filesVLayout"));

        gridLayout->addLayout(filesVLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actionRemove_File);
        mainToolBar->addAction(actionClear_Files);
        mainToolBar->addAction(actionConvert_Files);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Lightning", 0));
        actionRemove_File->setText(QApplication::translate("MainWindow", "Remove File", 0));
#ifndef QT_NO_TOOLTIP
        actionRemove_File->setToolTip(QApplication::translate("MainWindow", "Remove Selected File", 0));
#endif // QT_NO_TOOLTIP
        actionRemove_File->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0));
        actionClear_Files->setText(QApplication::translate("MainWindow", "Clear Files", 0));
#ifndef QT_NO_TOOLTIP
        actionClear_Files->setToolTip(QApplication::translate("MainWindow", "Clear File List", 0));
#endif // QT_NO_TOOLTIP
        actionClear_Files->setShortcut(QApplication::translate("MainWindow", "Alt+Shift+C", 0));
        actionConvert_Files->setText(QApplication::translate("MainWindow", "Convert Files", 0));
#ifndef QT_NO_TOOLTIP
        actionConvert_Files->setToolTip(QApplication::translate("MainWindow", "Convert Files", 0));
#endif // QT_NO_TOOLTIP
        actionConvert_Files->setShortcut(QApplication::translate("MainWindow", "Alt+Shift+C", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
