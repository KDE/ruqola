/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "exploredatabasebasestoragewidget.h"
#include "localdatabase/localroomsubscriptionsdatabase.h"
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseRoomSubscriptionsWidget : public ExploreDatabaseBaseStorageWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseRoomSubscriptionsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseRoomSubscriptionsWidget() override;
    void slotLoadModelFromDataBase(const QString &accountName);

private:
    LocalRoomSubscriptionsDatabase *const mLocalRoomSubscriptions;
};
