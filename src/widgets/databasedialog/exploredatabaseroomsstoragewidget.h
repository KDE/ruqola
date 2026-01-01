/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "exploredatabasebasestoragewidget.h"
#include "localdatabase/localroomsdatabase.h"
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseRoomsStorageWidget : public ExploreDatabaseBaseStorageWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseRoomsStorageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseRoomsStorageWidget() override;
    void slotLoadModelFromDataBase(const QString &accountName);

private:
    LocalRoomsDatabase *const mLocalRoomsDatabase;
};
