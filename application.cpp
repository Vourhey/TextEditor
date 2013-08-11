#include <QSettings>
#include <QDebug>

#include "application.h"
#include "mainwindow.h"
#include "texteditor.h"
#include "appsettings.h"

Application *Application::m_app = 0;

Application::Application(int argc, char **argv)
    : QApplication(argc, argv)
{
    QCoreApplication::setOrganizationName("TextEditor");
    QCoreApplication::setOrganizationDomain("https://github.com/Vourhey/TextEditor");
    QCoreApplication::setApplicationName("VourheyApp");

    m_app = this;

    readSettings();

    createMainWindow();
    QStringList files = arguments();
    files.removeFirst();
    if(!files.isEmpty()) {
        mainWindows[0]->openFiles(files);
    }

//    connect(this, SIGNAL(lastWindowClosed()), SLOT(saveSettings()));
   
    qWarning("%p", this); 
    qWarning("%p", m_app);
}

Application *Application::instance()
{
    qWarning("%p", m_app);
    return m_app;
}

AppSettings *Application::appSettings() const
{
    return settings;
}

void Application::createMainWindow()
{
    MainWindow *mw = new MainWindow;
    mw->readSettings(settings);
    mw->show();
    mainWindows << mw;
}

TextEditor *Application::addTextEditor()
{
    TextEditor *editor = new TextEditor;
    textEditors << editor;
    return editor;
}

void Application::removeTextEditor(TextEditor *editor)
{
    int i = textEditors.indexOf(editor);
    if(i != -1) {
        delete textEditors.takeAt(i);
    }
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

void Application::updateSettingsRequest(Request r)
{
    readSettings();

    if(r == MAINWINDOW) {
        foreach(MainWindow *mw, mainWindows) {
            mw->readSettings(settings);
        }
    }

    if(r == TEXTEDITOR) {
        foreach(TextEditor *te, textEditors) {
            te->readSettings(settings);
        }
    }
}

