#include "BaseClass.h"

BaseClass::BaseClass(QWidget* parent) : QDialog(parent)
{
    windowSetup();
    mainLayout = new QVBoxLayout(this);

    // top label
    topLabel = new QLabel(this);
    mainLayout->addWidget(topLabel);

    // logo label
    logoLabel = new QLabel(this);
    mainLayout->addWidget(logoLabel);

    // button box
    buttonBox = new QDialogButtonBox(this);
    leftButton = new QPushButton(this);
    rightButton = new QPushButton(this);
    buttonBox->addButton(leftButton, QDialogButtonBox::NoRole); // derived classes choose role
    buttonBox->addButton(rightButton, QDialogButtonBox::NoRole);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

void BaseClass::windowSetup()
{
    QPixmap logoPixmap(":/images/logo.png");
    setWindowIcon(QIcon(logoPixmap));
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    setWindowTitle("DSP");
}
