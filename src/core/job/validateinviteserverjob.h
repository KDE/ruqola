/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "accountmanager.h"
#include "libruqolacore_export.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT ValidateInviteServerJob : public QObject
{
    Q_OBJECT
public:
    explicit ValidateInviteServerJob(QObject *parent = nullptr);
    ~ValidateInviteServerJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] AccountManager::AccountManagerInfo info() const;
    void setInfo(const AccountManager::AccountManagerInfo &newInfo);

Q_SIGNALS:
    void tokenIsValid(const AccountManager::AccountManagerInfo &info);
    void tokenIsInvalid();

private:
    AccountManager::AccountManagerInfo mInfo;
};
