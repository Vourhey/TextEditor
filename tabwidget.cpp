#include <QMenu>

#include "tabwidget.h"
#include "texteditor.h"

/* TabBar */

TabBar::TabBar(TabWidget *parent)
    : QTabBar(parent)
{
    contextMenu = 0;
    tabW = parent;

    setMovable(true);
    setDocumentMode(true);
    setTabsClosable(true);
}

void TabBar::contextMenuEvent(QContextMenuEvent *ev)
{
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

/* TabWidget */

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabBar(new TabBar(this));
    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTabAt(int)));
//    prevWidget = 0;
}

TextEditor *TabWidget::createNewTab(const QString &name)
{
    cWidget = new TextEditor;
    if(!name.isEmpty()) {
        cWidget->loadFile(name);
    }
    connect(cWidget, SIGNAL(fileNameChanged()), SLOT(updateTitle()));

    addTab(cWidget, cWidget->showName());
    cWidget->setFocus();

    return cWidget;
}

bool TabWidget::closeAll()
{
    for(int i=0; i<count(); ++i) {
        if(!qobject_cast<TextEditor*>(widget(i))->isSave()) {
            return false;
        }
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
        delete te;
    }
}

void TabWidget::updateTitle()
{
    TextEditor *te = qobject_cast<TextEditor*>(sender());

    int i = indexOf(te);
    setTabText(i, te->showName());
}

