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

#include "messagetextedit.h"
#include "rocketchataccount.h"
#include "model/inputcompletermodel.h"
#include "common/completionlistview.h"
#include "ruqola.h"

#include <QAbstractTextDocumentLayout>
#include <QKeyEvent>

#include <common/emojicompletiondelegate.h>

MessageTextEdit::MessageTextEdit(QWidget *parent)
    : KTextEdit(parent)
{
    connect(this, &QTextEdit::textChanged, this, &MessageTextEdit::slotTextChanged);
    setAcceptRichText(false);

    connect(document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged,
            this, &QWidget::updateGeometry);

    mUserAndChannelCompletionListView = new CompletionListView;
    mUserAndChannelCompletionListView->setTextWidget(this);
    connect(mUserAndChannelCompletionListView, &CompletionListView::complete, this, &MessageTextEdit::slotComplete);

    mEmojiCompletionListView = new CompletionListView;
    mEmojiCompletionListView->setItemDelegate(new EmojiCompletionDelegate(mEmojiCompletionListView));
    mEmojiCompletionListView->setTextWidget(this);
    connect(mEmojiCompletionListView, &CompletionListView::complete, this, &MessageTextEdit::slotComplete);
}

MessageTextEdit::~MessageTextEdit()
{
    disconnect(this, &QTextEdit::textChanged, this, &MessageTextEdit::slotTextChanged);
    delete mUserAndChannelCompletionListView;
    delete mEmojiCompletionListView;
}

void MessageTextEdit::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount->inputTextManager(), &InputTextManager::completionTypeChanged,
                   this, &MessageTextEdit::slotCompletionTypeChanged);
    }
    mCurrentRocketChatAccount = account;
    InputTextManager *textManager = mCurrentRocketChatAccount->inputTextManager();
    mUserAndChannelCompletionListView->setModel(textManager->inputCompleterModel());
    mEmojiCompletionListView->setModel(textManager->emojiCompleterModel());
    connect(textManager, &InputTextManager::completionTypeChanged,
            this, &MessageTextEdit::slotCompletionTypeChanged);
}

void MessageTextEdit::insertEmoji(const QString &text)
{
    textCursor().insertText(text + QLatin1Char(' '));
}

QString MessageTextEdit::text() const
{
    return toPlainText();
}

QSize MessageTextEdit::sizeHint() const
{
    // The width of the QTextDocument is the current widget width, so this is somewhat circular logic.
    // But I don't really want to redo the layout with a different width like idealWidth(), seems slow.
    const QSize docSize = document()->size().toSize();
    const int margin = int(document()->documentMargin());
    return QSize(docSize.width() + margin, qMin(300, docSize.height()) + margin);
}

QSize MessageTextEdit::minimumSizeHint() const
{
    const int margin = int(document()->documentMargin());
    return QSize(300, fontMetrics().height() + margin);
}

void MessageTextEdit::keyPressEvent(QKeyEvent *e)
{
    const int key = e->key();
    qDebug() << key;
    if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        if ((key == Qt::Key_Enter && (e->modifiers() == Qt::KeypadModifier)) || !e->modifiers()) {
            Q_EMIT sendMessage(text());
            clear();
        } else {
            textCursor().insertBlock();
            ensureCursorVisible();
        }
        e->accept();
        return;
    } else if (key == Qt::Key_Up || key == Qt::Key_Down) {
        if (document()->lineCount() > 1) {
            KTextEdit::keyPressEvent(e);
            return;
        }
    }

    e->ignore();
    // Check if the listview or room widget want to handle the key (e.g Esc, PageUp)
    Q_EMIT keyPressed(e);
    if (e->isAccepted()) {
        return;
    }

    KTextEdit::keyPressEvent(e);
}

void MessageTextEdit::slotTextChanged()
{
    mCurrentRocketChatAccount->inputTextManager()->setInputTextChanged(text(), textCursor().position());
    Q_EMIT textEditing(document()->isEmpty());
}

void MessageTextEdit::slotCompletionTypeChanged(InputTextManager::CompletionForType type)
{
    if (type == InputTextManager::Emoji) {
        // show emoji completion popup when typing ':'
        mEmojiCompletionListView->slotCompletionAvailable();
        mUserAndChannelCompletionListView->hide();
    } else if (type == InputTextManager::None) {
        mUserAndChannelCompletionListView->hide();
        mEmojiCompletionListView->hide();
    } else {
        // the user and channel completion inserts rows when typing '@' so it will trigger slotCompletionAvailable automatically
        mEmojiCompletionListView->hide();
    }
}

void MessageTextEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(InputCompleterModel::CompleterName).toString();
    auto *inputTextManager = mCurrentRocketChatAccount->inputTextManager();
    QTextCursor cursor = textCursor();
    int textPos = cursor.position();
    const QString newText = inputTextManager->applyCompletion(completerName + QLatin1Char(' '), text(), &textPos);

    mUserAndChannelCompletionListView->hide();
    mEmojiCompletionListView->hide();

    disconnect(this, &QTextEdit::textChanged, this, &MessageTextEdit::slotTextChanged);
    setPlainText(newText);
    connect(this, &QTextEdit::textChanged, this, &MessageTextEdit::slotTextChanged);

    cursor.setPosition(textPos);
    setTextCursor(cursor);
}
