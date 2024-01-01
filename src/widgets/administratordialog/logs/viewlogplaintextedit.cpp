/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "viewlogplaintextedit.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QPointer>
#include <QTextStream>
#if HAVE_TEXT_CUSTOM_EDITOR
ViewLogPlainTextEdit::ViewLogPlainTextEdit(QWidget *parent)
    : TextCustomEditor::PlainTextEditorWidget(new ViewLogPlainTextEditor(parent), parent)
{
}

ViewLogPlainTextEdit::~ViewLogPlainTextEdit() = default;

ViewLogPlainTextEditor::ViewLogPlainTextEditor(QWidget *parent)
    : TextCustomEditor::PlainTextEditor(parent)
{
    setReadOnly(true);
    setSearchSupport(true);
}

ViewLogPlainTextEditor::~ViewLogPlainTextEditor() = default;

void ViewLogPlainTextEditor::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    Q_UNUSED(pos)
    auto action = new QAction(QIcon::fromTheme(QStringLiteral("document-save")), i18n("Save as &File"), this);
    connect(action, &QAction::triggered, this, &ViewLogPlainTextEditor::slotSaveAsFile);
    menu->addSeparator();
    menu->addAction(action);
}

void ViewLogPlainTextEditor::slotSaveAsFile()
{
    saveTextAs(toPlainText(), QString(), this);
}

void ViewLogPlainTextEditor::saveTextAs(const QString &text, const QString &filter, QWidget *parent, const QUrl &url, const QString &caption)
{
    QPointer<QFileDialog> fdlg(new QFileDialog(parent, QString(), url.path(), filter));
    if (!caption.isEmpty()) {
        fdlg->setWindowTitle(caption);
    }
    fdlg->setAcceptMode(QFileDialog::AcceptSave);
    if (fdlg->exec() == QDialog::Accepted) {
        const QString fileName = fdlg->selectedFiles().at(0);
        if (!saveToFile(fileName, text)) {
            KMessageBox::error(parent,
                               i18n("Could not write the file %1:\n"
                                    "\"%2\" is the detailed error description.",
                                    fileName,
                                    QString::fromLocal8Bit(strerror(errno))),
                               i18n("Save File Error"));
        }
    }
    delete fdlg;
}

bool ViewLogPlainTextEditor::saveToFile(const QString &filename, const QString &text)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out << text;
    file.close();
    return true;
}
#else
ViewLogPlainTextEdit::ViewLogPlainTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setReadOnly(true);
}

ViewLogPlainTextEdit::~ViewLogPlainTextEdit() = default;

void ViewLogPlainTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *popup = createStandardContextMenu();
    if (popup) {
        auto action = new QAction(QIcon::fromTheme(QStringLiteral("document-save")), i18n("Save as &File"), this);
        connect(action, &QAction::triggered, this, &ViewLogPlainTextEdit::slotSaveAsFile);
        popup->addSeparator();
        popup->addAction(action);

        popup->exec(event->globalPos());
        delete popup;
    }
}

void ViewLogPlainTextEdit::slotSaveAsFile()
{
    saveTextAs(toPlainText(), QString(), this);
}

void ViewLogPlainTextEdit::saveTextAs(const QString &text, const QString &filter, QWidget *parent, const QUrl &url, const QString &caption)
{
    QPointer<QFileDialog> fdlg(new QFileDialog(parent, QString(), url.path(), filter));
    if (!caption.isEmpty()) {
        fdlg->setWindowTitle(caption);
    }
    fdlg->setAcceptMode(QFileDialog::AcceptSave);
    if (fdlg->exec() == QDialog::Accepted) {
        const QString fileName = fdlg->selectedFiles().at(0);
        if (!saveToFile(fileName, text)) {
            KMessageBox::error(parent,
                               i18n("Could not write the file %1:\n"
                                    "\"%2\" is the detailed error description.",
                                    fileName,
                                    QString::fromLocal8Bit(strerror(errno))),
                               i18n("Save File Error"));
        }
    }
    delete fdlg;
}

bool ViewLogPlainTextEdit::saveToFile(const QString &filename, const QString &text)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out << text;
    file.close();
    return true;
}

#endif

#include "moc_viewlogplaintextedit.cpp"
