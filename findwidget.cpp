#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QTextCursor>
#include <QTextDocument>

#include "findwidget.h"
#include "texteditor.h"

FindWidget::FindWidget(QWidget *parent)
    : QWidget(parent)
{
    closeButton = new QPushButton;
    closeButton->setText("X");
    connect(closeButton, SIGNAL(clicked()), parent, SLOT(hide()));
    
    nextButton = new QPushButton;
    nextButton->setIcon(QIcon(":/images/go-down.png"));
    nextButton->setText(tr("Next"));
    connect(nextButton, SIGNAL(clicked()), SLOT(next()));

    prevButton = new QPushButton;
    prevButton->setIcon(QIcon(":/images/go-up.png"));
    prevButton->setText(tr("Previous"));
    connect(prevButton, SIGNAL(clicked()), SLOT(previous()));

    highlightAllButton = new QPushButton;
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

    QHBoxLayout *mainLayout = new QHBoxLayout;

    mainLayout->addWidget(closeButton);
    mainLayout->addWidget(new QLabel(tr("Find:")));
    mainLayout->addWidget(findLine);
    mainLayout->addWidget(nextButton);
    mainLayout->addWidget(prevButton);
    mainLayout->addWidget(highlightAllButton);
    mainLayout->addWidget(matchCase);

    setLayout(mainLayout);
}

void FindWidget::next()
{
}

void FindWidget::previous()
{
}

void FindWidget::updateFind()
{
    QString text = findLine->text();
    QTextDocument::FindFlags ff;

    bool selectAll = false;

    if(highlightAllButton->isChecked()) {
        selectAll = true;
    }
    if(matchCase->isChecked()) {
        ff |= QTextDocument::FindCaseSensitively;
    }

    QTextCursor tc = m_editor->textCursor();
    tc.setPosition(tc.selectionStart());
    tc = m_editor->document()->find(text, tc, ff);
    m_editor->setTextCursor(tc);
}

void FindWidget::setEditor(TextEditor *textEditor)
{
    m_editor = textEditor;
    updateFind();
}

