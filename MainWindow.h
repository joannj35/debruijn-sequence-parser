#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QComboBox>
#include <QRegularExpression>
#include <QMessageBox>
#include "BaseClass.h"
#include "DataDisplayWindow.h"

class MainWindow : public BaseClass
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    DataDisplayWindow* dataDisplayWindow;
    QGroupBox* formGroupBox;
    QComboBox* fieldComboBox;
    QComboBox* spanComboBox;
    QComboBox* complexityComboBox;

    void createFormGroupBox();
    int getSequenceData(int& totaldbSeq, int& totalSmallSeq, int& totalYielding, int& totalNonYielding, QString filePath);

private slots:
    void onSpanChanged(int index);
    void onFieldChanged(int index);
    void onNextButtonClicked();
    void populateComplexityComboBox(int span, int p);
    void populateSpanComboBox(int p);
};

#endif // MAINWINDOW_H
