/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
#include "common/commandcompletiondelegate.h"
#include "common/emojicompletiondelegate.h"
#include "ruqola.h"

#include <QAbstractTextDocumentLayout>
#include <QKeyEvent>

MessageTextEdit::MessageTextEdit(QWidget *parent)
    : KTextEdit(parent)
{
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

    mCommandCompletionListView = new CompletionListView;
    mCommandCompletionListView->setItemDelegate(new CommandCompletionDelegate(mCommandCompletionListView));
    mCommandCompletionListView->setTextWidget(this);
    connect(mCommandCompletionListView, &CompletionListView::complete, this, &MessageTextEdit::slotComplete);
}

MessageTextEdit::~MessageTextEdit()
{
    delete mUserAndChannelCompletionListView;
    delete mEmojiCompletionListView;
    delete mCommandCompletionListView;
}

void MessageTextEdit::setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog)
{
    if (mCurrentInputTextManager) {
        disconnect(mCurrentInputTextManager, &InputTextManager::completionTypeChanged,
                   this, &MessageTextEdit::slotCompletionTypeChanged);
    }
    mCurrentRocketChatAccount = account;
    mCurrentInputTextManager = threadMessageDialog ? mCurrentRocketChatAccount->inputThreadMessageTextManager() : mCurrentRocketChatAccount->inputTextManager();
    mUserAndChannelCompletionListView->setModel(mCurrentInputTextManager->inputCompleterModel());
    mEmojiCompletionListView->setModel(mCurrentInputTextManager->emojiCompleterModel());
    mCommandCompletionListView->setModel(mCurrentInputTextManager->commandModel());
    connect(mCurrentInputTextManager, &InputTextManager::completionTypeChanged,
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

void MessageTextEdit::changeText(const QString &newText, int cursorPosition)
{
    setPlainText(newText);

    QTextCursor cursor(document());
    cursor.setPosition(cursorPosition);
    setTextCursor(cursor);

    mCurrentInputTextManager->setInputTextChanged(text(), cursorPosition);
}

void MessageTextEdit::keyPressEvent(QKeyEvent *e)
{
    const int key = e->key();
    if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        if ((key == Qt::Key_Enter && (e->modifiers() == Qt::KeypadModifier)) || !e->modifiers()) {
            Q_EMIT sendMessage(text());
            //We send text => we will clear => we will send textEditing is empty => clear notification
            Q_EMIT textEditing(true);
            clear();
        } else {
            textCursor().insertBlock();
            ensureCursorVisible();
        }
        e->accept();
        return;
    } else if (key == Qt::Key_Up || key == Qt::Key_Down) {
        // document()->lineCount() is > 1 if the user used Shift+Enter
        // firstBlockLayout->lineCount() is > 1 if a single long line wrapped around
        const QTextLayout *firstBlockLayout = document()->firstBlock().layout();
        if (document()->lineCount() > 1 || firstBlockLayout->lineCount() > 1) {
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
    //Assign key to KTextEdit first otherwise text() doesn't return correct text
    KTextEdit::keyPressEvent(e);
    if (key == Qt::Key_Delete || key == Qt::Key_Backspace) {
        if (textCursor().hasSelection() && textCursor().selectedText() == text()) {
            //We will clear all text => we will send textEditing is empty => clear notification
            Q_EMIT textEditing(true);
        } else {
            mCurrentInputTextManager->setInputTextChanged(text(), textCursor().position());
            Q_EMIT textEditing(document()->isEmpty());
        }
    } else {
        if (!e->modifiers() || e->matches(QKeySequence::Paste) || key == Qt::Key_Slash || key == '@' || key == '#') {
            mCurrentInputTextManager->setInputTextChanged(text(), textCursor().position());
            Q_EMIT textEditing(document()->isEmpty());
        }
    }
}

void MessageTextEdit::slotCompletionTypeChanged(InputTextManager::CompletionForType type)
{
    if (type == InputTextManager::Emoji) {
        // show emoji completion popup when typing ':'
        mEmojiCompletionListView->slotCompletionAvailable();
        mUserAndChannelCompletionListView->hide();
        mCommandCompletionListView->hide();
    } else if (type == InputTextManager::None) {
        mUserAndChannelCompletionListView->hide();
        mEmojiCompletionListView->hide();
        mCommandCompletionListView->hide();
    } else if (type == InputTextManager::Command) {
        mUserAndChannelCompletionListView->hide();
        mEmojiCompletionListView->hide();
        mCommandCompletionListView->slotCompletionAvailable();
    } else {
        // the user and channel completion inserts rows when typing '@' so it will trigger slotCompletionAvailable automatically
        mEmojiCompletionListView->hide();
        mCommandCompletionListView->hide();
    }
}

void MessageTextEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(InputCompleterModel::CompleterName).toString();
    int textPos = textCursor().position();
    const QString newText = mCurrentInputTextManager->applyCompletion(completerName + QLatin1Char(' '), text(), &textPos);

    mUserAndChannelCompletionListView->hide();
    mEmojiCompletionListView->hide();
    mCommandCompletionListView->hide();

    changeText(newText, textPos);
}
