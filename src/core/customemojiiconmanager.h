/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <TextEmoticonsCore/CustomEmojiIconManager>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT CustomEmojiIconManager : public TextEmoticonsCore::CustomEmojiIconManager
{
    Q_OBJECT
public:
    explicit CustomEmojiIconManager(QObject *parent = nullptr);
    ~CustomEmojiIconManager() override;

    Q_REQUIRED_RESULT QIcon generateIcon(const QString &customIdentifier) override;

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

    RocketChatAccount *currentRocketChatAccount() const;

private:
    RocketChatAccount *mCurrentRocketChatAccount = nullptr;
};
