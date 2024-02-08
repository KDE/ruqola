/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"

#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT E2eKeyManager : public QObject
{
    Q_OBJECT
public:
    explicit E2eKeyManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~E2eKeyManager() override;

private:
    RocketChatAccount *const mAccount;
};
