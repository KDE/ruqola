/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "switchchannelhistorymodel.h"

SwitchChannelHistoryModel::SwitchChannelHistoryModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

SwitchChannelHistoryModel::~SwitchChannelHistoryModel()
{
}

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
    //    switch (role) {
    //    }
    return {};
}

void SwitchChannelHistoryModel::addHistory(const QString &name, const QString &identifier)
{
    if (mSwichChannelsInfo.count() > 10) {
        mSwichChannelsInfo.takeFirst();
    }
    mSwichChannelsInfo.append({name, identifier});
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(mSwichChannelsInfo.size() - 1, 1), {});
}

const QString SwitchChannelHistoryModel::identifier(int index)
{
    if (index < 0 || index >= mSwichChannelsInfo.count()) {
        return {};
    }
    return mSwichChannelsInfo.at(index).mIdentifier;
}
