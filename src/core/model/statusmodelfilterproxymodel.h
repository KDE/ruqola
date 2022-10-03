/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT StatusModelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit StatusModelFilterProxyModel(QObject *parent = nullptr);
    ~StatusModelFilterProxyModel() override;

    Q_REQUIRED_RESULT bool useOnlyStandardStatus() const;
    void setUseOnlyStandardStatus(bool useOnlyStandardStatus);

    Q_REQUIRED_RESULT bool allowOfflineSupport() const;
    void setAllowOfflineSupport(bool newAllowOfflineSupport);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    bool mUseOnlyStandardStatus = false;
    bool mAllowOfflineSupport = true;
};
