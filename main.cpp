#include "MainWindow.h"
#include <QApplication>
#include <QPalette>

bool isDarkMode();

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow mw;
    mw.show();

    QString commonStyles =
        "QPushButton { "
        "  background-color: #2196F3; " // blue
        "  color: white; " // text color
        "  border-radius: 4px; " // Rounded corners
        "  font-weight: bold; "
        "  border: none; "
        "  padding: 5px 15px; "
        "  font-size: 13px; "
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

        "QGroupBox { "
        "border: 2px solid #2196F3; "
        "  border-radius: 4px; "
        "  margin-top: 2ex; "
        "} "

        "QGroupBox::title { "
        "  subcontrol-origin: margin; "
        "  subcontrol-position: top center; " // Adjust title position to center if you want
        "  padding: 0 3px 0 3px; " 
        "}";

    QString messageBoxStyle;
    if (isDarkMode()) {
        messageBoxStyle = "QMessageBox { background-color: #444444; }"; // dark gray
    }
    else {
        messageBoxStyle = "QMessageBox { background-color: #eeeeee; }"; // light gray
    }

    app.setStyleSheet(commonStyles + messageBoxStyle);

    return app.exec();
}

bool isDarkMode() {
    QPalette palette = QApplication::palette();
    QColor windowTextColor = palette.color(QPalette::WindowText);
    QColor windowBackgroundColor = palette.color(QPalette::Window);

    // Calculate the luminance (brightness) of the window text color
    double luminance = 0.299 * windowTextColor.redF() + 0.587 * windowTextColor.greenF() + 0.114 * windowTextColor.blueF();

    // Assuming dark mode if the luminance is low
    return luminance > 0.5;
}
