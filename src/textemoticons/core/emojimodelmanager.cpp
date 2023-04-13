/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojimodelmanager.h"

#include "unicodeemoticonmanager.h"
#include <KConfigGroup>
#include <KSharedConfig>
#include <TextEmoticonsCore/EmojiModel>
using namespace TextEmoticonsCore;
namespace
{
static const char myEmoticonRecentUsedGroupName[] = "EmoticonRecentUsed";
}
EmojiModelManager::EmojiModelManager(QObject *parent)
    : QObject(parent)
    , mEmojiModel(new TextEmoticonsCore::EmojiModel(this))
{
    mEmojiModel->setUnicodeEmoticonList(TextEmoticonsCore::UnicodeEmoticonManager::self()->unicodeEmojiList());
    loadRecentUsed();
}

EmojiModelManager::~EmojiModelManager()
{
    writeRecentUsed();
}

EmojiModelManager *EmojiModelManager::self()
{
    static EmojiModelManager s_self;
    return &s_self;
}

TextEmoticonsCore::EmojiModel *EmojiModelManager::emojiModel() const
{
    return mEmojiModel;
}

const QStringList &EmojiModelManager::recentIdentifier() const
{
    return mRecentIdentifier;
}

void EmojiModelManager::setRecentIdentifier(const QStringList &newRecentIdentifier)
{
    mRecentIdentifier = newRecentIdentifier;
    writeRecentUsed();
    Q_EMIT usedIdentifierChanged(mRecentIdentifier);
}

void EmojiModelManager::addIdentifier(const QString &identifier)
{
    if (int i = mRecentIdentifier.indexOf(identifier)) {
        // Remove it for adding in top of list
        if (i != -1) {
            mRecentIdentifier.removeAt(i);
        }
    }
    mRecentIdentifier.prepend(identifier);
    writeRecentUsed();
    Q_EMIT usedIdentifierChanged(mRecentIdentifier);
}

CustomEmojiIconManager *EmojiModelManager::customEmojiIconManager() const
{
    return mEmojiModel->customEmojiIconManager();
}

void EmojiModelManager::setCustomEmojiIconManager(CustomEmojiIconManager *newCustomEmojiIconManager)
{
    mEmojiModel->setCustomEmojiIconManager(newCustomEmojiIconManager);
}

void EmojiModelManager::loadRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedGroupName);
    mRecentIdentifier = group.readEntry("Recents", QStringList());
}

void EmojiModelManager::writeRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedGroupName);
    group.writeEntry("Recents", mRecentIdentifier);
    group.sync();
}
