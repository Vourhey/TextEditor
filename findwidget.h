#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QWidget>

class TextEditor;
class QPushButton;
class QLineEdit;
class QCheckBox;

class FindWidget : public QWidget 
{
    Q_OBJECT
public:
    FindWidget(QWidget *parent);

    void setEditor(TextEditor *textEditor);

public slots:
    void next();
    void previous();

private slots:
    void updateFind();

private:
    TextEditor *m_editor;

    QPushButton *closeButton,
        *nextButton,
        *prevButton,
        *highlightAllButton;
    QLineEdit *findLine;
    QCheckBox *matchCase;
};

#endif // FINDWIDGET_H
