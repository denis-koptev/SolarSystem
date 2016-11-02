#include "Solar_Interface.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Solar_Interface my_interface;
    my_interface.show();
    //my_interface.showFullScreen();
    return a.exec();
}
