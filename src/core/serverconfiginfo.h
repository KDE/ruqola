/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT ServerConfigInfo : public QObject
{
    Q_OBJECT
public:
    explicit ServerConfigInfo(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~ServerConfigInfo() override;
    Q_REQUIRED_RESULT QString serverVersionStr() const;
    Q_REQUIRED_RESULT QString serverName() const;
    Q_REQUIRED_RESULT QString userName() const;
    Q_REQUIRED_RESULT QString serverUrl() const;
    Q_REQUIRED_RESULT QString logoUrl() const;
    Q_REQUIRED_RESULT QString faviconUrl() const;

private:
    Q_DISABLE_COPY(ServerConfigInfo)
    RocketChatAccount *const mAccount;
};

