/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "ruqolaquicksearchmessagesettings.h"
#include "model/messagesmodel.h"
#include "ruqola_quick_search_debug.h"

RuqolaQuickSearchMessageSettings::RuqolaQuickSearchMessageSettings(MessagesModel *model, QObject *parent)
    : QObject(parent)
    , mMessageModel(model)
{
}

RuqolaQuickSearchMessageSettings::~RuqolaQuickSearchMessageSettings() = default;

void RuqolaQuickSearchMessageSettings::clear()
{
    mCurrentMessageIdentifier.clear();
    mCurrentSearchIndex = -1;
    mFoundSearchCount = -1;
}

bool RuqolaQuickSearchMessageSettings::canSearchMessage() const
{
    if (!mMessageModel) {
        qCWarning(RUQOLA_QUICK_SEARCH_LOG) << " Model is not defined. It's a bug";
        return false;
    }
    if (mFoundSearchCount == -1) {
        qCWarning(RUQOLA_QUICK_SEARCH_LOG) << "mFoundSearchCount == -1 it's a bug";
        return false;
    }
    if (mCurrentMessageIdentifier.isEmpty()) {
        if (mMessageModel->isEmpty()) {
            return false;
        }
    }
    return true;
}

void RuqolaQuickSearchMessageSettings::lastMessageUuid()
{
    const QByteArray messageId = mMessageModel->lastMessageId();
    mCurrentMessageIdentifier = messageId;
}

void RuqolaQuickSearchMessageSettings::next()
{
    if (!canSearchMessage()) {
        return;
    }
    const QByteArray previousMessageIdentifier = mCurrentMessageIdentifier;
    if (mCurrentMessageIdentifier.isEmpty()) {
        lastMessageUuid();
        if (mCurrentMessageIdentifier.isEmpty()) {
            Q_EMIT updateNextPreviousButtons(false, false);
            return;
        }
    }
    const int storeCurrentSearchIndex = mCurrentSearchIndex;
    if (mCurrentSearchIndex == -1) {
        mCurrentSearchIndex = 0;
    } else {
        mCurrentSearchIndex++;
    }
    // qDebug() << " mCurrentSearchIndex " << mCurrentSearchIndex << " mMessageModel->message(mCurrentMessageIdentifier).numberOfTextSearched() "
    //          << mMessageModel->message(mCurrentMessageIdentifier).numberOfTextSearched();
    const auto hasSearchedString = [](const Message &msg) {
        return msg.numberOfTextSearched() > 0;
    };
    if (mCurrentSearchIndex >= mMessageModel->findMessageById(mCurrentMessageIdentifier).numberOfTextSearched()) {
        mCurrentSearchIndex = 0;
        auto msg = mMessageModel->findNextMessageAfter(mCurrentMessageIdentifier, hasSearchedString);
        if (!msg.messageId().isEmpty()) {
            mCurrentMessageIdentifier = msg.messageId();
        } else {
            mCurrentSearchIndex = storeCurrentSearchIndex;
            Q_EMIT updateNextPreviousButtons(false, mFoundSearchCount < 2 ? false : true);
            // Invalidate it.
            // clear();
            return;
        }
        Q_EMIT updateNextPreviousButtons((msg.numberOfTextSearched() > 0), mFoundSearchCount < 2 ? false : true);
    } else {
        Q_EMIT updateNextPreviousButtons(((mMessageModel->findMessageById(mCurrentMessageIdentifier).numberOfTextSearched() > 0)
                                          && (mCurrentSearchIndex < mMessageModel->findMessageById(mCurrentMessageIdentifier).numberOfTextSearched() - 1))
                                             || !mMessageModel->findNextMessageAfter(mCurrentMessageIdentifier, hasSearchedString).messageId().isEmpty(),
                                         true);
    }
    Q_EMIT refreshMessage(mCurrentMessageIdentifier, previousMessageIdentifier, mCurrentSearchIndex);
}

void RuqolaQuickSearchMessageSettings::previous()
{
    if (!canSearchMessage()) {
        return;
    }
    const QByteArray previousMessageIdentifier = mCurrentMessageIdentifier;
    if (mCurrentMessageIdentifier.isEmpty()) {
        lastMessageUuid();
        if (mCurrentMessageIdentifier.isEmpty()) {
            Q_EMIT updateNextPreviousButtons(false, false);
            return;
        }
    }
    if (mCurrentSearchIndex == -1) {
        mCurrentSearchIndex = 0;
    } else {
        mCurrentSearchIndex--;
        if (mCurrentSearchIndex < 0) {
            const auto hasSearchedString = [](const Message &msg) {
                return msg.numberOfTextSearched() > 0;
            };

            auto msg = mMessageModel->findLastMessageBefore(mCurrentMessageIdentifier, hasSearchedString);
            if (!msg.messageId().isEmpty()) {
                mCurrentMessageIdentifier = msg.messageId();
                mCurrentSearchIndex = msg.numberOfTextSearched() - 1;
            } else {
                // Keep in 0 index
                mCurrentSearchIndex = 0;
                // Invalidate it.
                // clear();
                Q_EMIT updateNextPreviousButtons(mFoundSearchCount < 2 ? false : true, false);
                return;
            }
            Q_EMIT updateNextPreviousButtons((msg.numberOfTextSearched() > 0),
                                             (mCurrentSearchIndex > 0)
                                                 || !mMessageModel->findLastMessageBefore(mCurrentMessageIdentifier, hasSearchedString).messageId().isEmpty());
        } else {
            Q_EMIT updateNextPreviousButtons((mFoundSearchCount > 1), mFoundSearchCount < 2 ? false : true);
        }
    }
    Q_EMIT refreshMessage(mCurrentMessageIdentifier, previousMessageIdentifier, mCurrentSearchIndex);
}

QByteArray RuqolaQuickSearchMessageSettings::currentMessageIdentifier() const
{
    return mCurrentMessageIdentifier;
}

void RuqolaQuickSearchMessageSettings::setCurrentMessageIdentifier(const QByteArray &newCurrentMessageIdentifier)
{
    mCurrentMessageIdentifier = newCurrentMessageIdentifier;
}

int RuqolaQuickSearchMessageSettings::currentSearchIndex() const
{
    return mCurrentSearchIndex;
}

void RuqolaQuickSearchMessageSettings::setCurrentSearchIndex(int newCurrentSearchIndex)
{
    mCurrentSearchIndex = newCurrentSearchIndex;
}

int RuqolaQuickSearchMessageSettings::foundSearchCount() const
{
    return mFoundSearchCount;
}

void RuqolaQuickSearchMessageSettings::setFoundSearchCount(int newFoundSearchCount)
{
    mFoundSearchCount = newFoundSearchCount;
}

#include "moc_ruqolaquicksearchmessagesettings.cpp"
