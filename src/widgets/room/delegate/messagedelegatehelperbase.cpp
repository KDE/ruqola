/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperbase.h"

#include "delegateutils/messagedelegateutils.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "textconverter.h"
#include <QListView>

MessageDelegateHelperBase::MessageDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : TextUiBase(textSelectionImpl, view)
    , mRocketChatAccount(account)
{
    connect(mTextSelectionImpl->textSelection(), &TextSelection::repaintNeeded, this, &MessageDelegateHelperBase::updateView);
    TextUiBase::setCacheMaxEntries(32); // Enough?
}

MessageDelegateHelperBase::~MessageDelegateHelperBase() = default;

void MessageDelegateHelperBase::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

void MessageDelegateHelperBase::updateView(const QModelIndex &index)
{
    // qDebug() << " void MessageDelegateHelperText::updateView(const QModelIndex &index)" << index;
    mListView->update(index);
}

void MessageDelegateHelperBase::removeMessageCache(const QByteArray &messageId)
{
    TextUiBase::removeMessageCache(messageId);
}

void MessageDelegateHelperBase::clearTextDocumentCache()
{
    TextUiBase::clearCache();
}

QSize MessageDelegateHelperBase::documentDescriptionForIndexSize(const DocumentDescriptionInfo &info) const
{
    auto *doc = documentDescriptionForIndex(info);
    // Add +10 as if we use only doc->idealWidth() it's too small and it creates a new line.
    return doc ? QSize(doc->idealWidth() + 10, doc->size().height()) : QSize();
}

QTextDocument *MessageDelegateHelperBase::documentDescriptionForIndex(const DocumentDescriptionInfo &info) const
{
    auto it = mDocumentCache.find(info.documentId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (info.width != -1 && !qFuzzyCompare(ret->textWidth(), info.width)) {
            ret->setTextWidth(info.width);
        }
        return ret;
    }

    if (info.description.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    QByteArray needUpdateMessageId; // TODO use it ?
    // Laurent Ruqola::self()->rocketChatAccount() only for test.
    auto account = mRocketChatAccount ? mRocketChatAccount : Ruqola::self()->rocketChatAccount();
    int maximumRecursiveQuotedText = -1;
    if (account) {
        maximumRecursiveQuotedText = account->ruqolaServerConfig()->messageQuoteChainLimit();
    }
    const TextConverter::ConvertMessageTextSettings settings(info.description,
                                                             account ? account->userName() : QString(),
                                                             {},
                                                             account ? account->highlightWords() : QStringList(),
                                                             account ? account->emojiManager() : nullptr,
                                                             account ? account->messageCache() : nullptr,
                                                             {},
                                                             {},
                                                             {},
                                                             mSearchText,
                                                             maximumRecursiveQuotedText);

    int recursiveIndex = 0;
    int numberOfTextSearched = 0;
    int hightLightStringIndex = 0;
    const QString contextString = TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex);
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, info.width);
    auto ret = doc.get();
    mDocumentCache.insert(info.documentId, std::move(doc));
    return ret;
}

void MessageDelegateHelperBase::setSearchText(const QString &newSearchText)
{
    mSearchText = newSearchText;
}

QString MessageDelegateHelperBase::searchText() const
{
    return mSearchText;
}

QDebug operator<<(QDebug d, const MessageDelegateHelperBase::DocumentDescriptionInfo &t)
{
    d.space() << "documentId:" << t.documentId;
    d.space() << "description:" << t.description;
    d.space() << "width:" << t.width;
    return d;
}
