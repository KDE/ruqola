/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "exploredatabasebasestoragewidget.h"
#include "libruqolawidgets_private_export.h"
#include "localdatabase/localmessagesdatabase.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseMessagesStorageWidget : public ExploreDatabaseBaseStorageWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseMessagesStorageWidget(QWidget *parent = nullptr);
    ~ExploreDatabaseMessagesStorageWidget() override;

    void slotLoadModelFromDataBase(const QString &accountName, const QString &roomName);

private:
    std::unique_ptr<LocalMessagesDatabase> mLocalMessageDatabase;
};
