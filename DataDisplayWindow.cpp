#include "DataDisplayWindow.h"

DataDisplayWindow::DataDisplayWindow(QWidget* parent, int field, int span, int complexity, int totaldbSeq, int totalSmallSeq, int totalYielding, int totalNonYielding)
	: BaseClass(parent), field(field), span(span), complexity(complexity), totaldbSeq(totaldbSeq), totalSmallSeq(totalSmallSeq), totalYielding(totalYielding), totalNonYielding(totalNonYielding)
{
	// top label
	std::string str = "de-Bruijn Sequences\n Over Field F" + std::to_string(field)
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
	leftButton->setText(tr("Load Files"));
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
	mainLayout->addSpacing(20);
	mainLayout->addWidget(buttonBox);

	// main widget
	QWidget* mainWidget = new QWidget(this);
	mainWidget->setLayout(mainLayout);
	setLayout(mainLayout);
}

void DataDisplayWindow::createResultsGroupBox()
{
	QFormLayout* resultsLayout = new QFormLayout;
	resultsGroupBox = new QGroupBox(tr("Results Overview"));

	// LABELS
	QLabel* labelTotalDbSeq = new QLabel(tr("Total number of de Bruijn sequences is: %1").arg(totaldbSeq));
	QLabel* labelTotalSmallSeq = new QLabel(tr("Total number of sequences of small complexity is: %1").arg(totalSmallSeq));
	QLabel* labelTotalYielding = new QLabel(tr("Total number of sequences of small complexity which yielded de Bruijn sequences is : %1").arg(totalYielding));
	QLabel* labelTotalNonYielding = new QLabel(tr("Total number of sequences of small complexity which DIDN'T yield de Bruijn sequences is : %1").arg(totalNonYielding));

	// LABEL FONT
	QFont groupBoxFont = resultsGroupBox->font();
	groupBoxFont.setPointSize(11);
	resultsGroupBox->setFont(groupBoxFont); // group box title
	labelTotalDbSeq->setFont(groupBoxFont);
	labelTotalSmallSeq->setFont(groupBoxFont);
	labelTotalYielding->setFont(groupBoxFont);
	labelTotalNonYielding->setFont(groupBoxFont);

	// LAYOUT
	resultsLayout->addRow(labelTotalDbSeq);
	resultsLayout->addRow(labelTotalSmallSeq);
	resultsLayout->addRow(labelTotalYielding);
	resultsLayout->addRow(labelTotalNonYielding);
	resultsGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	resultsGroupBox->setLayout(resultsLayout);
}

void DataDisplayWindow::createFiltersGroupBox()
{
	QFormLayout* filtersLayout = new QFormLayout;
	filtersGroupBox = new QGroupBox(tr("Filters"));

	// CHECK BOXES
	noFilterCheckbox = new QCheckBox(tr("Original File"), this);
	noFilterCheckbox->setToolTip(tr("Select to open the original file"));

	yieldingCheckbox = new QCheckBox(tr("Yielding Sequences Only"), this);
	yieldingCheckbox->setToolTip(tr("Select to show the small sequences & their yeilded de-Bruijn sequences"));

	yieldingSmallCheckbox = new QCheckBox(tr("Yielding Small Sequences Only"), this);
	yieldingSmallCheckbox->setToolTip(tr("Select to show sequences of small complexity which yield any de-Bruijn sequences"));

	NonYieldingCheckbox = new QCheckBox(tr("Non-Yielding Small Sequences Only"), this);
	NonYieldingCheckbox->setToolTip(tr("Select to show sequences of small complexity which do not yield de-Bruijn sequences"));

	filterSmallSeqCheckbox = new QCheckBox(tr("Sequences Of Small Complexity Only"), this);
	filterSmallSeqCheckbox->setToolTip(tr("Select to open the file containing the sequences of small complexity of which we generate the de-Bruijn sequences"));

	// LABEL FONT
	QFont groupBoxFont = filtersGroupBox->font();
	groupBoxFont.setPointSize(11);
	filtersGroupBox->setFont(groupBoxFont); // group box title
	noFilterCheckbox->setFont(groupBoxFont);
	yieldingCheckbox->setFont(groupBoxFont);
	NonYieldingCheckbox->setFont(groupBoxFont);
	filterSmallSeqCheckbox->setFont(groupBoxFont);

	// LAYOUT
	filtersLayout->addRow(noFilterCheckbox);
	filtersLayout->addRow(yieldingCheckbox);
	filtersLayout->addRow(NonYieldingCheckbox);
	filtersLayout->addRow(filterSmallSeqCheckbox);
	filtersGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	filtersGroupBox->setLayout(filtersLayout);
}

void DataDisplayWindow::createSmallSeqInspectGroupBox()
{
	QVBoxLayout* groupBoxLayout = new QVBoxLayout;
	QLabel* label = new QLabel(tr("Enter the sequences of small complexity which you would like to inspect, separated by a new line: "));

	smallSeqInspectGroupBox = new QGroupBox(tr("Sequence Inspection"));

	// TEXT EDIT
	smallSequenceInput = new QTextEdit(this);
	smallSequenceInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// CHECKBOX
	inspectSeparatelyCheckbox = new QCheckBox(tr("Extract to separate files"), this);
	inspectSeparatelyCheckbox->setToolTip(tr("Check to save each small sequence result in a separate file"));

	// LABEL FONT
	QFont groupBoxFont = smallSeqInspectGroupBox->font();
	groupBoxFont.setPointSize(11);
	smallSeqInspectGroupBox->setFont(groupBoxFont); // group box title
	label->setFont(groupBoxFont);
	inspectSeparatelyCheckbox->setFont(groupBoxFont);
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
	groupBoxLayout->addWidget(inspectSeparatelyCheckbox);
	groupBoxLayout->addLayout(buttonLayout); // Add the horizontal layout containing the button & checkbox
	smallSeqInspectGroupBox->setLayout(groupBoxLayout);
}

void DataDisplayWindow::onOpenFileClicked()
{
	bool isNoFilterChecked = noFilterCheckbox->isChecked();
	bool NonYieldingChecked = NonYieldingCheckbox->isChecked();
	bool yieldingChecked = yieldingCheckbox->isChecked();
	bool smallSeqChecked = filterSmallSeqCheckbox->isChecked();
	QString fileName = "";

	if (isNoFilterChecked) {
		fileName = QString("/data/F_%1/span_%2/%3/field_%1_span_%2_complexity_%3.txt").arg(field).arg(span).arg(complexity);
		openFileForUser(fileName);
		noFilterCheckbox->setCheckState(Qt::Unchecked);
	}

	if (NonYieldingChecked) {
		fileName = QString("/data/F_%1/span_%2/%3/%3_non_yielding_small_sequences.txt").arg(field).arg(span).arg(complexity);
		openFileForUser(fileName);
		NonYieldingCheckbox->setCheckState(Qt::Unchecked);
	}

	if (yieldingChecked) {
		fileName = QString("/data/F_%1/span_%2/%3/%3_yielding_small_sequences.txt").arg(field).arg(span).arg(complexity);
		openFileForUser(fileName);
		yieldingCheckbox->setCheckState(Qt::Unchecked);
	}

	if (smallSeqChecked) {
		int offset;

		if (field == 2) 
			offset = complexity - qPow(field, span - 1);
		else
			offset = complexity % field;

		//open file
		fileName = QString("/data/F_%1/smallSequences/sequences_of_complexity_%2.txt").arg(field).arg(offset);
		openFileForUser(fileName);
		filterSmallSeqCheckbox->setCheckState(Qt::Unchecked);
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
	bool inspectSeparatelyChecked = inspectSeparatelyCheckbox->isChecked();

	QString userInput = smallSequenceInput->toPlainText();
	if (userInput.isEmpty()) {
		QMessageBox::warning(this, tr("Error"), tr("Sequence inspection requires at least one sequence to be inserted in the box before proceeding."));
		return;
	}
	QStringList smallSequences = userInput.split('\n', Qt::SkipEmptyParts);

	QString saveFilePath;
	QFile fileOut;
	QTextStream out;

	QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	QString folderName = "inspection_result"; // output folder
	QDir desktopDir(desktopPath);

	//inspect separely 
	if (inspectSeparatelyChecked && !desktopDir.exists(folderName)) {
		desktopDir.mkdir(folderName); // Create the folder if it does not exist
	}
	else if (!inspectSeparatelyChecked) {
		saveFilePath = QFileDialog::getSaveFileName(this, tr("Save File"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("Text Files (*.txt)"));

		if (saveFilePath.isEmpty()) {
			QMessageBox::information(this, tr("Cancelled"), tr("File save cancelled."));
			return;
		}

		fileOut.setFileName(saveFilePath);
		if (!fileOut.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text )) {
			QMessageBox::warning(this, tr("Error"), tr("Cannot create or open file for writing."));
			return;
		}
		out.setDevice(&fileOut);
	}
	
	QString ReadFilePath = QString(":/results/data/F_%1/span_%2/%3/field_%1_span_%2_complexity_%3.txt").arg(field).arg(span).arg(complexity);
	QFile ReadFile(ReadFilePath);
	if (!ReadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("Error"), tr("Cannot open file for reading."));
		return;
	}
	 
	QTextStream in(&ReadFile);
	QString line;
	bool isSequenceOfInterest = false; // Flag to track if we're within a section of interest
	static QRegularExpression smallSeqRegex(R"(Debruijn Sequences generated by the sequence (\d+) : )");

	while (in.readLineInto(&line)) {

		QRegularExpressionMatch smallSeqMatch = smallSeqRegex.match(line);
		if (smallSeqMatch.hasMatch()) { //sequence of interest

			QString currentSeq = smallSeqMatch.captured(1);
			if (smallSequences.contains(currentSeq)) {

				isSequenceOfInterest = true;
				//inspect separelty
				if (inspectSeparatelyChecked) {

					QString individualFileName = QString("%1/%2_F%3_complexity_%4.txt").arg(folderName).arg(currentSeq).arg(field).arg(complexity);
					fileOut.setFileName(desktopPath + "/" + individualFileName);

					if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
						QMessageBox::warning(this, tr("Error"), tr("Cannot create or open file for writing."));
						return;
					}
					out.setDevice(&fileOut);
				}
				out << line << "\n";
			}
			else {
				isSequenceOfInterest = false;
				if (inspectSeparatelyChecked && fileOut.isOpen()) {
					fileOut.close();
				}
			}
		}
		else if (isSequenceOfInterest) {
			if (!line.trimmed().isEmpty()) {
				out << line << "\n";
			}
			else {
				out << "\n";
				isSequenceOfInterest = false;
				if (inspectSeparatelyChecked && fileOut.isOpen()) {
					fileOut.close(); // Close the individual file
				}
			}
		}
	}
	
	if (inspectSeparatelyChecked && fileOut.isOpen()) {
		fileOut.close();
	}

	// inform the user that the inspection is complete
	if (!inspectSeparatelyChecked) {
		QMessageBox::information(this, tr("Success"), tr("Inspection complete. Click OK to open the inspection result file."));
		QDesktopServices::openUrl(QUrl::fromLocalFile(fileOut.fileName()));
	}
	else {
		QMessageBox::information(this, tr("Success"), tr("All separate inspections completed. The results were saved under a new folder <inspection_result> in your desktop"));
	}
}
