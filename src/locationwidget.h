#ifndef LOCATIONWIDGET_H
#define LOCATIONWIDGET_H

#include <QLabel>

class TextEditor;

class LocationWidget : public QLabel
{
    Q_OBJECT
public:
    LocationWidget(QWidget *parent = 0);
    void setEditor(TextEditor *editor);

public slots:
    void updateLabel();

private:
    void setPosition(int line, int column);

    TextEditor *m_editor;
};

#endif // LOCATIONWIDGET_H
