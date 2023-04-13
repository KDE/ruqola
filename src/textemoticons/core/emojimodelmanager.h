/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "textemoticonscore_export.h"
#include <QObject>
namespace TextEmoticonsCore
{
class EmojiModel;
class CustomEmojiIconManager;
class TEXTEMOTICONSCORE_EXPORT EmojiModelManager : public QObject
{
    Q_OBJECT
public:
    explicit EmojiModelManager(QObject *parent = nullptr);
    ~EmojiModelManager() override;

    static EmojiModelManager *self();
    Q_REQUIRED_RESULT TextEmoticonsCore::EmojiModel *emojiModel() const;

    Q_REQUIRED_RESULT const QStringList &recentIdentifier() const;
    void setRecentIdentifier(const QStringList &newRecentIdentifier);
    void addIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT TextEmoticonsCore::CustomEmojiIconManager *customEmojiIconManager() const;
    void setCustomEmojiIconManager(TextEmoticonsCore::CustomEmojiIconManager *newCustomEmojiIconManager);

Q_SIGNALS:
    void usedIdentifierChanged(const QStringList &lst);

private:
    TextEmoticonsCore::EmojiModel *const mEmojiModel;
    QStringList mRecentIdentifier;
    void loadRecentUsed();
    void writeRecentUsed();
};
}
