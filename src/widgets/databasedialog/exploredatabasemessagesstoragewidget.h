/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "localdatabase/localmessagesdatabase.h"
#include <QWidget>
class QTableView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseMessagesStorageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseMessagesStorageWidget(QWidget *parent = nullptr);
    ~ExploreDatabaseMessagesStorageWidget() override;

    void slotLoadModelFromDataBase(const QString &accountName, const QString &roomName);

private:
    QTableView *const mTableView;
    std::unique_ptr<LocalMessagesDatabase> mLocalMessageDatabase;
    std::unique_ptr<QSqlTableModel> mModel;
};
