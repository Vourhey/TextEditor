#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

class TextEditor;

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent = 0);

    TextEditor *createNewTab(const QString &name = QString());
    bool closeAll();
    TextEditor *editor(int i = -1) const;

private:
    TextEditor *cWidget;
};

#endif // TABWIDGET_H
