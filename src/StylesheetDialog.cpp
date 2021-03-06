// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "StylesheetDialog.h"
#include "ui_StylesheetDialog.h"
#include <Qsci/qscilexercss.h>
#include <QFileDialog>
#include <QColorDialog>
#include <QCursor>

QString StylesheetDialog::m_stylesheet;

StylesheetDialog::StylesheetDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::StylesheetDialog) {
    m_ui->setupUi(this);
    QsciLexerCSS* lexer = new QsciLexerCSS(this);
    m_ui->styleSheetTextEdit->setLexer(lexer);
}

StylesheetDialog::~StylesheetDialog() {
    delete m_ui;
}

void StylesheetDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void StylesheetDialog::save() {
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Seleft file to save stylesheet"),
                                                    QDir::currentPath(),
                                                    "*.qss");
    if(filename.isEmpty()) return;
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    f.write(m_ui->styleSheetTextEdit->text().toUtf8());
    f.close();
}

void StylesheetDialog::load() {
QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Seleft stylesheet to load"),
                                                    QDir::currentPath(),
                                                    "*.qss");
    if(filename.isEmpty()) return;
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    m_ui->styleSheetTextEdit->setText(f.readAll());
    f.close();
}

void StylesheetDialog::apply() {
    qApp->setStyleSheet("");
    m_stylesheet = QString(m_ui->styleSheetTextEdit->text());
    qApp->setStyleSheet(m_stylesheet);
}

void StylesheetDialog::chooseColor() {
    QString selection = m_ui->styleSheetTextEdit->selectedText();
    QColor initial;
    initial.setNamedColor(selection);
    QColor c = QColorDialog::getColor(initial, this);
    if(!c.isValid()) return;
    m_ui->styleSheetTextEdit->removeSelectedText();
    m_ui->styleSheetTextEdit->insert(c.name());
}
