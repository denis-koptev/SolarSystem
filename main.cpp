#include "SolarInterface.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SolarInterface my_interface;
    if (argc == 2) {
        my_interface.on_start_load(argv[1]);
    }
    my_interface.show();
    //my_interface.showFullScreen();
    return a.exec();
}
