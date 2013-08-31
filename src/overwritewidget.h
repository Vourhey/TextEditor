#ifndef OVERWRITEWIDGET_H
#define OVERWRITEWIDGET_H

#include <QLabel>

class TextEditor;

class OverwriteWidget : public QLabel
{
    Q_OBJECT
public:
    OverwriteWidget(QWidget *parent = 0);
    void setEditor(TextEditor *editor);

protected:
    bool event(QEvent *ev);

private slots:
    void changeOverwriteMode();

private:
    TextEditor *m_editor;
};

#endif // OVERWRITEWIDGET_H
