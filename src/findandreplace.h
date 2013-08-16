#ifndef FINDANDREPLACE_H
#define FINDANDREPLACE_H

#include <QWidget>

class QComboBox;
class QCheckBox;
class QPushButton;

class TextEditor;

class FindAndReplace : public QWidget
{
    Q_OBJECT
public:
    FindAndReplace(QWidget *parent = 0);

    void setEditor(TextEditor *editor);

protected:
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void findWhileInputSlot(const QString &str);
    bool findSlot();
    void replaceSlot();

private:
    void createUI();
    
    QComboBox *findBox,
        *replaceBox,
        *directionBox,
        *whereReplaceBox;

    QCheckBox *caseSenseCheck,
        *wholeWordCheck,
        *replaceAllCheck;

    QPushButton *findButton,
        *replaceButton,
        *cancelButton;

    TextEditor *m_editor;
};

#endif // FINDANDREPLACE_H
