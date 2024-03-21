/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfoproxymodel.h"

DeviceInfoProxyModel::DeviceInfoProxyModel(QAbstractListModel *model, QObject *parent)
    : SearchTreeBaseFilterProxyModel(model, parent)
{
}

DeviceInfoProxyModel::~DeviceInfoProxyModel() = default;
