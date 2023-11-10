/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfohistorymodel.h"
#include <KLocalizedString>

ServerErrorInfoHistoryModel::ServerErrorInfoHistoryModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

ServerErrorInfoHistoryModel::~ServerErrorInfoHistoryModel() = default;

int ServerErrorInfoHistoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mServerErrorInfo.count();
}

QVariant ServerErrorInfoHistoryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mServerErrorInfo.count()) {
        return {};
    }
    const auto info = mServerErrorInfo.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case AccountName:
        return info.accountName();
    case MessageStr:
        return info.message();
    case Identifier:
        return info.identifier();
    case DateTime:
        return info.dateTime();
    case DateTimeStr:
        return info.dateTimeStr();
    }
    return {};
}

void ServerErrorInfoHistoryModel::clear()
{
    if (!mServerErrorInfo.isEmpty()) {
        beginResetModel();
        mServerErrorInfo.clear();
        endResetModel();
    }
}

void ServerErrorInfoHistoryModel::insertServerErrorInfos(const QVector<ServerErrorInfo> &infos)
{
    clear();
    if (!infos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, infos.count() - 1);
        mServerErrorInfo = infos;
        endInsertRows();
    }
}

void ServerErrorInfoHistoryModel::addServerErrorInfo(const ServerErrorInfo &info)
{
    const int numberOfElement = mServerErrorInfo.count();
    mServerErrorInfo.append(info);
    beginInsertRows(QModelIndex(), numberOfElement, mServerErrorInfo.count() - 1);
    endInsertRows();
}

#include "moc_servererrorinfohistorymodel.cpp"
