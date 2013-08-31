#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QTextBlock>

#include "texteditor.h"
#include "gotodialog.h"

GotoDialog::GotoDialog(QWidget *parent)
    : QDialog(parent)
{
    lineSpin = new QSpinBox;
    lineSpin->setMinimum(1);
    columnSpin = new QSpinBox;
    connect(lineSpin, SIGNAL(valueChanged(int)), SLOT(updateColumnSpin(int)));

    cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

    jumpButton = new QPushButton(tr("Jump to"));
    jumpButton->setDefault(true);
    connect(jumpButton, SIGNAL(clicked()), SLOT(jump()));

    QGridLayout *mainLayout = new QGridLayout;

    QLabel *lbl = new QLabel(tr("Line: "));
    mainLayout->addWidget(lbl, 0, 0);
    
    lbl = new QLabel(tr("Column: "));
    mainLayout->addWidget(lbl, 1, 0);

    mainLayout->addWidget(lineSpin, 0, 1);
    mainLayout->addWidget(columnSpin, 1, 1);
    mainLayout->addWidget(cancelButton, 2, 0);
    mainLayout->addWidget(jumpButton, 2, 1);

    setLayout(mainLayout);
}

void GotoDialog::setEditor(TextEditor *editor)
{
    m_editor = editor;
    lineSpin->setMaximum(m_editor->blockCount());
    lineSpin->setValue(1);
}

void GotoDialog::jump()
{
    QTextBlock block = m_editor->document()->findBlockByLineNumber(lineSpin->value() - 1);
    int p = block.position();
    p += columnSpin->value();

    QTextCursor tc = m_editor->textCursor();
    tc.setPosition(p);
    m_editor->setTextCursor(tc);

    accept();
}

void GotoDialog::updateColumnSpin(int line)
{
    QTextBlock block = m_editor->document()->findBlockByLineNumber(line - 1);
    columnSpin->setMaximum(block.text().length());
}

