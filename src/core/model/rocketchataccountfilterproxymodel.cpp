/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountfilterproxymodel.h"
#include "rocketchataccountmodel.h"

RocketChatAccountFilterProxyModel::RocketChatAccountFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(RocketChatAccountModel::Name);
    sort(0);
}

RocketChatAccountFilterProxyModel::~RocketChatAccountFilterProxyModel() = default;
