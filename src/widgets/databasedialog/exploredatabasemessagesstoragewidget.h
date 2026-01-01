/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "exploredatabasebasestoragewidget.h"
#include "libruqolawidgets_private_export.h"
#include "localdatabase/localmessagesdatabase.h"
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseMessagesStorageWidget : public ExploreDatabaseBaseStorageWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseMessagesStorageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseMessagesStorageWidget() override;

    void slotLoadModelFromDataBase(const QString &accountName, const QByteArray &roomId);

private:
    LocalMessagesDatabase *const mLocalMessageDatabase;
};
