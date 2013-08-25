#ifndef TEXTEDITOR_H
#define TEXTEDTTOR_H

#include <QPlainTextEdit>

class AppSettings;
class LineNumbers;

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextEditor(QWidget *parent = 0);

    QString showName() const;
    void loadFile(const QString &fileName);
    bool isSave();
    bool isNull();

    void readSettings(AppSettings *settings);

    // for LineNumbers
    void lineNumbersPaintEvent(QPaintEvent *event);
    int lineNumbersWidth();

public slots:
    bool save();
    bool saveAs();
    void deleteSlot();
    void toLowercase();
    void toUppercase();
    void toTitleCase();
    void toOppositeCase();
    void transpose();
    void moveToLineUp();
    void moveToLineDown();
    void duplicateLineSelection();

signals:
    void fileNameChanged();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);

private:
    bool saveFile(const QString &fileName);
    void setFileName(const QString &fileName);
    void addToRecentFiles(const QString &fileName);
    void moveToLineUpDown(bool up);

    void lineNumberVisible();

    static unsigned int number;     // counter
    QByteArray defaultFont;

    QString currentFile;
    QString titleName;

    bool lineNumberBool;    
    QWidget *lineNumberArea;
};

class LineNumbers : public QWidget
{
public:
    LineNumbers(TextEditor *editor) : QWidget(editor) {
        m_editor = editor;
    }

    QSize sizeHint() const {
        return QSize(m_editor->lineNumbersWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        m_editor->lineNumbersPaintEvent(event);
    }

private:
    TextEditor *m_editor;
};

#endif // TEXTEDITOR_H
