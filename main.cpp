#include "mainwindow.h"
#include "barcodesettingsmanager.h"
#include <QApplication>

/**
 * Code128 barcode generator.
 * For a better understanding of functionality provided see MainWindow.
 */

int main(int argc, char *argv[])
{
    qRegisterMetaType<BarcodeSettings>("BarcodeSettings");

    QApplication barcode_generator(argc, argv);
    MainWindow main_window;
    main_window.setWindowTitle("Barcode generator");
    main_window.show();
    return barcode_generator.exec();
}
