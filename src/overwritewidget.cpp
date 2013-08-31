#include <QShortcut>
#include "overwritewidget.h"
#include "texteditor.h"

OverwriteWidget::OverwriteWidget(QWidget *parent)
    : QLabel(parent)
{
    QShortcut *s = new QShortcut(Qt::Key_Insert, this);
    s->setContext(Qt::ApplicationShortcut);
    connect(s, SIGNAL(activated()), SLOT(changeOverwriteMode()));

    setText(tr("OVR"));
    setEnabled(false);
}

bool OverwriteWidget::event(QEvent *ev)
{
    if(ev->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = static_cast<QMouseEvent*>(ev);
        if((e->buttons() & Qt::LeftButton) == Qt::LeftButton) {
            qWarning("mousePressEvent()");
            changeOverwriteMode();
            return true;
        }
    }

    return QLabel::event(ev);
}

void OverwriteWidget::setEditor(TextEditor *editor)
{
    qWarning("OverwriteWidget::setEditor()");
    m_editor = editor;
    if(m_editor->overwriteMode()) {
        setEnabled(true);
    } else {
        setEnabled(false);
    }
}

void OverwriteWidget::changeOverwriteMode()
{
    qWarning("OverwriteWidget::changeOverwriteMode()");

    if(m_editor->overwriteMode()) {
        m_editor->setOverwriteMode(false);
        setEnabled(false);
    } else {
        m_editor->setOverwriteMode(true);
        setEnabled(true);
    }
}

