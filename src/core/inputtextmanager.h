/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include <QJsonObject>
#include <QObject>

#include "model/inputcompletermodel.h"

#include "libruqolacore_export.h"

class QAbstractItemModel;
class QSortFilterProxyModel;
class RocketChatAccount;
class CommandsModelFilterProxyModel;
class EmoticonModelFilterProxyModel;
class LIBRUQOLACORE_EXPORT InputTextManager : public QObject
{
    Q_OBJECT
public:
    enum CompletionForType { Channel = 0, User, Emoji, Command, None };
    explicit InputTextManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~InputTextManager() override;

    void setEmoticonModel(QAbstractItemModel *model);

    Q_REQUIRED_RESULT InputCompleterModel *inputCompleterModel() const;
    Q_REQUIRED_RESULT QAbstractItemModel *emojiCompleterModel() const;

    void inputTextCompleter(const QJsonObject &obj);

    Q_REQUIRED_RESULT QString applyCompletion(const QString &newWord, const QString &str, int *pPosition);

    void setInputTextChanged(const QString &str, int position);

    void clearCompleter();

    // Only for autotests
    Q_REQUIRED_RESULT QString searchWord(const QString &text, int position, int &start);

    void setCommandModel(QAbstractItemModel *model);
    Q_REQUIRED_RESULT QAbstractItemModel *commandModel() const;

Q_SIGNALS:
    // Trigger autocompletion request in DDPClient (via RocketChatAccount)
    // Emitted with Channel and User, never Emoji or None
    void completionRequested(const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type);
    void completionTypeChanged(InputTextManager::CompletionForType type);

private:
    Q_DISABLE_COPY(InputTextManager)

    void slotCompletionChannels(const QString &pattern);
    void setCompletionType(CompletionForType type);
    void slotCompletionChannelDone(const QJsonObject &root);

    InputCompleterModel *const mInputCompleterModel;
    EmoticonModelFilterProxyModel *const mEmoticonFilterProxyModel;
    CommandsModelFilterProxyModel *const mCommandFilterProxyModel;
    RocketChatAccount *const mAccount;
    CompletionForType mCurrentCompletionType = None;
    QString mCurrentCompletionPattern;
};

Q_DECLARE_METATYPE(InputTextManager::CompletionForType)

