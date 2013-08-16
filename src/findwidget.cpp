#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextCursor>
#include <QTextEdit>
#include <QBrush>

#include "findwidget.h"
#include "texteditor.h"

FindWidget::FindWidget(QWidget *parent)
    : QToolBar(parent)
{
    closeButton = new QAction("X", this);
    connect(closeButton, SIGNAL(triggered()), SLOT(hide()));
    
    nextButton = new QAction(QIcon(":/images/go-down.png"), tr("Next"), this);
    nextButton->setEnabled(false);
    connect(nextButton, SIGNAL(triggered()), SLOT(next()));

    prevButton = new QAction(QIcon(":/images/go-up.png"), tr("Previous"), this);
    prevButton->setEnabled(false);
    connect(prevButton, SIGNAL(triggered()), SLOT(previous()));

    highlightAllButton = new QAction(QIcon(":/images/edit-select-all.png"), tr("Highlight All"), this);
    highlightAllButton->setCheckable(true);
    connect(highlightAllButton, SIGNAL(triggered()), SLOT(updateFind()));

    findLine = new QLineEdit;
    findLine->setMinimumWidth(100);
    setFocusProxy(findLine);
    connect(findLine, SIGNAL(textEdited(const QString&)), SLOT(updateFind()));
    connect(findLine, SIGNAL(returnPressed()), SLOT(next()));

    matchCase = new QCheckBox(tr("Match Case"));
    connect(matchCase, SIGNAL(clicked()), SLOT(updateFind()));

    addAction(closeButton);
    addWidget(new QLabel(tr("Find: ")));
    addWidget(findLine);
    addAction(nextButton);
    addAction(prevButton);
    addAction(highlightAllButton);
    addWidget(matchCase);

    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void FindWidget::next()
{
    QTextDocument::FindFlags ff;
    if(matchCase->isChecked()) {
        ff |= QTextDocument::FindCaseSensitively;
    }

    QTextCursor tc = m_editor->textCursor();
    tc = doc->find(findString, tc, ff);

    if(tc.isNull()) {
        tc.setPosition(0);
        tc = doc->find(findString, tc, ff);
    } 

    m_editor->setTextCursor(tc);
}

void FindWidget::previous()
{
    QTextDocument::FindFlags ff = QTextDocument::FindBackward;
    if(matchCase->isChecked()) {
        ff |= QTextDocument::FindCaseSensitively;
    }

    QTextCursor tc = m_editor->textCursor();
    tc = doc->find(findString, tc, ff);

    if(tc.isNull()) {
        tc = m_editor->textCursor();
        tc.movePosition(QTextCursor::End);
        tc = doc->find(findString, tc, ff);
    } 

    m_editor->setTextCursor(tc);
}

void FindWidget::updateFind()
{
    findString = findLine->text();
    findLine->setStyleSheet("QLineEdit { background: white; }"); 
    m_editor->setExtraSelections(QList<QTextEdit::ExtraSelection>());

    if(findString.isEmpty()) {
        nextButton->setEnabled(false);
        prevButton->setEnabled(false);
        QTextCursor tc = m_editor->textCursor();
        tc.setPosition(tc.selectionStart());
        m_editor->setTextCursor(tc);
        return;
    }

    nextButton->setEnabled(true);
    prevButton->setEnabled(true);

    QTextDocument::FindFlags ff;
    if(matchCase->isChecked()) {
        ff |= QTextDocument::FindCaseSensitively;
    }

    if(highlightAllButton->isChecked()) {
        QTextCursor tc = m_editor->textCursor();
        tc.setPosition(0);

        QList<QTextEdit::ExtraSelection> selections;
        QTextEdit::ExtraSelection s;

        tc = doc->find(findString, tc, ff);
        while(!tc.isNull()) {
            s.cursor = tc;
            s.format.setBackground(QBrush(Qt::yellow));
            selections << s;
            tc = doc->find(findString, tc, ff);
        }

        m_editor->setExtraSelections(selections);
    }

    QTextCursor tc = m_editor->textCursor();
    tc.setPosition(tc.selectionStart());
    tc = doc->find(findString, tc, ff);

    if(tc.isNull()) {
        tc.setPosition(0);
        tc = doc->find(findString, tc, ff);
        if(tc.isNull()) {
            findLine->setStyleSheet("QLineEdit { background: red; }"); 
            nextButton->setEnabled(false);
            prevButton->setEnabled(false);
            return;
        }
    } 

    m_editor->setTextCursor(tc);
}

void FindWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        hide();
    } else {
        QWidget::keyReleaseEvent(event);
    }
}

void FindWidget::setEditor(TextEditor *textEditor)
{
    m_editor = textEditor;
    doc = textEditor->document();
    updateFind();
}

