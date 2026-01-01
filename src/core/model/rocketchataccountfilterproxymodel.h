/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

#include "config-ruqola.h"
#include "libruqolacore_export.h"
#if HAVE_ACTIVITY_SUPPORT
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

#if HAVE_ACTIVITY_SUPPORT
    [[nodiscard]] ActivitiesManager *activitiesManager() const;
    void setActivitiesManager(ActivitiesManager *newActivitiesManager);
#endif

    [[nodiscard]] bool filterActivities() const;
    void setFilterActivities(bool newFilterActivities);

    void slotActivitiesChanged();

protected:
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QStringList mAccountOrder;
    bool mFilterActivities = true;
#if HAVE_ACTIVITY_SUPPORT
    ActivitiesManager *mActivitiesManager = nullptr;
#endif
};
