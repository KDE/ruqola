/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "exploredatabasebasestoragewidget.h"
#include "localdatabase/e2eroomsdatabase.h"
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseE2ERoomsStorageWidget : public ExploreDatabaseBaseStorageWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseE2ERoomsStorageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseE2ERoomsStorageWidget() override;
    void slotLoadModelFromDataBase(const QString &accountName);

private:
    E2ERoomsDataBase *const mLocalE2ERoomsDatabase;
};
