#ifndef TEXTEDITOR_H
#define TEXTEDTTOR_H

#include <QPlainTextEdit>

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextEditor(QWidget *parent = 0);

    QString showName() const;
    void loadFile(const QString &fileName);
    bool isSave();

public:
    bool save();
    bool saveAs();
    void deleteSlot();

signals:
    void fileNameChanged();

private:
    bool saveFile(const QString &fileName);
    void setFileName(const QString &fileName);

    static unsigned int number;     // counter

    QString currentFile;
    QString titleName;
};

#endif // TEXTEDITOR_H
