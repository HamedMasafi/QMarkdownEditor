#include "mainwindow.h"
#include "markdownhighlighter.h"

#include <QDebug>
#include <QFileDialog>
#include <QScrollBar>
#include <QSettings>
#include <QSyntaxHighlighter>
#include <QTextBlock>

MainWindow::MainWindow(QWidget *parent) :
      QMainWindow(parent)
{
    setupUi(this);

    new MarkdownHighlighter(textEdit->document());

    QSettings s;
    _filePath = s.value("FilePath").toString();
    if (!_filePath.isEmpty()) {
        QFile f(_filePath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        textEdit->setText(f.readAll());
        f.close();
    }

    connect(textEdit->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &MainWindow::textBrowser_scroll);
}

MainWindow::~MainWindow()
{
    if (!_filePath.isEmpty()) {
        QSettings s;
        s.setValue("FilePath", _filePath);
        s.sync();
    }
}

void MainWindow::textBrowser_scroll(int value)
{
    textBrowser->verticalScrollBar()->setValue(
        (int)(((float)value / (float)textEdit->verticalScrollBar()->maximum())
         * (float)textBrowser->verticalScrollBar()->maximum()));
}

void MainWindow::on_textEdit_textChanged()
{
    textBrowser->setMarkdown(textEdit->toPlainText());
}

void MainWindow::on_actionNew_triggered()
{
    _filePath.clear();
    textEdit->clear();
    textBrowser->clear();
}

void MainWindow::on_actionOpen_triggered()
{
    auto filePath
        = QFileDialog::getOpenFileName(this,
                                       "Open markdown",
                                       QString(),
                                       "Markdown files (*.md);;All files (*)");

    if (!filePath.isEmpty()) {
        QFile f(filePath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        textEdit->setText(f.readAll());
        f.close();
        _filePath = filePath;
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (_filePath.isEmpty())
        on_actionSave_as_triggered();

    QFile f(_filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    f.write(textEdit->toPlainText().toUtf8());
    f.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    auto filePath
        = QFileDialog::getSaveFileName(this,
                                       "Open markdown",
                                       QString(),
                                       "Markdown files (*.md);;All files (*)");

    if (!filePath.isEmpty()) {
        QFile f(filePath);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        f.write(textEdit->toPlainText().toUtf8());
        f.close();
        _filePath = filePath;
    }
}
