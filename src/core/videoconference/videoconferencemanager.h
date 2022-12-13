/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QJsonArray>
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT VideoConferenceManager : public QObject
{
    Q_OBJECT
public:
    explicit VideoConferenceManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~VideoConferenceManager() override;

    void parseVideoConference(const QJsonArray &contents);

private:
    RocketChatAccount *const mRocketChatAccount;
};
