#include <QMenu>
#include <QSignalMapper>
#include <QShortcut>
#include <QKeySequence>

#include "tabwidget.h"
#include "texteditor.h"
#include "application.h"

/* TabBar */

TabBar::TabBar(TabWidget *parent)
    : QTabBar(parent)
{
    qWarning("TabBar::TabBar()");
    contextMenu = 0;
    tabW = parent;

    setMovable(true);
    setDocumentMode(true);
    setTabsClosable(true);

// add shortcuts Alt+[0-9]
    QSignalMapper *m = new QSignalMapper(this);
    QShortcut *shortcut;
    QString altd("Alt+%1");

    for(int i=1; i<10; ++i) {
        shortcut = new QShortcut(QKeySequence(altd.arg(i)), this);
        connect(shortcut, SIGNAL(activated()), m, SLOT(map()));
        m->setMapping(shortcut, i-1);
    }
    shortcut = new QShortcut(QKeySequence("Alt+0"), this);
    connect(shortcut, SIGNAL(activated()), m, SLOT(map()));
    m->setMapping(shortcut, 9);

    connect(m, SIGNAL(mapped(int)), SLOT(setCurrentIndex(int)));
    hide();
}

void TabBar::contextMenuEvent(QContextMenuEvent *ev)
{
    qWarning("TabBar::contextMenuEvent()");
    if(!contextMenu) {
        contextMenu = new QMenu;
        saveAct = new QAction(tr("Save"), this);
        saveAsAct = new QAction(tr("Save As..."), this);
        closeAct = new QAction(tr("Close Tab"), this);
    }

    setCurrentIndex(tabAt(ev->pos()));

    saveAct->disconnect();
    connect(saveAct, SIGNAL(triggered()), tabW->editor(), SLOT(save()));

    saveAsAct->disconnect();
    connect(saveAsAct, SIGNAL(triggered()), tabW->editor(), SLOT(saveAs()));

    closeAct->disconnect();
    connect(closeAct, SIGNAL(triggered()), tabW, SLOT(closeTabAt()));

    contextMenu->addAction(saveAct);
    contextMenu->addAction(saveAsAct);
    contextMenu->addSeparator();
    contextMenu->addAction(closeAct);

    /* todo */

    contextMenu->exec(ev->globalPos());
}

/*
void TabBar::changeTabByAlt()
{
    qWarning("TabBar::changeTabByAlt()");
    QAction *s = qobject_cast<QAction*>(sender());
    setCurrentIndex(s->data().toInt());
} */

/* TabWidget */

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    m_tabBar = new TabBar(this);
    m_tabBar->hide();
    setTabBar(m_tabBar);
    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTabAt(int)));
//    prevWidget = 0;
}

TextEditor *TabWidget::createNewTab(const QString &name)
{
    int i = currentIndex();

    if(i != -1 && editor()->isNull() && !name.isEmpty()) {
        cWidget = editor(i);
    } else {
        cWidget = myapp->addTextEditor();
        connect(cWidget, SIGNAL(fileNameChanged()), SLOT(updateTitle()));
        i = addTab(cWidget, cWidget->showName());
    }

    if(!name.isEmpty()) {
        cWidget->loadFile(name);
    }

    setCurrentIndex(i);
    cWidget->setFocus();

    if(count() > 1) {
        m_tabBar->show();
    }

    return cWidget;
}

bool TabWidget::closeAll()
{
    TextEditor *te;
    for(int i=count() - 1; i >= 0; --i) {
        te = editor(i);
        if(!te->isSave()) {
            return false;
        }

        removeTab(i);
        myapp->removeTextEditor(te); 
    }

    return true;
}

/*
TextEditor *TabWidget::prevEditor() const
{
    return prevWidget;
}
*/

/*
 * if i == -1 than return current editor
 */
TextEditor *TabWidget::editor(int i) const
{
    if(i == -1) {
        return qobject_cast<TextEditor*>(currentWidget());
    }

    return qobject_cast<TextEditor*>(widget(i));
}

void TabWidget::closeTabAt(int i)
{
    if(i == -1) {
        i = currentIndex();
    }

    TextEditor *te = editor(i);

    if(te->isSave()) {
        removeTab(i);
        myapp->removeTextEditor(te);
    }

    if(count() == 1) {
        m_tabBar->hide();
    }
}

void TabWidget::nextTab()
{
    int i = currentIndex() + 1;

    if(i == count()) {
        i = 0;
    }

    setCurrentIndex(i);
}

void TabWidget::previousTab()
{
    int i = currentIndex() - 1;
    
    if(i == -1) {
        i = count() - 1;
    }

    setCurrentIndex(i);
}

void TabWidget::updateTitle()
{
    TextEditor *te = qobject_cast<TextEditor*>(sender());

    int i = indexOf(te);
    setTabText(i, te->showName());
    emit fileNameChanged(i);
}

