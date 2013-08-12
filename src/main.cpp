#include <QTextCodec>
#include "application.h"

int main(int argc, char **argv)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    Application app(argc, argv);
    return app.exec();
}

