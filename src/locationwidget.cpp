#include <QTextCursor>
#include "texteditor.h"
#include "locationwidget.h"

LocationWidget::LocationWidget(QWidget *parent)
    : QLabel(parent)
{
    setPosition(0, 0);
}

void LocationWidget::setEditor(TextEditor *editor)
{
    m_editor = editor;
    setPosition(0, 0);
}

void LocationWidget::updateLabel()
{
    QTextCursor cur = m_editor->textCursor();
    setPosition(cur.blockNumber(), cur.positionInBlock());
}

void LocationWidget::setPosition(int line, int column)
{
    setText(tr("Line: %1 Column: %2").arg(line+1).arg(column));
}

