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

class EmojiModelManager::EmojiModelManagerPrivate
{
public:
    EmojiModelManagerPrivate(EmojiModelManager *q)
        : mEmojiModel(new TextEmoticonsCore::EmojiModel(q))
    {
        mEmojiModel->setUnicodeEmoticonList(TextEmoticonsCore::UnicodeEmoticonManager::self()->unicodeEmojiList());
    }
    void loadRecentUsed()
    {
        KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedGroupName);
        mRecentIdentifier = group.readEntry("Recents", QStringList());
    }

    void writeRecentUsed()
    {
        KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedGroupName);
        group.writeEntry("Recents", mRecentIdentifier);
        group.sync();
    }

    TextEmoticonsCore::EmojiModel *const mEmojiModel;
    QStringList mRecentIdentifier;
};

EmojiModelManager::EmojiModelManager(QObject *parent)
    : QObject(parent)
    , d(new EmojiModelManagerPrivate(this))
{
    d->loadRecentUsed();
}

EmojiModelManager::~EmojiModelManager()
{
    d->writeRecentUsed();
}

EmojiModelManager *EmojiModelManager::self()
{
    static EmojiModelManager s_self;
    return &s_self;
}

TextEmoticonsCore::EmojiModel *EmojiModelManager::emojiModel() const
{
    return d->mEmojiModel;
}

const QStringList &EmojiModelManager::recentIdentifier() const
{
    return d->mRecentIdentifier;
}

void EmojiModelManager::setRecentIdentifier(const QStringList &newRecentIdentifier)
{
    d->mRecentIdentifier = newRecentIdentifier;
    d->writeRecentUsed();
    Q_EMIT usedIdentifierChanged(d->mRecentIdentifier);
}

void EmojiModelManager::addIdentifier(const QString &identifier)
{
    if (int i = d->mRecentIdentifier.indexOf(identifier)) {
        // Remove it for adding in top of list
        if (i != -1) {
            d->mRecentIdentifier.removeAt(i);
        }
    }
    d->mRecentIdentifier.prepend(identifier);
    d->writeRecentUsed();
    Q_EMIT usedIdentifierChanged(d->mRecentIdentifier);
}

CustomEmojiIconManager *EmojiModelManager::customEmojiIconManager() const
{
    return d->mEmojiModel->customEmojiIconManager();
}

void EmojiModelManager::setCustomEmojiIconManager(CustomEmojiIconManager *newCustomEmojiIconManager)
{
    d->mEmojiModel->setCustomEmojiIconManager(newCustomEmojiIconManager);
}

#include "moc_emojimodelmanager.cpp"
