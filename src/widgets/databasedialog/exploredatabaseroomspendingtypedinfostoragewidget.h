/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "exploredatabasebasestoragewidget.h"
#include "localdatabase/localroompendingtypedinfodatabase.h"
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseRoomsPendingTypedInfoStorageWidget : public ExploreDatabaseBaseStorageWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseRoomsPendingTypedInfoStorageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseRoomsPendingTypedInfoStorageWidget() override;
    void slotLoadModelFromDataBase(const QString &accountName);

private:
    LocalRoomPendingTypedInfoDatabase *const mLocalRoomsPendingTypedInfo;
};
