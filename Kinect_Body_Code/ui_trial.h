/********************************************************************************
** Form generated from reading UI file 'trial.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIAL_H
#define UI_TRIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_trialClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *trialClass)
    {
        if (trialClass->objectName().isEmpty())
            trialClass->setObjectName(QStringLiteral("trialClass"));
        trialClass->resize(600, 400);
        menuBar = new QMenuBar(trialClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        trialClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(trialClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        trialClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(trialClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        trialClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(trialClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        trialClass->setStatusBar(statusBar);

        retranslateUi(trialClass);

        QMetaObject::connectSlotsByName(trialClass);
    } // setupUi

    void retranslateUi(QMainWindow *trialClass)
    {
        trialClass->setWindowTitle(QApplication::translate("trialClass", "trial", 0));
    } // retranslateUi

};

namespace Ui {
    class trialClass: public Ui_trialClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIAL_H
