/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "exploredatabasebasestoragewidget.h"
#include "localdatabase/e2edatabase.h"
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseE2EStorageWidget : public ExploreDatabaseBaseStorageWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseE2EStorageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseE2EStorageWidget() override;
    void slotLoadModelFromDataBase(const QString &accountName);

private:
    E2EDataBase *const mLocalE2EDatabase;
};
