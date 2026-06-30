/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "readreceiptsmodel.h"

#include "roles/roleinfo.h"
#include "ruqola_debug.h"
#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;
ReadReceiptsModel::ReadReceiptsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

ReadReceiptsModel::~ReadReceiptsModel() = default;

int ReadReceiptsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }
    // TODO
    return {};
}

QVariant ReadReceiptsModel::data(const QModelIndex &index, int role) const
{
#if 0 // TODO
    if (index.row() < 0 || index.row() >= mStatusList.count()) {
        return {};
    }
    const DisplayStatusInfo statusInfo = mStatusList.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case StatusI18n:
        return statusInfo.displayText;
    case Status:
        return QVariant::fromValue(statusInfo.status);
    case Order:
        return statusInfo.order;
    case Qt::DecorationRole:
    case Icon:
        return statusInfo.icon;
    default:
        break;
    }
#endif
    return {};
}

#include "moc_readreceiptsmodel.cpp"
