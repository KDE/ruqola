/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messagelineedit.h"
#include "rocketchataccount.h"
#include "model/inputcompletermodel.h"
#include "common/completionlistview.h"
#include "ruqola.h"

#include <QKeyEvent>
#include <QListView>
#include <QScreen>
#include <QScrollBar>

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <QApplication>
#include <QDesktopWidget>
#endif

MessageLineEdit::MessageLineEdit(QWidget *parent)
    : CompletionTextEdit(parent)
{
    connect(this, &QTextEdit::textChanged, this, &MessageLineEdit::slotTextChanged);
    setCompletionModel(Ruqola::self()->rocketChatAccount()->inputCompleterModel());
    connect(this, &MessageLineEdit::complete, this, &MessageLineEdit::slotComplete);
}

MessageLineEdit::~MessageLineEdit()
{
}

void MessageLineEdit::insert(const QString &text)
{
    textCursor().insertText(text);
}

QString MessageLineEdit::text() const
{
    return toPlainText();
}

void MessageLineEdit::keyPressEvent(QKeyEvent *e)
{
    const int key = e->key();
    if (key == Qt::Key_Return) {
        if (!e->modifiers()) {
            Q_EMIT sendMessage(text());
            clear();
        } else {
            textCursor().insertBlock();
            ensureCursorVisible();
        }
        e->accept();
        return;
    } else if (key == Qt::Key_Up || key == Qt::Key_Down) {
        if (document()->blockCount() > 1) {
            CompletionTextEdit::keyPressEvent(e);
            return;
        }
    }

    e->ignore();
    // Check if the listview or room widget want to handle the key (e.g Esc, PageUp)
    Q_EMIT keyPressed(e);
    if (e->isAccepted()) {
        return;
    }

    CompletionTextEdit::keyPressEvent(e);
}

void MessageLineEdit::slotTextChanged()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    rcAccount->setInputTextChanged(text(), textCursor().position());
    Q_EMIT textEditing(document()->isEmpty());
}

void MessageLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(InputCompleterModel::CompleterName).toString();
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString newText = rcAccount->replaceWord(completerName + QLatin1Char(' '), text(), textCursor().position());

    mCompletionListView->hide();

    disconnect(this, &QTextEdit::textChanged, this, &MessageLineEdit::slotTextChanged);
    setPlainText(newText);
    connect(this, &QTextEdit::textChanged, this, &MessageLineEdit::slotTextChanged);
}
