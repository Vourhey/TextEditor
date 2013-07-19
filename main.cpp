#include "application.h"
#include "mainwindow.h"

int main(int argc, char **argv)
{
    Application app(argc, argv);
//    MainWindow mw;
//    mw.show();
    return app.exec();
}

