//#include "mainwindow.h"
#include "application.h"

int main(int argc, char **argv)
{
    Application app(argc, argv);
//    MainWindow mw;
//    mw.show();
    return app.exec();
}

