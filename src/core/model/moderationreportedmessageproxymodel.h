/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "searchtreebasefilterproxymodel.h"

class LIBRUQOLACORE_EXPORT ModerationReportedMessageProxyModel : public SearchTreeBaseFilterProxyModel
{
    Q_OBJECT
public:
    explicit ModerationReportedMessageProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~ModerationReportedMessageProxyModel() override;

    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
