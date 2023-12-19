#pragma once

#ifndef DATADISPLAYWINDOW_H
#define DATADISPLAYWINDOW_H

#include "BaseClass.h"
#include <QDialog>
#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>
#include <QtMath>
#include <QPixmap>
#include <QVBoxLayout>
#include <QLabel>
#include <QRegularExpressionValidator>

class DataDisplayWindow : public BaseClass
{

public:
    explicit DataDisplayWindow(QWidget* parent, int field, int span, int complexity, int totaldbSeq, int totalSmallSeq);

private:
    int field;
    int span;
    int complexity;
    int totalSmallSeq;
    int totaldbSeq;
    QTextEdit* smallSequenceInput;
    QPushButton* inspectButton;
    QCheckBox* filterZerosCheckbox;
    QCheckBox* filterNoneZerosCheckbox;
    QCheckBox* filterSmallSeqCheckbox;
    QCheckBox* noFilterCheckbox;
    QGroupBox* filtersGroupBox;
    QGroupBox* resultsGroupBox;
    QGroupBox* smallSeqInspectGroupBox;

    void openFileForUser(const QString& filePath);
    void createResultsGroupBox();
    void createFiltersGroupBox();
    void createSmallSeqInspectGroupBox();

private slots:
    void onOpenFileClicked();
    void validateText();
    void onInspectButtonClicked();
};

#endif // DATADISPLAYWINDOW_H


