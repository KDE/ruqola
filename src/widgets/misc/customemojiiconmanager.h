/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <TextEmoticonsCore/CustomEmojiIconManager>
class RocketChatAccount;
class CustomEmojiIconManager : public TextEmoticonsCore::CustomEmojiIconManager
{
    Q_OBJECT
public:
    explicit CustomEmojiIconManager(QObject *parent = nullptr);
    ~CustomEmojiIconManager() override;

    Q_REQUIRED_RESULT QIcon generateIcon(const QString &customIdentifier) override;

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

private:
    RocketChatAccount *mCurrentRocketChatAccount = nullptr;
};
