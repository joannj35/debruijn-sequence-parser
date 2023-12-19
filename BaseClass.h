#pragma once

#ifndef BASECLASS_H
#define BASECLASS_H

#include <QDialog>
#include <QLabel>
#include <QCoreApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QMenuBar>

class BaseClass : public QDialog
{
    Q_OBJECT

public:
    explicit BaseClass(QWidget* parent = nullptr);

protected:
    void windowSetup();

    QVBoxLayout* mainLayout;
    QMenu* appMenu;
    QMenuBar* menuBar;
    QDialogButtonBox* buttonBox;
    QPushButton* leftButton;
    QPushButton* rightButton;
    QLabel* topLabel;
    QLabel* logoLabel;
};

#endif // BASECLASS_H


