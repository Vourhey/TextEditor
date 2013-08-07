#include <QSettings>
#include <QDebug>

#include "application.h"
#include "appsettings.h"
#include "mainwindow.h"

Application *Application::m_app = 0;

Application::Application(int argc, char **argv)
    : QApplication(argc, argv)
{
    QCoreApplication::setOrganizationName("TextEditor");
    QCoreApplication::setOrganizationDomain("https://github.com/Vourhey/TextEditor");
    QCoreApplication::setApplicationName("VourheyApp");

    readSettings();

    createMainWindow();
    QStringList files = arguments();
    files.removeFirst();
    if(!files.isEmpty()) {
        mainWindows[0]->openFiles(files);
    }

//    connect(this, SIGNAL(lastWindowClosed()), SLOT(saveSettings()));
    
    m_app = this;
}

Application *Application::instance()
{
    return m_app;
}

void Application::createMainWindow()
{
    MainWindow *mw = new MainWindow;
    mw->readSettings(settings);
    mw->show();
    mainWindows << mw;
}

/*
void Application::saveSettings()
{
}
*/

void Application::readSettings()
{
    settings = new AppSettings;

    QSettings s;
    QStringList keys = s.allKeys();

    foreach(QString k, keys) {
        qWarning("%s", qPrintable(k));
        qDebug() << s.value(k);
        settings->setValue(k, s.value(k));
    }
}

void Application::updateSettingsRequest()
{
    readSettings();

    foreach(MainWindow *mw, mainWindows) {
        mw->readSettings(settings);
    }
}

