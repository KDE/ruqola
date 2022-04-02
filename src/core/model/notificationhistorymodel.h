/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractItemModel>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT NotificationHistoryModel : public QAbstractItemModel
{
public:
    explicit NotificationHistoryModel(QObject *parent = nullptr);
    ~NotificationHistoryModel() override;
};
