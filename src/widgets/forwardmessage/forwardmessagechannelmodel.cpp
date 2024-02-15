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
