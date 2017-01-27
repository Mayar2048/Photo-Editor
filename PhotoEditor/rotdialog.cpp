#include "rotdialog.h"
#include "ui_rotdialog.h"
#include <iostream>

RotDialog::RotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RotDialog)
{
    ui->setupUi(this);
    ui->clockDir->setChecked(true);
    ui->valLineEdit->setPlaceholderText("0");
    ui->valLineEdit->setValidator(new QIntValidator(0, 360, this));
    direction = 0;
    angle = 0;
}

RotDialog::~RotDialog()
{
    delete ui;
}

void RotDialog::setClockwise(bool dir) {
    direction = dir;
    std::cout << direction << std::endl;
}


void RotDialog::setAnticlockwise(bool dir) {
    direction = !dir;
    std::cout << direction << std::endl;
}

void RotDialog::setAngle() {
    angle = ui->valLineEdit->text().toInt();
}

int RotDialog::getDirection() {
    return (direction == 0) ? 1 : -1;
}

int RotDialog::getAngle() {
    std::cout << angle << std::endl;
    return angle;
}
