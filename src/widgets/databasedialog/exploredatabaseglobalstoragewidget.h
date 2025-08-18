/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "exploredatabasebasestoragewidget.h"
#include "localdatabase/globaldatabase.h"
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseGlobalStorageWidget : public ExploreDatabaseBaseStorageWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseGlobalStorageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseGlobalStorageWidget() override;
    void slotLoadModelFromDataBase(const QString &accountName);

private:
    GlobalDatabase *const mLocalGlobalDatabase;
};
