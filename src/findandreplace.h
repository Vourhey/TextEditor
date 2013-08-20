#ifndef FINDANDREPLACE_H
#define FINDANDREPLACE_H

#include <QDialog>

class QComboBox;
class QCheckBox;
class QPushButton;

class TextEditor;

class FindAndReplace : public QDialog
{
    Q_OBJECT
public:
    FindAndReplace(QWidget *parent = 0);
    ~FindAndReplace();

    void setEditor(TextEditor *editor);

protected:
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void findWhileInputSlot(const QString &str = QString());
    bool findSlot();
    void replaceSlot();

private:
    void createUI();
    void setError(bool b);
    void readSettings();
    
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

    QStringList findList,
        replaceList;
};

#endif // FINDANDREPLACE_H
