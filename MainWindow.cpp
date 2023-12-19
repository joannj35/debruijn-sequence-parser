#include "mainWindow.h"
#define ERR -1

MainWindow::MainWindow(QWidget* parent)
    : BaseClass(parent)
{
    // top label
    topLabel->setText(tr("Debruijn Sequence Parser"));
    QFont labelFont = topLabel->font();
    labelFont.setPointSize(30);
    labelFont.setBold(true);
    topLabel->setFont(labelFont);
    topLabel->setAlignment(Qt::AlignCenter);

    // form group box
    createFormGroupBox();
    connect(fieldComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFieldChanged(int)));
    connect(spanComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSpanChanged(int)));
    formGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // button box
    leftButton->setText(tr("Next"));
    connect(leftButton, &QPushButton::clicked, this, &MainWindow::onNextButtonClicked);
    rightButton->setText(tr("Close"));
    connect(rightButton, &QPushButton::clicked, this, &MainWindow::reject);
    buttonBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // main layout
    mainLayout->addWidget(topLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(buttonBox);

    // main widget
    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    setLayout(mainLayout);
}


void MainWindow::createFormGroupBox()
{
    QFormLayout* layout = new QFormLayout;
    formGroupBox = new QGroupBox(tr("Sequence Information"));

    // LABELS
    QLabel* orderLabel = new QLabel(tr("Over Field: "));
    QLabel* spanLabel = new QLabel(tr("Span: "));
    QLabel* complexityLabel = new QLabel(tr("Complexity:"));

    // LABEL FONT
    QFont groupBoxFont = formGroupBox->font();
    groupBoxFont.setPointSize(11);
    formGroupBox->setFont(groupBoxFont); // group boxs title
    orderLabel->setFont(groupBoxFont);
    spanLabel->setFont(groupBoxFont);
    complexityLabel->setFont(groupBoxFont);

    //COMBO BOXES
    fieldComboBox = new QComboBox;
    spanComboBox = new QComboBox;
    complexityComboBox = new QComboBox;

    fieldComboBox->addItem("<select>", QVariant(0));
    fieldComboBox->addItem("2", QVariant(2));
    for (int i = 3; i <= 7; i += 2)
        fieldComboBox->addItem(QString::number(i), QVariant(i));

    // LAYOUT
    layout->addRow(orderLabel, fieldComboBox);
    layout->addRow(spanLabel, spanComboBox);
    layout->addRow(complexityLabel, complexityComboBox);
    formGroupBox->setLayout(layout);
}

void MainWindow::onFieldChanged(int index) {
    spanComboBox->clear();
    complexityComboBox->clear();
    if (index != -1) {

        if (fieldComboBox->itemText(index) == "<select>")
            return; // don't populate ComboBox if placeholder is selected

        int p = fieldComboBox->itemData(index).toInt();
        populateSpanComboBox(p);
    }
}

void MainWindow::populateSpanComboBox(int p) {
    if (p != 2) //non-binary field
    {
        spanComboBox->addItem("2", QVariant(2));
    }
    else { //binary field
        spanComboBox->addItem("<select>", QVariant(0));
        spanComboBox->addItem("6", QVariant(6));
        spanComboBox->addItem("7", QVariant(7));
    }
}

void MainWindow::onSpanChanged(int index) {
    if (index != -1) {
        complexityComboBox->clear();
        if (spanComboBox->itemText(index) == "<select>") {
            return; // don't populate ComboBox if placeholder is selected
        }
        int span = spanComboBox->itemData(spanComboBox->currentIndex()).toInt();
        int p = fieldComboBox->itemData(fieldComboBox->currentIndex()).toInt();
        populateComplexityComboBox(span, p);
    }
}

//in the case of non-bianry field p: span is only 2 and complexities are 2p+1 , 2p+2
//in the case of binary field: span is 6 or 7 and complexities are p^(span-1)
void MainWindow::populateComplexityComboBox(int span, int p) {
    if (span == 2) // non binary
    {
        complexityComboBox->addItem("<select>", QVariant(0));
        complexityComboBox->addItem(QString::number(2 * p + 1), QVariant(2 * p + 1));
        complexityComboBox->addItem(QString::number(2 * p + 2), QVariant(2 * p + 2));
        return;
    }

    //binary
    int i = qPow(p, span - 1) + span;
    complexityComboBox->addItem("<select>", QVariant(0));
    for (; i <= qPow(p, span) - 1; ++i)
        complexityComboBox->addItem(QString::number(i), QVariant(i));
}

void MainWindow::onNextButtonClicked() {
    int field = fieldComboBox->currentData().toInt();
    int span = spanComboBox->currentData().toInt();
    int complexity = complexityComboBox->currentData().toInt();
    int totaldbSeq = -1;
    int totalSmallSeq = -1;

    // Check if any of the values is the placeholder '<select>'
    if (field == 0 || span == 0 || complexity == 0) {
        QMessageBox::warning(this, tr("Incomplete Selection Detected"), tr("All fields must be selected prior to proceeding."));
        return;
    }

    // fetch corresponding file
    QString fileName = QString("field_%1_span_%2_complexity_%3.txt").arg(field).arg(span).arg(complexity);
    QString complexityDirectoryPath = QString(":/results/data/F_%1/span_%2/%3/").arg(field).arg(span).arg(complexity);
    QString filePath = complexityDirectoryPath + fileName;

    int returnVal = getSequenceData(totaldbSeq, totalSmallSeq, filePath);
    if (returnVal == ERR)
        return;

    DataDisplayWindow* dataDisplayWindow = new DataDisplayWindow(this, field, span, complexity, totaldbSeq, totalSmallSeq);
    dataDisplayWindow->setAttribute(Qt::WA_DeleteOnClose);
    dataDisplayWindow->show();
}

int MainWindow::getSequenceData(int& totaldbSeq, int& totalSmallSeq, QString filePath)
{
    static QRegularExpression dbSeqRegex(R"(total number of sequences of complexity \d+ is: (\d+))");
    static QRegularExpression smallSeqRegex(R"(number of subsequences with complexity \d+ is: (\d+))");

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("File Open Error"), tr("Unable to open the file for reading."));
        return ERR;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        // Check for the dbSeq line
        QRegularExpressionMatch dbSeqMatch = dbSeqRegex.match(line);
        if (dbSeqMatch.hasMatch()) {
            totaldbSeq = dbSeqMatch.captured(1).toInt();
        }

        // Check for the smallSeq line
        QRegularExpressionMatch smallSeqMatch = smallSeqRegex.match(line);
        if (smallSeqMatch.hasMatch()) {
            totalSmallSeq = smallSeqMatch.captured(1).toInt();
        }
    }

    file.close();
    return 0;
}
