/********************************************************************************
** Form generated from reading UI file 'rotdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROTDIALOG_H
#define UI_ROTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RotDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QVBoxLayout *direction;
    QLabel *dirLabel;
    QRadioButton *clockDir;
    QRadioButton *anticlockDir;
    QWidget *layoutWidget1;
    QVBoxLayout *value;
    QLabel *valLabel;
    QLineEdit *valLineEdit;

    void setupUi(QDialog *RotDialog)
    {
        if (RotDialog->objectName().isEmpty())
            RotDialog->setObjectName(QStringLiteral("RotDialog"));
        RotDialog->resize(364, 320);
        buttonBox = new QDialogButtonBox(RotDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(40, 270, 221, 41));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(RotDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(70, 30, 211, 71));
        direction = new QVBoxLayout(layoutWidget);
        direction->setObjectName(QStringLiteral("direction"));
        direction->setContentsMargins(0, 0, 0, 0);
        dirLabel = new QLabel(layoutWidget);
        dirLabel->setObjectName(QStringLiteral("dirLabel"));

        direction->addWidget(dirLabel);

        clockDir = new QRadioButton(layoutWidget);
        clockDir->setObjectName(QStringLiteral("clockDir"));

        direction->addWidget(clockDir);

        anticlockDir = new QRadioButton(layoutWidget);
        anticlockDir->setObjectName(QStringLiteral("anticlockDir"));

        direction->addWidget(anticlockDir);

        layoutWidget1 = new QWidget(RotDialog);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(70, 150, 211, 61));
        value = new QVBoxLayout(layoutWidget1);
        value->setObjectName(QStringLiteral("value"));
        value->setContentsMargins(0, 0, 0, 0);
        valLabel = new QLabel(layoutWidget1);
        valLabel->setObjectName(QStringLiteral("valLabel"));

        value->addWidget(valLabel);

        valLineEdit = new QLineEdit(layoutWidget1);
        valLineEdit->setObjectName(QStringLiteral("valLineEdit"));

        value->addWidget(valLineEdit);


        retranslateUi(RotDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), RotDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RotDialog, SLOT(reject()));
        QObject::connect(clockDir, SIGNAL(clicked(bool)), RotDialog, SLOT(setAnticlockwise(bool)));
        QObject::connect(anticlockDir, SIGNAL(clicked(bool)), RotDialog, SLOT(setClockwise(bool)));
        QObject::connect(buttonBox, SIGNAL(accepted()), RotDialog, SLOT(setAngle()));

        QMetaObject::connectSlotsByName(RotDialog);
    } // setupUi

    void retranslateUi(QDialog *RotDialog)
    {
        RotDialog->setWindowTitle(QApplication::translate("RotDialog", "Dialog", 0));
        dirLabel->setText(QApplication::translate("RotDialog", "Angle Direction", 0));
        clockDir->setText(QApplication::translate("RotDialog", "Clockwise", 0));
        anticlockDir->setText(QApplication::translate("RotDialog", "Anticlockwise", 0));
        valLabel->setText(QApplication::translate("RotDialog", "Angle Value", 0));
    } // retranslateUi

};

namespace Ui {
    class RotDialog: public Ui_RotDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROTDIALOG_H
