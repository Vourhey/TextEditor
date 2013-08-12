#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDataStream>

#include "application.h"
#include "appsettings.h"
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

    readSettings(myapp->appSettings());
}

void TextEditor::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Can not open file %s", qPrintable(fileName));
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
        qWarning("Can not write to %s", qPrintable(fileName));
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

void TextEditor::readSettings(AppSettings *settings)
{
    qWarning("in TextEditor::readSettings()");

    settings->beginGroup("texteditor");

    QFont font;
    QByteArray ba = settings->value("font", defaultFont).toByteArray();
    QDataStream out(ba);
    out >> font;
    document()->setDefaultFont(font);

    settings->endGroup();
}

