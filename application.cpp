#include <QSettings>
#include "application.h"
#include "mainwindow.h"

Application::Application(int argc, char **argv)
    : QApplication(argc, argv)
{
    QCoreApplication::setOrganizationName("TextEditor");
    QCoreApplication::setOrganizationDomain("https://github.com/Vourhey/TextEditor");
    QCoreApplication::setApplicationName("VourheyApp");

    createMainWindow();
    QStringList files = arguments();
    files.removeFirst();
    if(!files.isEmpty()) {
        mainWindows[0]->openFiles(files);
    }

//    connect(this, SIGNAL(lastWindowClosed()), SLOT(saveSettings()));
//    restoreSettings();
}

Application *Application::instance()
{
    return this;
}

void Application::createMainWindow()
{
    MainWindow *mw = new MainWindow;
    mw->show();
    mainWindows << mw;
}

/*
void Application::saveSettings()
{
}

void Application::restoreSettings()
{
}
*/

