#include "DataDisplayWindow.h"

DataDisplayWindow::DataDisplayWindow(QWidget* parent, int field, int span, int complexity, int totaldbSeq, int totalSmallSeq)
    : BaseClass(parent), field(field), span(span), complexity(complexity), totaldbSeq(totaldbSeq), totalSmallSeq(totalSmallSeq)
{
    // top label
    std::string str = "De-Bruijn Sequences\n Over Field F" + std::to_string(field)
        + " Span " + std::to_string(span)
        + " Complexity " + std::to_string(complexity);
    const char* label = str.c_str();

    topLabel->setText(tr(label));
    QFont labelFont = topLabel->font();
    labelFont.setPointSize(15);
    labelFont.setBold(true);
    topLabel->setFont(labelFont);
    topLabel->setAlignment(Qt::AlignCenter);

    // results group box
    createResultsGroupBox();

    // filter checkboxes
    createFiltersGroupBox();

    // small seq inspection groupbox
    createSmallSeqInspectGroupBox();
    connect(smallSequenceInput, &QTextEdit::textChanged, this, &DataDisplayWindow::validateText);
    connect(inspectButton, &QPushButton::clicked, this, &DataDisplayWindow::onInspectButtonClicked);

    // button box
    leftButton->setText(tr("Open File"));
    connect(leftButton, &QPushButton::clicked, this, &DataDisplayWindow::onOpenFileClicked);
    rightButton->setText(tr("Close"));
    connect(rightButton, &QPushButton::clicked, this, &DataDisplayWindow::reject);
    buttonBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // main layout
    mainLayout->addWidget(topLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(resultsGroupBox);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(filtersGroupBox);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(smallSeqInspectGroupBox);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(buttonBox);

    // main widget
    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    setLayout(mainLayout);
}

void DataDisplayWindow::createResultsGroupBox()
{
    QFormLayout* resultsLayout = new QFormLayout;
    resultsGroupBox = new QGroupBox(tr("Results"));

    // LABELS
    QLabel* labelTotalDbSeq = new QLabel(tr("Total number of de-Bruijn sequences is: %1").arg(totaldbSeq));
    QLabel* labelTotalSmallSeq = new QLabel(tr("Total number of small sequences is: %1").arg(totalSmallSeq));

    // LABEL FONT
    QFont groupBoxFont = resultsGroupBox->font();
    groupBoxFont.setPointSize(11);
    resultsGroupBox->setFont(groupBoxFont); // group box title
    labelTotalDbSeq->setFont(groupBoxFont);
    labelTotalSmallSeq->setFont(groupBoxFont);

    // LAYOUT
    resultsLayout->addRow(labelTotalDbSeq);
    resultsLayout->addRow(labelTotalSmallSeq);
    resultsGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    resultsGroupBox->setLayout(resultsLayout);
}

void DataDisplayWindow::createFiltersGroupBox()
{
    QFormLayout* filtersLayout = new QFormLayout;
    filtersGroupBox = new QGroupBox(tr("Filters"));

    // CHECK BOXES
    noFilterCheckbox = new QCheckBox(tr("No Filters Applied"), this);
    noFilterCheckbox->setToolTip(tr("When checked, this will open the original -unfiltered- file "));

    filterZerosCheckbox = new QCheckBox(tr("Zero Result Sequences Excluded"), this);
    filterZerosCheckbox->setToolTip(tr("When checked, this will exclude small sequences that do not result any de-Bruijn sequences from the analysis."));

    filterNoneZerosCheckbox = new QCheckBox(tr("Non-Zero Result Sequences Excluded"), this);
    filterNoneZerosCheckbox->setToolTip(tr("When checked, this will exclude small sequences that result in de-Bruijn sequences from the analysis."));

    filterSmallSeqCheckbox = new QCheckBox(tr("Small Sequences Only"), this);
    filterSmallSeqCheckbox->setToolTip(tr("When checked, this will open the small sequences file"));

    // LABEL FONT
    QFont groupBoxFont = filtersGroupBox->font();
    groupBoxFont.setPointSize(11);
    filtersGroupBox->setFont(groupBoxFont); // group box title
    noFilterCheckbox->setFont(groupBoxFont);
    filterZerosCheckbox->setFont(groupBoxFont);
    filterNoneZerosCheckbox->setFont(groupBoxFont);
    filterSmallSeqCheckbox->setFont(groupBoxFont);

    // LAYOUT
    filtersLayout->addRow(noFilterCheckbox);
    filtersLayout->addRow(filterZerosCheckbox);
    filtersLayout->addRow(filterNoneZerosCheckbox);
    filtersLayout->addRow(filterSmallSeqCheckbox);
    filtersGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    filtersGroupBox->setLayout(filtersLayout);
}

void DataDisplayWindow::createSmallSeqInspectGroupBox()
{
    QVBoxLayout* groupBoxLayout = new QVBoxLayout;
    QLabel* label = new QLabel(tr("Enter small sequences that you would like to inspect, separated by a new line: "));

    smallSeqInspectGroupBox = new QGroupBox(tr("Small Sequence Inspection"));

    // TEXT EDIT
    smallSequenceInput = new QTextEdit(this);
    smallSequenceInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // LABEL FONT
    QFont groupBoxFont = smallSeqInspectGroupBox->font();
    groupBoxFont.setPointSize(11);
    smallSeqInspectGroupBox->setFont(groupBoxFont); // group box title
    label->setFont(groupBoxFont);
    smallSequenceInput->setFont(groupBoxFont);

    // BUTTON
    inspectButton = new QPushButton(tr("Inspect"), this);
    inspectButton->setStyleSheet(
        "QPushButton { "
        "  background-color: #2196F3; " // Vibrant blue
        "  color: white; " // text color
        "  border-radius: 4px; " // Rounded corners
        "  font-weight: bold; "
        "  border: none; "
        "  padding: 5px 15px; "
        "} "

        "QPushButton:hover { "
        "  background-color: #1E88E5; " // dark blue
        "} "

        "QPushButton:pressed { "
        "  background-color: #1565C0; " // dark blue
        "} "

        "QDialogButtonBox > QPushButton { "
        "  min-width: 50px; " // Minimum width for both buttons
        "}"
    );


    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(inspectButton);
    buttonLayout->addStretch(); // align button to the left

    // LAYOUT
    groupBoxLayout->addWidget(label);
    groupBoxLayout->addWidget(smallSequenceInput);
    groupBoxLayout->addLayout(buttonLayout); // Add the horizontal layout containing the button
    smallSeqInspectGroupBox->setLayout(groupBoxLayout);
}

void DataDisplayWindow::onOpenFileClicked()
{
    bool isNoFilterChecked = noFilterCheckbox->isChecked();
    bool isFilterZerosChecked = filterZerosCheckbox->isChecked();
    bool isFilterNonZerosChecked = filterNoneZerosCheckbox->isChecked();
    bool isFilterSmallSeqChecked = filterSmallSeqCheckbox->isChecked();
    QString fileName = "";

    if (isNoFilterChecked) { // No filters applied
        fileName = QString(":/results/data/F_%1/span_%2/%3/field_%1_span_%2_complexity_%3.txt").arg(field).arg(span).arg(complexity);
        openFileForUser(fileName);
    }

    if (isFilterZerosChecked) { // Zero Result Sequences Excluded
        fileName = QString(":/results/data/F_%1/span_%2/%3/%3_zeros_excluded.txt").arg(field).arg(span).arg(complexity);
        openFileForUser(fileName);
    }

    if (isFilterNonZerosChecked) { // Non-Zero Result Sequences Excluded
        fileName = QString(":/results/data/F_%1/span_%2/%3/%3_non_zero_excluded.txt").arg(field).arg(span).arg(complexity);
        openFileForUser(fileName);
    }

    if (isFilterSmallSeqChecked) { // Small Sequences Only
        if (field != 2)
            QMessageBox::information(this, tr(""), tr("This action is applicable for binary fields only"));
        else
        {
            int offset = complexity - qPow(field, span - 1);
            fileName = QString("/data/F_%1/smallSequences/sequences_of_complexity_%2.txt").arg(field).arg(offset);
            openFileForUser(fileName);
        }
    }
}

void DataDisplayWindow::openFileForUser(const QString& filePath)
{ 
    QString appDir = QCoreApplication::applicationDirPath();

#ifdef _DEBUG // go up two directories from the Release/Debug folder
    appDir = QDir(appDir).absoluteFilePath("../../"); 
#endif

    QString localFilePath = appDir + filePath;
    QUrl fileUrl = QUrl::fromLocalFile(localFilePath);

    if (!QDesktopServices::openUrl(fileUrl)) {
        QMessageBox::warning(this, tr("File Open Error"), tr("Unable to open file: ") + localFilePath);
    }
}

void DataDisplayWindow::validateText()
{
    QRegularExpression re("^(\\d+\n)*\\d+$");
    QString text = smallSequenceInput->toPlainText();
    QRegularExpressionMatch match = re.match(text);
    if (!match.hasMatch()) {
        QMessageBox::warning(this, tr("Invalid Text Detected"), tr("Only digits and new lines are allowed. Each line must contain only digits."));
    }
}

void DataDisplayWindow::onInspectButtonClicked()
{
    QString userInput = smallSequenceInput->toPlainText();
    QStringList smallSequences = userInput.split('\n', Qt::SkipEmptyParts);

    QString ReadFilePath = QString(":/results/data/F_%1/span_%2/%3/field_%1_span_%2_complexity_%3.txt").arg(field).arg(span).arg(complexity);

    QFile ReadFile(ReadFilePath);
    if (!ReadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file for reading."));
        return;
    }

    QFile fileOut("inspectionOutput.txt");
    if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot create or open inspectionOutput file for writing."));
        return;
    }

    QTextStream in(&ReadFile);
    QString line;
    bool isSequenceOfInterest = false; // Flag to track if we're within a section of interest

    QTextStream out(&fileOut);

    while (in.readLineInto(&line)) {
        if (line.startsWith("Debruijn Sequences generated by the sub sequence ")) {
            // Extract the sequence
            QString currentSeq = line.section(' ', 7, 7).trimmed();

            // Check if the sequence was provided by the user
            if (smallSequences.contains(currentSeq)) {
                isSequenceOfInterest = true;
                out << line << "\n";
            }
            else {
                isSequenceOfInterest = false;
            }
        }
        else if (isSequenceOfInterest) {
            if (!line.trimmed().isEmpty()) {
                // Continue writing to the file until a blank line is encountered
                out << line << "\n";
            }
            else {
                // Once a blank line is encountered, stop writing and reset the flag
                out << "\n";
                isSequenceOfInterest = false;
            }
        }
    }

    fileOut.close();
    QMessageBox::information(this, tr("Success"), tr("Inspection complete. Click OK to open the inspection result file."));
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + "/inspectionOutput.txt"));
}
