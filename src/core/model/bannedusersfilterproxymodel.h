/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT BannedUsersFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit BannedUsersFilterProxyModel(QObject *parent = nullptr);
    ~BannedUsersFilterProxyModel() override;

    void setFilterString(const QString &string);

private:
    QString mFilterString;
};
