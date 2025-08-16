/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QSqlTableModel>
#include <QWidget>
#include <memory.h>
class QTableView;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseBaseStorageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseBaseStorageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseBaseStorageWidget() override;

protected:
    std::unique_ptr<QSqlTableModel> mModel;
    QTableView *const mTableView;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
