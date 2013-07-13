#include "tabwidget.h"
#include "texteditor.h"

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setDocumentMode(true);
}

TextEditor *TabWidget::createNewTab(const QString &name)
{
    cWidget = new TextEditor;
    if(!name.isEmpty()) {
        cWidget->loadFile(name);
    }

    addTab(cWidget, cWidget->showName());

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
 * if i == -1 than return current editor
 */
TextEditor *TabWidget::editor(int i) const
{
    if(i == -1) {
        return qobject_cast<TextEditor*>(currentWidget());
    }

    return qobject_cast<TextEditor*>(widget(i));
}

