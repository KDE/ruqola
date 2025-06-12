/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractProxyModel>

class UsersForRoomListHeadingsProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
public:
    explicit UsersForRoomListHeadingsProxyModel(QObject *parent = nullptr);
    ~UsersForRoomListHeadingsProxyModel() override;
};
