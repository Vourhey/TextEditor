#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QList>
#include "appsettings.h"

#define myapp Application::instance()

class MainWindow;
class TextEditor;

enum Request {
    MAINWINDOW,
    TEXTEDITOR
};

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);
    ~Application();

    static Application *instance();
    AppSettings *appSettings() const;

    void createMainWindow();
    TextEditor *addTextEditor();
    void removeTextEditor(TextEditor *editor);

    void updateSettingsRequest(Request r); 

private slots:
    void saveSettings();

private:
    void readSettings();
    static Application *m_app;

    QList<MainWindow*> mainWindows;
    QList<TextEditor*> textEditors;
    AppSettings *settings;
};

#endif // APPLICATION_H
