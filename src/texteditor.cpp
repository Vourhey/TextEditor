#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDataStream>
#include <QPainter>
#include <QTextBlock>

#include "application.h"
#include "texteditor.h"

unsigned int TextEditor::number = 1;

TextEditor::TextEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    titleName = QString("untitled %1.txt").arg(number);
    ++number;
    connect(document(), SIGNAL(contentsChanged()), SIGNAL(fileNameChanged()));

    qWarning("create TextEditor");

    QDataStream stream(&defaultFont, QIODevice::WriteOnly);
    stream << QFont("Monospace", 9);

    lineNumberArea = 0;

    readSettings(myapp->appSettings());
}

void TextEditor::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", tr("Can not open file %1").arg(fileName));
        return;
    }

    QTextStream stream(&file);
    setPlainText(stream.readAll());

    setFileName(fileName);
}

QString TextEditor::showName() const
{
    if(document()->isModified()) {
        return QString("*%1").arg(titleName);
    }

    return titleName;
}

void TextEditor::setFileName(const QString &fileName)
{
    currentFile = fileName;
    titleName = QFileInfo(fileName).fileName();
    document()->setModified(false);
    emit fileNameChanged();
}

bool TextEditor::isSave()
{
    if(document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("TextEditor"),
                tr("%1 has been modified.\n"
                    "Do you want to save your changes?").arg(titleName),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }

    return true;
}

bool TextEditor::isNull()
{
    return currentFile.isEmpty() && toPlainText().isEmpty();
}

bool TextEditor::save()
{
    if(!currentFile.isEmpty()) {
        return saveFile(currentFile);
    }

    return saveAs();
}

bool TextEditor::saveAs()
{
    QString name = QFileDialog::getSaveFileName(this, tr("Save File"));
    if(name.isEmpty()) {
        return false;
    }

    return saveFile(name);
}

bool TextEditor::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "TextEditor", tr("Can not write to %1").arg(fileName));
        return false;
    }

    QTextStream stream(&file);
    stream << toPlainText();

    setFileName(fileName);
    return true;
}

void TextEditor::deleteSlot()
{
    QTextCursor cur = textCursor();
    cur.removeSelectedText();
    setTextCursor(cur);
}

void TextEditor::lineNumberVisible()
{
    if(lineNumberBool) {
        if(!lineNumberArea) {
            lineNumberArea = new LineNumbers(this);

            connect(this, SIGNAL(blockCountChanged(int)), SLOT(updateLineNumberAreaWidth(int)));
            connect(this, SIGNAL(updateRequest(QRect,int)), SLOT(updateLineNumberArea(QRect,int)));
        }

        lineNumberArea->setVisible(true);
        updateLineNumberAreaWidth(0);
    } else {
        if(lineNumberArea) {
            lineNumberArea->setVisible(false);
            setViewportMargins(0, 0, 0, 0);
       }
    }
}

int TextEditor::lineNumbersWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void TextEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumbersWidth(), 0, 0, 0);
}

void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
        lineNumberArea->scroll(0, dy);
    } else {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void TextEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    if(lineNumberArea) {
        lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumbersWidth(), cr.height()));
    }
}

void TextEditor::lineNumbersPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void TextEditor::readSettings(AppSettings *settings)
{
    qWarning("TextEditor::readSettings()");

    settings->beginGroup("texteditor");

    QFont font;
    QByteArray ba = settings->value("font", defaultFont).toByteArray();
    QDataStream out(ba);
    out >> font;
    document()->setDefaultFont(font);

    lineNumberBool = settings->value("linenumbers", false).toBool();
    qWarning("for %p lineNumberBool is %d", this, lineNumberBool);
    lineNumberVisible();

    settings->endGroup();
}

