/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef INPUTTEXTMANAGER_H
#define INPUTTEXTMANAGER_H

#include <QJsonObject>
#include <QObject>

#include "libruqolacore_export.h"

class InputCompleterModel;
class QAbstractItemModel;
class QSortFilterProxyModel;

class LIBRUQOLACORE_EXPORT InputTextManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(InputCompleterModel* inputCompleterModel READ inputCompleterModel CONSTANT)
public:
    enum CompletionForType {
        Channel = 0,
        User,
        Emoji,
        None
    };
    explicit InputTextManager(QObject *parent = nullptr);
    ~InputTextManager() override;

    void setEmoticonModel(QAbstractItemModel *model);

    Q_REQUIRED_RESULT InputCompleterModel *inputCompleterModel() const;
    Q_REQUIRED_RESULT QAbstractItemModel *emojiCompleterModel() const;

    void inputTextCompleter(const QJsonObject &obj);

    Q_INVOKABLE Q_REQUIRED_RESULT QString replaceWord(const QString &newWord, const QString &str, int position);
    Q_REQUIRED_RESULT QString applyCompletion(const QString &newWord, const QString &str, int *pPosition);

    Q_INVOKABLE void setInputTextChanged(const QString &str, int position);

    void clearCompleter();

    //Only for autotests
    Q_REQUIRED_RESULT QString searchWord(const QString &text, int position);

Q_SIGNALS:
    // Trigger autocompletion request in DDPClient (via RocketChatAccount)
    // Emitted with Channel and User, never Emoji or None
    void completionRequested(const QString &pattern, const QString &exceptions, InputTextManager::CompletionForType type);
    void completionTypeChanged(InputTextManager::CompletionForType type);

private:
    Q_DISABLE_COPY(InputTextManager)

    void setCompletionType(CompletionForType type);

    InputCompleterModel *mInputCompleterModel = nullptr;
    QSortFilterProxyModel *mEmoticonFilterProxyModel = nullptr;
    CompletionForType mCurrentCompletionType = None;
};

Q_DECLARE_METATYPE(InputTextManager::CompletionForType)

#endif // INPUTTEXTMANAGER_H
