/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT RocketChatAccountFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RocketChatAccountFilterProxyModel(QObject *parent = nullptr);

    ~RocketChatAccountFilterProxyModel() override;
};

