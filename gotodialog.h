#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QDialog>

class QSpinBox;
class QPushButton;
class TextEditor;

class GotoDialog : public QDialog
{
    Q_OBJECT
public:
    GotoDialog(QWidget *parent = 0);

    void setEditor(TextEditor *editor);

private slots:
    void jump();
    void updateColumnSpin(int line);

private:
    QSpinBox *lineSpin,
        *columnSpin;
    QPushButton *cancelButton,
        *jumpButton;
    TextEditor *m_editor;
};

#endif // GOTODIALOG_H
