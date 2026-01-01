/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

class MultiStaticSelectLineEditProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MultiStaticSelectLineEditProxyModel(QObject *parent = nullptr);
    ~MultiStaticSelectLineEditProxyModel() override;

    void clear();
};
