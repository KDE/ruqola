/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customemojiiconmanager.h"
#include "emoticons/emojimanager.h"
#include "rocketchataccount.h"

CustomEmojiIconManager::CustomEmojiIconManager(QObject *parent)
    : TextEmoticonsCore::CustomEmojiIconManager{parent}
{
}

CustomEmojiIconManager::~CustomEmojiIconManager()
{
}

QIcon CustomEmojiIconManager::generateIcon(const QString &customIdentifier)
{
    if (mCurrentRocketChatAccount) {
        const QString fileName = mCurrentRocketChatAccount->emojiManager()->customEmojiFileName(customIdentifier);
        //        qDebug() << " mCurrentRocketChatAccount " << mCurrentRocketChatAccount->accountName();
        //        qDebug() << " fileName " << fileName << "customIdentifier " << customIdentifier;
        if (!fileName.isEmpty()) {
            const QUrl emojiUrl = mCurrentRocketChatAccount->attachmentUrlFromLocalCache(fileName);
            //            qDebug() << " emojiUrl " << emojiUrl;
            if (!emojiUrl.isEmpty()) {
                const QIcon icon(emojiUrl.toLocalFile());
                return icon;
            }
        }
    }
    return {};
}

void CustomEmojiIconManager::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mCurrentRocketChatAccount = currentRocketChatAccount;
}

RocketChatAccount *CustomEmojiIconManager::currentRocketChatAccount() const
{
    return mCurrentRocketChatAccount;
}
