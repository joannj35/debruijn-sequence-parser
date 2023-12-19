#include "mainWindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow mw;
    mw.show();

    app.setStyleSheet(
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

    return app.exec();
}

