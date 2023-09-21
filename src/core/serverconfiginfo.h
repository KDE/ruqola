/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    [[nodiscard]] QString serverVersionStr() const;
    [[nodiscard]] QString accountName() const;
    [[nodiscard]] QString userName() const;
    [[nodiscard]] QString serverUrl() const;
    [[nodiscard]] QString logoUrl() const;
    [[nodiscard]] QString faviconUrl() const;

private:
    Q_DISABLE_COPY(ServerConfigInfo)
    RocketChatAccount *const mAccount;
};
