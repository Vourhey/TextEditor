#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QToolBar>

class TextEditor;
class QToolButton;
class QLineEdit;
class QCheckBox;
class QTextDocument;

class FindWidget : public QToolBar
{
    Q_OBJECT
public:
    FindWidget(QWidget *parent = 0);

    void setEditor(TextEditor *textEditor);

public slots:
    void next();
    void previous();

private slots:
    void updateFind();

private:
    TextEditor *m_editor;
    QTextDocument *doc;
    QString findString;

    QToolButton *closeButton,
        *nextButton,
        *prevButton,
        *highlightAllButton;
    QLineEdit *findLine;
    QCheckBox *matchCase;
};

#endif // FINDWIDGET_H
