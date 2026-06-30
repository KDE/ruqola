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
    return mReadReceipts.count();
}

QVariant ReadReceiptsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mReadReceipts.count()) {
        return {};
    }
    const ReadReceipt receiptInfo = mReadReceipts.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case ReadReceiptsModel::ReadReceiptsInfo::UserName:
        return receiptInfo.userName();
    case ReadReceiptsModel::ReadReceiptsInfo::UserId:
        return receiptInfo.userId();
    case ReadReceiptsModel::ReadReceiptsInfo::Name:
        return receiptInfo.name();
    case ReadReceiptsModel::ReadReceiptsInfo::TimeStamp:
        return receiptInfo.timeStamp();
    default:
        break;
    }
    return {};
}

ReadReceipts ReadReceiptsModel::readReceipts() const
{
    return mReadReceipts;
}

void ReadReceiptsModel::setReadReceipts(const ReadReceipts &newReadReceipts)
{
    beginInsertRows(QModelIndex(), 0, mReadReceipts.count() - 1);
    mReadReceipts = newReadReceipts;
    endInsertRows();
}

#include "moc_readreceiptsmodel.cpp"
