/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multistaticselectlineeditproxymodel.h"
#include "multistaticselectlineeditmodel.h"

MultiStaticSelectLineEditProxyModel::MultiStaticSelectLineEditProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(MultiStaticSelectLineEditModel::Text);
    sort(0);
}

MultiStaticSelectLineEditProxyModel::~MultiStaticSelectLineEditProxyModel() = default;

#include "moc_multistaticselectlineeditproxymodel.cpp"
