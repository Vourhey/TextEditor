#ifndef TEXTEDITOR_H
#define TEXTEDTTOR_H

#include <QPlainTextEdit>

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextEditor(QWidget *parent = 0);
};

#endif // TEXTEDITOR_H
