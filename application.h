#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QList>

class MainWindow;
class AppSettings;

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);

    static Application *instance();
    void updateSettingsRequest();

public slots:
    void createMainWindow();

/*
private slots:
    void saveSettings(); */

private:
    void readSettings();
    static Application *m_app;

    QList<MainWindow*> mainWindows;
    AppSettings *settings;
};

#endif // APPLICATION_H
