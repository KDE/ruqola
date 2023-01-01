/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QJsonObject>
#include <QObject>

#include "model/inputcompletermodel.h"

#include "libruqolacore_export.h"

class QAbstractItemModel;
class RocketChatAccount;
class CommandsModelFilterProxyModel;
class EmoticonModelFilterProxyModel;
class LIBRUQOLACORE_EXPORT InputTextManager : public QObject
{
    Q_OBJECT
public:
    enum CompletionForType {
        Channel = 0,
        User,
        Emoji,
        Command,
        None,
    };
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
