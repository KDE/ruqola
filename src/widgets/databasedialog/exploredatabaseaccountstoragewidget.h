/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "exploredatabasebasestoragewidget.h"
class RocketChatAccount;
class LocalAccountsDatabase;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseAccountStorageWidget : public ExploreDatabaseBaseStorageWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseAccountStorageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseAccountStorageWidget() override;
    void slotLoadModelFromDataBase(const QString &accountName);

private:
    LocalAccountsDatabase *const mLocalAccountsDatabase;
};
