/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

#include "config-ruqola.h"
#include "libruqolacore_export.h"
#if HAS_ACTIVITY_SUPPORT
class ActivitiesManager;
#endif
class LIBRUQOLACORE_EXPORT RocketChatAccountFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RocketChatAccountFilterProxyModel(QObject *parent = nullptr);

    ~RocketChatAccountFilterProxyModel() override;

    [[nodiscard]] QStringList accountOrder() const;
    void setAccountOrder(const QStringList &newAccountOrder);

#if HAS_ACTIVITY_SUPPORT
    [[nodiscard]] ActivitiesManager *activitiesManager() const;
    void setActivitiesManager(ActivitiesManager *newActivitiesManager);
#endif

protected:
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QStringList mAccountOrder;
#if HAS_ACTIVITY_SUPPORT
    ActivitiesManager *mActivitiesManager = nullptr;
#endif
};
