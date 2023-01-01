/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inputtextmanager.h"
#include "connection.h"
#include "misc/directoryjob.h"
#include "model/commandsmodel.h"
#include "model/commandsmodelfilterproxymodel.h"
#include "model/emoticonmodelfilterproxymodel.h"
#include "ownuser/ownuserpreferences.h"
#include "rocketchataccount.h"
#include "ruqola_completion_debug.h"
#include "ruqola_debug.h"

InputTextManager::InputTextManager(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mInputCompleterModel(new InputCompleterModel(this))
    , mEmoticonFilterProxyModel(new EmoticonModelFilterProxyModel(this))
    , mCommandFilterProxyModel(new CommandsModelFilterProxyModel(this))
    , mAccount(account)
{
}

InputTextManager::~InputTextManager() = default;

void InputTextManager::setEmoticonModel(QAbstractItemModel *model)
{
    mEmoticonFilterProxyModel->setSourceModel(model);
}

void InputTextManager::setCommandModel(QAbstractItemModel *model)
{
    mCommandFilterProxyModel->setSourceModel(model);
}

QAbstractItemModel *InputTextManager::commandModel() const
{
    return mCommandFilterProxyModel;
}

QString InputTextManager::applyCompletion(const QString &newWord, const QString &text, int *pPosition)
{
    if (newWord.isEmpty()) {
        qCDebug(RUQOLA_COMPLETION_LOG) << "Empty newWord";
        return text;
    }
    if (text.isEmpty()) {
        qCDebug(RUQOLA_COMPLETION_LOG) << "Empty text";
        return text;
    }
    int position = *pPosition;
    // Cursor position can be at the end of word => text.length
    if ((position > text.length()) || (position < 0)) {
        qCDebug(RUQOLA_COMPLETION_LOG) << "Invalid position" << position;
        return text;
    }

    int start = 0;
    for (int i = position - 1; i >= 0; i--) {
        if (i == 0) {
            start = 1;
            break;
        }
        if (text.at(i).isSpace()) {
            // Don't replace # or @
            start = i + 2;
            break;
        }
    }
    int end = text.length() - 1;
    for (int i = position; i < text.length(); i++) {
        if (text.at(i).isSpace()) {
            end = i;
            if (!newWord.endsWith(QLatin1Char(' '))) {
                --end;
            }
            break;
        }
    }
    QString replaceText = text;
    replaceText.replace(start, end - start + 1, newWord);
    *pPosition = start + newWord.length();
    return replaceText;
}

QString InputTextManager::searchWord(const QString &text, int position, int &start)
{
    if (text.isEmpty()) {
        return {};
    }
    // Cursor position can be at the end of word => text.length
    if ((position > text.length()) || (position < 0)) {
        qCWarning(RUQOLA_COMPLETION_LOG) << "Invalid position" << position << " text " << text;
        return {};
    }

    start = 0;
    for (int i = position; i > 0; i--) {
        if (text.at(i - 1).isSpace()) {
            start = i;
            break;
        }
    }
    int end = text.length() - 1;
    for (int i = position; i < text.length(); i++) {
        if (text.at(i).isSpace()) {
            end = i;
            break;
        }
    }

    QString word = text.mid(start, end - start + 1);
    if (!word.isEmpty() && word.at(word.length() - 1).isSpace()) {
        word.chop(1);
    }
    // qDebug() << "position" << position << " word " << word << " text " << text << " start " << start << " end " << end;
    return word;
}

void InputTextManager::setCompletionType(InputTextManager::CompletionForType type)
{
    if (type != mCurrentCompletionType) {
        mCurrentCompletionType = type;
        Q_EMIT completionTypeChanged(type);
    }
}

void InputTextManager::slotCompletionChannels(const QString &pattern)
{
    if (!pattern.isEmpty()) {
        auto job = new RocketChatRestApi::DirectoryJob(this);
        RocketChatRestApi::DirectoryJob::DirectoryInfo info;
        info.pattern = pattern;
        info.searchType = RocketChatRestApi::DirectoryJob::Rooms;
        job->setDirectoryInfo(info);
        mAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, this, &InputTextManager::slotCompletionChannelDone);
        if (!job->start()) {
            qCWarning(RUQOLA_COMPLETION_LOG) << "Impossible to start searchRoomUser job";
        }
    }
}

void InputTextManager::slotCompletionChannelDone(const QJsonObject &root)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    inputTextCompleter(obj);
}

void InputTextManager::setInputTextChanged(const QString &text, int position)
{
    if (text.isEmpty()) {
        clearCompleter();
        return;
    }
    qCDebug(RUQOLA_COMPLETION_LOG) << "calling searchWord(" << text << "," << position << ")";
    int start = -1;
    const QString word = searchWord(text, position, start);
    const QString str = word.mid(1);
    qCDebug(RUQOLA_COMPLETION_LOG) << " str:" << str << "start:" << start << "word:" << word << "position:" << position;
    if (word.isEmpty() || position != start + word.length()) { // only trigger completion at the end of the word
        clearCompleter();
    } else {
        if (word.startsWith(QLatin1Char('@'))) {
            // Trigger autocompletion request in DDPClient (via RocketChatAccount)
            setCompletionType(InputTextManager::CompletionForType::User);
            mCurrentCompletionPattern = str;
            if (str.isEmpty()) {
                mInputCompleterModel->setDefaultUserCompletion();
            } else {
                Q_EMIT completionRequested(str, QString(), InputTextManager::CompletionForType::User);
            }
        } else if (word.startsWith(QLatin1Char('#'))) {
            // Trigger autocompletion request in DDPClient (via RocketChatAccount)
            mCurrentCompletionPattern = str;
            Q_EMIT completionRequested(str, QString(), InputTextManager::CompletionForType::Channel);
            // slotCompletionChannels(str);
            setCompletionType(InputTextManager::CompletionForType::Channel);
        } else if (word.startsWith(QLatin1Char(':'))) {
            if (mAccount && mAccount->ownUserPreferences().useEmojis()) {
                mEmoticonFilterProxyModel->setFilterFixedString(word);
                setCompletionType(InputTextManager::CompletionForType::Emoji);
            }
        } else if (word.startsWith(QLatin1Char('/')) && position == word.length()) { // "/" must be at beginning of text
            mCommandFilterProxyModel->setFilterFixedString(word);
            setCompletionType(InputTextManager::CompletionForType::Command);
        } else {
            clearCompleter();
        }
    }
}

void InputTextManager::clearCompleter()
{
    mInputCompleterModel->clear();
    setCompletionType(None);
}

// Used by MessageTextEdit to set the completion model for the listview
InputCompleterModel *InputTextManager::inputCompleterModel() const
{
    return mInputCompleterModel;
}

QAbstractItemModel *InputTextManager::emojiCompleterModel() const
{
    return mEmoticonFilterProxyModel;
}

// Called by DDPClient to fill in the completer model based on the typed input
void InputTextManager::inputTextCompleter(const QJsonObject &obj)
{
    mInputCompleterModel->parseChannels(obj);
    // Don't show a popup with exactly the same as the pattern
    // (e.g. type or navigate within @dfaure -> the offer is "dfaure", useless)
    if (mInputCompleterModel->rowCount() == 1) {
        const QString completerName = mInputCompleterModel->index(0, 0).data(InputCompleterModel::CompleterName).toString();
        if (mCurrentCompletionPattern == completerName) {
            clearCompleter();
        }
    }
}
