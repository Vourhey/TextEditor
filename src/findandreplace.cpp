#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QTextDocument>

#include "findandreplace.h"
#include "texteditor.h"

FindAndReplace::FindAndReplace(QWidget *parent)
    : QWidget(parent)
{
    m_editor = 0;

    findBox = new QComboBox;
    findBox->setEditable(true);

/*
    // how to make it right? I don't know... 
    // I'll back to this question later
    findBox->setStyleSheet("QComboBox { background: red }");
*/

    replaceBox = new QComboBox;
    replaceBox->setEditable(true);

    directionBox = new QComboBox;
    directionBox->addItem(tr("Up"));
    directionBox->addItem(tr("Down"));
    directionBox->addItem(tr("Both"));
    directionBox->setCurrentIndex(1);

    caseSenseCheck = new QCheckBox(tr("Case sensitive"));
    wholeWordCheck = new QCheckBox(tr("Match whole word"));
    replaceAllCheck = new QCheckBox(tr("Replace all in: "));

    whereReplaceBox = new QComboBox;
    whereReplaceBox->addItem(tr("Selection"));
    whereReplaceBox->addItem(tr("Document"));
    whereReplaceBox->addItem(tr("All Documents"));
    whereReplaceBox->setEnabled(false);

    findButton = new QPushButton(tr("Find"));
    replaceButton = new QPushButton(tr("Replace"));
    cancelButton = new QPushButton(tr("Cancel"));

    connect(findButton, SIGNAL(clicked()), SLOT(findSlot()));
    connect(replaceButton, SIGNAL(clicked()), SLOT(replaceSlot()));
    connect(replaceAllCheck, SIGNAL(toggled(bool)), whereReplaceBox, SLOT(setEnabled(bool)));
    connect(cancelButton, SIGNAL(clicked()), SLOT(hide()));

    createUI();
}

void FindAndReplace::createUI()
{
    QVBoxLayout *vLayout_labels = new QVBoxLayout;
    QLabel *lbl = new QLabel(tr("Search for: "));
    vLayout_labels->addWidget(lbl);
    lbl = new QLabel(tr("Replace with: "));
    vLayout_labels->addWidget(lbl);
    lbl = new QLabel(tr("Search direction: "));
    vLayout_labels->addWidget(lbl);

    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout *directionLayout = new QHBoxLayout;
    directionLayout->addWidget(directionBox);
    directionLayout->addSpacerItem(spacer);

    QVBoxLayout *vLayout_boxes = new QVBoxLayout;
    vLayout_boxes->addWidget(findBox);
    vLayout_boxes->addWidget(replaceBox);
    vLayout_boxes->addLayout(directionLayout);

    QHBoxLayout *firstLayout = new QHBoxLayout;
    firstLayout->addLayout(vLayout_labels);
    firstLayout->addLayout(vLayout_boxes);

    QHBoxLayout *whereReplaceLayout = new QHBoxLayout;
    whereReplaceLayout->addWidget(replaceAllCheck);
    whereReplaceLayout->addWidget(whereReplaceBox);

    QVBoxLayout *vLayout_checkes = new QVBoxLayout;
    vLayout_checkes->addWidget(caseSenseCheck);
    vLayout_checkes->addWidget(wholeWordCheck);
    vLayout_checkes->addLayout(whereReplaceLayout);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    buttonsLayout->addSpacerItem(spacer);
    buttonsLayout->addWidget(findButton);
    buttonsLayout->addWidget(replaceButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(firstLayout);
    mainLayout->addLayout(vLayout_checkes);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}

void FindAndReplace::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        hide();
    } else {
        QWidget::keyReleaseEvent(event);
    }
}

void FindAndReplace::setEditor(TextEditor *editor)
{
    m_editor = editor;
}

bool FindAndReplace::findSlot()
{
    if(findBox->currentText().isEmpty()) {
        // make red background
        return false;
    }

    return true;
}

void FindAndReplace::replaceSlot()
{
    // QTextDocument *doc = m_editor->document();

    if(!m_editor->textCursor().hasSelection()) {
        if(!findSlot()) {
            return;
        }
    }
}

