/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagechannelmodel.h"

ForwardMessageChannelModel::ForwardMessageChannelModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

ForwardMessageChannelModel::~ForwardMessageChannelModel() = default;

int ForwardMessageChannelModel::rowCount(const QModelIndex &parent) const
{
    // TODO
    return 0;
}

QVariant ForwardMessageChannelModel::data(const QModelIndex &index, int role) const
{
    // TODO
    return {};
}
