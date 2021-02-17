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

#ifndef MESSAGETEXTEDIT_H
#define MESSAGETEXTEDIT_H

#include "inputtextmanager.h"
#include <KTextEdit>

#include "libruqolawidgets_private_export.h"
#include <QPointer>
class RocketChatAccount;
class CompletionListView;

/**
 * @brief The MessageTextEdit class is the widget used for typing messages to be sent.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageTextEdit : public KTextEdit
{
    Q_OBJECT
public:
    explicit MessageTextEdit(QWidget *parent = nullptr);
    ~MessageTextEdit() override;

    void setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog);

    void insertEmoji(const QString &text);
    Q_REQUIRED_RESULT QString text() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void changeText(const QString &str, int cursorPosition);

    QMenu *mousePopupMenu() override;

Q_SIGNALS:
    void sendMessage(const QString &str);
    void keyPressed(QKeyEvent *ev);
    void textEditing(bool clearNotification);

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    void slotCompletionTypeChanged(InputTextManager::CompletionForType type);
    void slotCompletionAvailable();
    void slotComplete(const QModelIndex &index);
    void slotSetAsBold();
    void slotSetAsItalic();
    void slotSetAsStrikeOut();
    void insertFormat(QChar formatChar);
    void slotInsertBlockCode();

    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    InputTextManager *mCurrentInputTextManager = nullptr;
    CompletionListView *mUserAndChannelCompletionListView = nullptr;
    CompletionListView *mEmojiCompletionListView = nullptr;
    CompletionListView *mCommandCompletionListView = nullptr;
};

#endif // MESSAGETEXTEDIT_H
