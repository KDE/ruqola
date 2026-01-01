/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ExploreDatabaseViewMessagesWidget;
class RocketChatAccount;
class ExploreDatabaseJsonWidget;
class ExploreDatabaseMessagesStorageWidget;
class ExploreDatabaseAccountStorageWidget;
class ExploreDatabaseRoomsStorageWidget;
class ExploreDatabaseGlobalStorageWidget;
class ExploreDatabaseE2EStorageWidget;
class ExploreDatabaseRoomSubscriptionsWidget;
class ExploreDatabaseRoomsPendingTypedInfoStorageWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExploreDatabaseDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseDialog() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ExploreDatabaseViewMessagesWidget *const mExploreDatabaseWidget;
    ExploreDatabaseJsonWidget *const mExploreDatabaseJsonWidget;
    ExploreDatabaseMessagesStorageWidget *const mExploreDatabaseMessagesStorageWidget;
    ExploreDatabaseAccountStorageWidget *const mExploreDatabaseAccountStorageWidget;
    ExploreDatabaseRoomsStorageWidget *const mExploreDatabaseRoomsStorageWidget;
    ExploreDatabaseGlobalStorageWidget *const mExploreDatabaseGlobalStorageWidget;
    ExploreDatabaseE2EStorageWidget *const mExploreDatabaseE2EStorageWidget;
    ExploreDatabaseRoomsPendingTypedInfoStorageWidget *const mExploreDatabaseRoomsPendingTypedInfoStorageWidget;
    ExploreDatabaseRoomSubscriptionsWidget *const mExploreDatabaseRoomSubscriptionsWidget;
};
