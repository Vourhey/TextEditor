#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>
#include <QBrush>

#include "findwidget.h"
#include "texteditor.h"

FindWidget::FindWidget(QWidget *parent)
    : QToolBar(parent)
{
    closeButton = new QToolButton;
    closeButton->setText("X");
    connect(closeButton, SIGNAL(clicked()), SLOT(hide()));
    
    nextButton = new QToolButton;
    nextButton->setEnabled(false);
    nextButton->setIcon(QIcon(":/images/go-down.png"));
    nextButton->setText(tr("Next"));
    connect(nextButton, SIGNAL(clicked()), SLOT(next()));

    prevButton = new QToolButton;
    prevButton->setEnabled(false);
    prevButton->setIcon(QIcon(":/images/go-up.png"));
    prevButton->setText(tr("Previous"));
    connect(prevButton, SIGNAL(clicked()), SLOT(previous()));

    highlightAllButton = new QToolButton;
    highlightAllButton->setIcon(QIcon(":/images/edit-select-all.png"));
    highlightAllButton->setText(tr("Highlight All"));
    highlightAllButton->setCheckable(true);
    connect(highlightAllButton, SIGNAL(clicked()), SLOT(updateFind()));

    findLine = new QLineEdit;
    findLine->setMinimumWidth(100);
    setFocusProxy(findLine);
    connect(findLine, SIGNAL(textEdited(const QString&)), SLOT(updateFind()));

    matchCase = new QCheckBox(tr("Match Case"));
    connect(matchCase, SIGNAL(clicked()), SLOT(updateFind()));

    addWidget(closeButton);
    addWidget(new QLabel(tr("Find: ")));
    addWidget(findLine);
    addWidget(nextButton);
    addWidget(prevButton);
    addWidget(highlightAllButton);
    addWidget(matchCase);

// replace every qtoolbutton to qaction and uncomment it
//    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void FindWidget::next()
{
    QTextDocument::FindFlags ff;
    if(matchCase->isChecked()) {
        ff |= QTextDocument::FindCaseSensitively;
    }

    QTextCursor tc = m_editor->textCursor();
    tc = doc->find(findString, tc, ff);
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
    m_editor->setTextCursor(tc);
}

void FindWidget::updateFind()
{
    findString = findLine->text();
    if(findString.isEmpty()) {
        return;
    }

    QTextDocument::FindFlags ff;

/*    if(findString.isEmpty()) {
        nextButton->setEnabled(false);
        prevButton->setEnabled(false);
    } else { */
        nextButton->setEnabled(true);
        prevButton->setEnabled(true);
//    }

    if(highlightAllButton->isChecked()) {
        QTextCursor tc = m_editor->textCursor();
        tc.setPosition(0);

        QList<QTextEdit::ExtraSelection> selections;
        QTextEdit::ExtraSelection s;
        QTextCharFormat format;
        format.setBackground(QBrush(Qt::yellow));

        tc = doc->find(findString, tc);
        while(!tc.isNull()) {
            s.cursor = tc;
            s.format = format;
            selections << s;
            tc = doc->find(findString, tc);
        }

        m_editor->setExtraSelections(selections);
    } else {
        m_editor->setExtraSelections(QList<QTextEdit::ExtraSelection>());
    }

    if(matchCase->isChecked()) {
        ff |= QTextDocument::FindCaseSensitively;
    }

    QTextCursor tc = m_editor->textCursor();
    tc.setPosition(tc.selectionStart());
    tc = doc->find(findString, tc, ff);
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

