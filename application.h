#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QList>

class MainWindow;

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);

    Application *instance();

public slots:
    void createMainWindow();

/*
private slots:
    void saveSettings(); */

private:
//    void restoreSettings();

    QList<MainWindow*> mainWindows;
};

#endif // APPLICATION_H
