/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

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
}

bool RuqolaQuickSearchMessageSettings::canSearchMessage() const
{
    if (!mMessageModel) {
        qCWarning(RUQOLA_QUICK_SEARCH_LOG) << " Model is not defined. It's a bug";
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
#if 0
    auto msg = mMessageModel->messages().constLast();
    if (msg.isValid()) {
        mCurrentMessageIdentifier = msg.uuid();
    } else {
        qCWarning(RUQOLA_QUICK_SEARCH_LOG) << "Invalid message. It's a bug";
    }
#endif
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
#if 0
    // qDebug() << " mCurrentSearchIndex " << mCurrentSearchIndex << " mMessageModel->message(mCurrentMessageIdentifier).numberOfTextSearched() "
    //          << mMessageModel->message(mCurrentMessageIdentifier).numberOfTextSearched();
    if (mCurrentSearchIndex >= mMessageModel->message(mCurrentMessageIdentifier).numberOfTextSearched()) {
        auto hasSearchedString = [](const TextAutoGenerateMessage &msg) {
            return msg.numberOfTextSearched() > 0;
        };

        mCurrentSearchIndex = 0;
        auto msg = mMessageModel->findNextMessageAfter(mCurrentMessageIdentifier, hasSearchedString);
        if (msg.isValid()) {
            mCurrentMessageIdentifier = msg.uuid();
        } else {
            mCurrentSearchIndex = storeCurrentSearchIndex;
            Q_EMIT updateNextPreviousButtons(false, true /*TODO ????*/);
            // Invalidate it.
            // clear();
            return;
        }
        Q_EMIT updateNextPreviousButtons((msg.numberOfTextSearched() > 0), true);
    } else {
        Q_EMIT updateNextPreviousButtons((mMessageModel->message(mCurrentMessageIdentifier).numberOfTextSearched() > 0), true);
    }
    Q_EMIT refreshMessage(mCurrentMessageIdentifier, previousMessageIdentifier, mCurrentSearchIndex);
#endif
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
#if 0
            auto hasSearchedString = [](const TextAutoGenerateMessage &msg) {
                return msg.numberOfTextSearched() > 0;
            };

            auto msg = mMessageModel->findLastMessageBefore(mCurrentMessageIdentifier, hasSearchedString);
            if (msg.isValid()) {
                mCurrentMessageIdentifier = msg.uuid();
                mCurrentSearchIndex = msg.numberOfTextSearched() - 1;
            } else {
                // Keep in 0 index
                mCurrentSearchIndex = 0;
                // Invalidate it.
                // clear();
                Q_EMIT updateNextPreviousButtons(true, false); // TODO verify it
                return;
            }
            Q_EMIT updateNextPreviousButtons((msg.numberOfTextSearched() > 0), true);
#endif
        } else {
            Q_EMIT updateNextPreviousButtons(true, true);
        }
    }
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

#include "moc_ruqolaquicksearchmessagesettings.cpp"
