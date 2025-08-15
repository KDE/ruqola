/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "localdatabase/localaccountsdatabase.h"
#include <QWidget>
class QTableView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseAccountStorageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseAccountStorageWidget(QWidget *parent = nullptr);
    ~ExploreDatabaseAccountStorageWidget() override;
    void slotLoadModelFromDataBase();

private:
    QTableView *const mTableView;
    std::unique_ptr<LocalAccountsDatabase> mLocalAccountsDatabase;
};
