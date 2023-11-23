/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QSortFilterProxyModel>
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT CommandsModelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommandsModelFilterProxyModel(RocketChatAccount *account, QObject *parent = nullptr);
    ~CommandsModelFilterProxyModel() override;

    void setRoomId(const QString &roomId);

protected:
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString mRoomId;
    RocketChatAccount *const mRocketChatAccount;
};
