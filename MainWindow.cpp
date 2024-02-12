#include "mainWindow.h"
#define ERR -1

MainWindow::MainWindow(QWidget* parent)
    : BaseClass(parent)
{
    // top label
    topLabel->setText(tr("de Bruijn Sequence Parser"));
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

    // summary overview group box
    createSummaryGroupBox();
    summaryGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // button box
    leftButton->setText(tr("Next"));
    openExcelButton->setToolTip(tr("Click to proceed to the next page"));
    connect(leftButton, &QPushButton::clicked, this, &MainWindow::onNextButtonClicked);
    rightButton->setText(tr("Exit"));
    openExcelButton->setToolTip(tr("Click to quit the application"));
    connect(rightButton, &QPushButton::clicked, this, &MainWindow::reject);
    buttonBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // main layout
    mainLayout->addWidget(topLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(summaryGroupBox);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(buttonBox);

    // main widget
    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    setLayout(mainLayout);
}


void MainWindow::createFormGroupBox()
{
    QFormLayout* layout = new QFormLayout;
    formGroupBox = new QGroupBox(tr("Load Sequence Data"));

    // LABELS
    QLabel* orderLabel = new QLabel(tr("Over Field: "));
    QLabel* spanLabel = new QLabel(tr("Span: "));
    QLabel* complexityLabel = new QLabel(tr("Complexity:"));

    // LABEL FONT
    QFont groupBoxFont = formGroupBox->font();
    groupBoxFont.setPointSize(11);
    formGroupBox->setFont(groupBoxFont); // group box title
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

void MainWindow::createSummaryGroupBox()
{
    QFormLayout* layout = new QFormLayout;
    summaryGroupBox = new QGroupBox(tr("Data Overview"));

    // LABEL 
    QLabel* label = new QLabel(tr("A summary Excel sheet, which provides a comprehensive overview of the resulting data"));

    // LABEL FONT
    QFont groupBoxFont = summaryGroupBox->font();
    groupBoxFont.setPointSize(11);
    summaryGroupBox->setFont(groupBoxFont); // group box title
    label->setFont(groupBoxFont);

    //BUTTON BOX
    summarybuttonBox = new QDialogButtonBox(this);
    openExcelButton = new QPushButton(tr("Open File"));
    openExcelButton->setToolTip(tr("Click to open the summary Excel sheet"));
    connect(openExcelButton, &QPushButton::clicked, this, &MainWindow::onOpenExcelButtonClicked);
    summarybuttonBox->addButton(openExcelButton, QDialogButtonBox::NoRole);
    summarybuttonBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // LAYOUT
    layout->addRow(label);
    layout->addRow(summarybuttonBox);
    summaryGroupBox->setLayout(layout);
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

void MainWindow::populateComplexityComboBox(int span, int p) {
    complexityComboBox->addItem("<select>", QVariant(0));

    if (p != 2) // non binary
    {
        for (int i = 2*p + 1; i < qPow(p, span); ++i) {
            if (p == 7 && i > 25) //for f7: support complexities up until 25
                break;
            complexityComboBox->addItem(QString::number(i), QVariant(i));
        }
        return;
    }

    //binary
    for (int i = qPow(p, span - 1) + span; i <= qPow(p, span - 1) + 24; ++i) // i <= qPow(p, span) - 1 for all possible complexities
        complexityComboBox->addItem(QString::number(i), QVariant(i));
}

void MainWindow::onNextButtonClicked() {
    int field = fieldComboBox->currentData().toInt();
    int span = spanComboBox->currentData().toInt();
    int complexity = complexityComboBox->currentData().toInt();
    int totaldbSeq = -1;
    int totalSmallSeq = -1;
    int totalYielding = -1;
    int totalNonYielding = -1;

    // Check if any of the values is the placeholder '<select>'
    if (field == 0 || span == 0 || complexity == 0) {
        QMessageBox::warning(this, tr("Incomplete Selection Detected"), tr("All fields must be selected before proceeding."));
        return;
    }

    // fetch corresponding file
    QString filePath = QString(":/summary/data/F_%1/span_%2/%3/%3_summary.txt").arg(field).arg(span).arg(complexity);
    
    int returnVal = getSequenceData(totaldbSeq, totalSmallSeq, totalYielding, totalNonYielding, filePath);
    if (returnVal == ERR)
        return;

    DataDisplayWindow* dataDisplayWindow = new DataDisplayWindow(this, field, span, complexity, totaldbSeq, totalSmallSeq, totalYielding, totalNonYielding);
    dataDisplayWindow->setAttribute(Qt::WA_DeleteOnClose);
    dataDisplayWindow->show();
}

void MainWindow::onOpenExcelButtonClicked()
{
    
    QString appDir = QCoreApplication::applicationDirPath();

#ifdef _DEBUG // go up two directories from the Release/Debug folder
    appDir = QDir(appDir).absoluteFilePath("../../");
#endif

    QString filePath = "/data/summary.xlsx";
    QString localFilePath = appDir + filePath; 
    QUrl fileUrl = QUrl::fromLocalFile(localFilePath);

    if (!QDesktopServices::openUrl(fileUrl)) {
        QMessageBox::warning(this, tr("File Open Error"), tr("Unable to open file: ") + localFilePath);
    }
}


int MainWindow::getSequenceData(int& totaldbSeq, int& totalSmallSeq, int& totalYielding, int& totalNonYielding, QString filePath)
{
    static QRegularExpression dbSeqRegex(R"(total number of debruijn sequences of complexity \d+ is: (\d+))");
    static QRegularExpression smallSeqRegex(R"(total number of sequences of small complexity \d+ is: (\d+))");
    static QRegularExpression yieldingSeqRegex(R"(The total number of sequences of small complexity=\d+ which yield debruijn sequences is: (\d+))");
    static QRegularExpression nonYieldingSeqRegex(R"(The total number of sequences of small complexity=\d+ which DO NOT yield any debruijn sequences is: (\d+))");


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

        // Check for yielding line
        QRegularExpressionMatch yieldingSeqMatch = yieldingSeqRegex.match(line);
        if (yieldingSeqMatch.hasMatch()) {
            totalYielding = yieldingSeqMatch.captured(1).toInt();
        }

        // Check for non yielding line
        QRegularExpressionMatch nonYieldingSeqMatch = nonYieldingSeqRegex.match(line);
        if (nonYieldingSeqMatch.hasMatch()) {
            totalNonYielding = nonYieldingSeqMatch.captured(1).toInt();
        }
    }

    file.close();
    return 0;
}
