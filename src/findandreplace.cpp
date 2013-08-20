#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QTextDocument>

#include "application.h"
#include "findandreplace.h"
#include "texteditor.h"

FindAndReplace::FindAndReplace(QWidget *parent)
    : QDialog(parent)
{
    m_editor = 0;

    findBox = new QComboBox;
    findBox->setEditable(true);

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
    findButton->setDefault(true);
    replaceButton = new QPushButton(tr("Replace"));
    cancelButton = new QPushButton(tr("Cancel"));
    
    readSettings();

    connect(findButton, SIGNAL(clicked()), SLOT(findSlot()));
    connect(findBox, SIGNAL(editTextChanged(const QString&)), SLOT(findWhileInputSlot(const QString&)));
    connect(replaceButton, SIGNAL(clicked()), SLOT(replaceSlot()));
    connect(replaceAllCheck, SIGNAL(toggled(bool)), whereReplaceBox, SLOT(setEnabled(bool)));
    connect(cancelButton, SIGNAL(clicked()), SLOT(hide()));
    connect(caseSenseCheck, SIGNAL(clicked()), SLOT(findWhileInputSlot()));
    connect(wholeWordCheck, SIGNAL(clicked()), SLOT(findWhileInputSlot()));

    createUI();
}

FindAndReplace::~FindAndReplace()
{
    qWarning("FindAndReplace::~FindAndReplace()");

    AppSettings *appset = myapp->appSettings();

    appset->beginGroup("findandreplace");
    appset->setValue("findlist", findList);
    appset->setValue("replacelist", replaceList);
    appset->endGroup();
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
//    vLayout_checkes->addLayout(whereReplaceLayout);

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

void FindAndReplace::findWhileInputSlot(const QString &str)
{
    qWarning("FindAndReplace::findWhileInputSlot()");

    QTextCursor tc = m_editor->textCursor();
    findBox->lineEdit()->setStyleSheet("QLineEdit { background: white; }");
    QString findStr = str;

    if(str.isNull()) {
        findStr = findBox->currentText();
    }

    if(findStr.isEmpty()) {
        findButton->setEnabled(false);
        replaceButton->setEnabled(false); 
        tc.setPosition(0);
        m_editor->setTextCursor(tc);
        return;
    }

    QTextDocument::FindFlags ff;
    if(caseSenseCheck->isChecked()) {
        ff |= QTextDocument::FindCaseSensitively;
    }
    if(wholeWordCheck->isChecked()) {
        ff |= QTextDocument::FindWholeWords;
    }

    QTextDocument *doc = m_editor->document();
//    qWarning("doc's text: %s", qPrintable(doc->toPlainText()));
    tc = m_editor->textCursor();
//    qWarning("tc is null: %d", tc.isNull());
    tc.setPosition(tc.selectionStart());
    tc = doc->find(findStr, tc, ff);

    if(tc.isNull()) {
        tc = m_editor->textCursor();
        tc.setPosition(0);
        tc = doc->find(findStr, tc, ff);
        if(tc.isNull()) {   // there is no str
            setError(true);
            return;
        }
    }

    setError(false);

    qWarning("tc is null: %d", tc.isNull());
    m_editor->setTextCursor(tc);
}

bool FindAndReplace::findSlot()
{
    qWarning("FindAndReplace::findSlot()");

    QString findString = findBox->currentText();
    QTextDocument *doc = m_editor->document();
    QTextCursor tc = m_editor->textCursor();

    QTextDocument::FindFlags ff;
    if(caseSenseCheck->isChecked()) {
        ff |= QTextDocument::FindCaseSensitively;
    }
    if(wholeWordCheck->isChecked()) {
        ff |= QTextDocument::FindWholeWords;
    }
    if(directionBox->currentIndex() == 0) { // Up
        ff |= QTextDocument::FindBackward;
//        tc.setPosition(tc.selectionStart());
    }

    tc = doc->find(findString, tc, ff);

    if(tc.isNull()) {
        setError(true);
        if(directionBox->currentIndex() == 2) { // Both
            tc = m_editor->textCursor();
            tc.setPosition(0);
            tc = doc->find(findString, tc, ff);
            if(!tc.isNull()) {
                setError(false);
                m_editor->setTextCursor(tc);
                return true;
            }
        }
        return false;
    }

    m_editor->setTextCursor(tc);
    return true;
}

void FindAndReplace::replaceSlot()
{
    QTextCursor tc = m_editor->textCursor();

    if(!tc.hasSelection()) {
        if(!findSlot()) {
            return;
        }
    }

    tc = m_editor->textCursor();
    QString replaceString = replaceBox->currentText();
    tc.insertText(replaceString);

    QString findString = findBox->currentText();

    findList.append(findString);
    findList.removeDuplicates();
    replaceList.append(replaceString);
    replaceList.removeDuplicates();

    findBox->clear();
    findBox->addItems(findList);
    findBox->setEditText(findString);

    replaceBox->clear();
    replaceBox->addItems(replaceList);
    replaceBox->setEditText(replaceString);

    findSlot();
}

void FindAndReplace::setError(bool b)
{
    if(b) {
        // make red background
        findBox->lineEdit()->setStyleSheet("QLineEdit { background: red; }");
        findButton->setEnabled(false);
        replaceButton->setEnabled(false);
    } else {
        findBox->lineEdit()->setStyleSheet("QLineEdit { background: white; }");
        findButton->setEnabled(true);
        replaceButton->setEnabled(true);

    }
}

void FindAndReplace::readSettings()
{
    AppSettings *appset = myapp->appSettings();

    appset->beginGroup("findandreplace");
    findList = appset->value("findlist", QStringList()).toStringList();
    replaceList = appset->value("replacelist", QStringList()).toStringList();
    appset->endGroup();

    findBox->addItems(findList);
    findBox->setEditText("");
    replaceBox->addItems(replaceList);
    replaceBox->setEditText("");
}

