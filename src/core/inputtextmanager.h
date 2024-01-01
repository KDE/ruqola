/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include "model/inputcompletermodel.h"

#include "libruqolacore_export.h"

class QAbstractItemModel;
class RocketChatAccount;
class CommandsModelFilterProxyModel;
class EmoticonFilterProxyModel;
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

    [[nodiscard]] InputCompleterModel *inputCompleterModel() const;
    [[nodiscard]] QAbstractItemModel *emojiCompleterModel() const;

    void inputTextCompleter(const QJsonObject &obj);

    [[nodiscard]] QString applyCompletion(const QString &newWord, const QString &str, int *pPosition);

    void setInputTextChanged(const QString &roomId, const QString &str, int position);

    void clearCompleter();

    // Only for autotests
    [[nodiscard]] QString searchWord(const QString &text, int position, int &start);

    void setCommandModel(QAbstractItemModel *model);
    [[nodiscard]] QAbstractItemModel *commandModel() const;

Q_SIGNALS:
    // Trigger autocompletion request in DDPClient (via RocketChatAccount)
    // Emitted with Channel and User, never Emoji or None
    void completionRequested(const QString &roomId, const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type);
    void completionTypeChanged(InputTextManager::CompletionForType type);
    void selectFirstTextCompleter();

private:
    LIBRUQOLACORE_NO_EXPORT void setCompletionType(CompletionForType type);

    InputCompleterModel *const mInputCompleterModel;
    EmoticonFilterProxyModel *const mEmoticonFilterProxyModel;
    CommandsModelFilterProxyModel *const mCommandFilterProxyModel;
    RocketChatAccount *const mRocketChatAccount;
    CompletionForType mCurrentCompletionType = None;
    QString mCurrentCompletionPattern;
};

Q_DECLARE_METATYPE(InputTextManager::CompletionForType)
