#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

class TextEditor;
class QMenu;
class QAction;
class TabWidget;

class TabBar : public QTabBar
{
    Q_OBJECT
public:
    TabBar(TabWidget *parent);

private slots:
//    void changeTabByAlt();

protected:
    void contextMenuEvent(QContextMenuEvent *ev);

private:
    QMenu *contextMenu;
    QAction *saveAct,
        *saveAsAct,
        *closeAct;
    TabWidget *tabW;
};

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent = 0);

    TextEditor *createNewTab(const QString &name = QString());
    bool closeAll();
    TextEditor *editor(int i = -1) const;
//    TextEditor *prevEditor() const;

public slots:
    void closeTabAt(int i = -1);
    void nextTab();
    void previousTab();

private slots:
    void updateTitle();

private:
    TextEditor *cWidget;
//        *prevWidget;
    TabBar *m_tabBar;
};

#endif // TABWIDGET_H
