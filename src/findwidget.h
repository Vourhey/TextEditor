#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QToolBar>
#include <QTextDocument>

class TextEditor;
//class QToolButton;
class QAction;
class QLineEdit;
class QCheckBox;

class FindWidget : public QToolBar
{
    Q_OBJECT
public:
    FindWidget(QWidget *parent = 0);

    void setEditor(TextEditor *textEditor);

public slots:
    void next();
    void previous();

protected:
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void updateFind();

private:
    TextEditor *m_editor;
    QTextDocument *doc;
    QString findString;

    QAction *closeButton,
        *nextButton,
        *prevButton,
        *highlightAllButton;
    QLineEdit *findLine;
    QCheckBox *matchCase;
};

#endif // FINDWIDGET_H
