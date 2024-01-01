/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "switchchannelhistorymodel.h"

SwitchChannelHistoryModel::SwitchChannelHistoryModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

SwitchChannelHistoryModel::~SwitchChannelHistoryModel() = default;

int SwitchChannelHistoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mSwichChannelsInfo.count();
}

QVariant SwitchChannelHistoryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mSwichChannelsInfo.count()) {
        return {};
    }
    const SwitchChannelInfo info = mSwichChannelsInfo.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return info.mName;
    }
    return {};
}

void SwitchChannelHistoryModel::addHistory(const QString &name, const QString &identifier)
{
    const SwitchChannelInfo info{name, identifier};
    if (!mSwichChannelsInfo.isEmpty()) {
        if (mSwichChannelsInfo.at(mSwichChannelsInfo.count() - 1) == info) {
            return;
        }
    }
    if (mSwichChannelsInfo.count() > 10) {
        mSwichChannelsInfo.takeFirst();
    }
    mSwichChannelsInfo.removeAll(info);
    mSwichChannelsInfo.prepend(info);
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(mSwichChannelsInfo.size() - 1, 1), {});
}

const QString SwitchChannelHistoryModel::identifier(int index)
{
    if (index < 0 || index >= mSwichChannelsInfo.count()) {
        return {};
    }
    return mSwichChannelsInfo.at(index).mIdentifier;
}

bool SwitchChannelHistoryModel::SwitchChannelInfo::operator==(const SwitchChannelInfo &other) const
{
    return other.mIdentifier == mIdentifier && other.mName == mName;
}

#include "moc_switchchannelhistorymodel.cpp"
