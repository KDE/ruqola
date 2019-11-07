/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SEARCHCHANNELFILTERPROXYMODEL_H
#define SEARCHCHANNELFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT SearchChannelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchChannelFilterProxyModel(QObject *parent = nullptr);
    ~SearchChannelFilterProxyModel() override;
    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

protected:
    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // SEARCHCHANNELFILTERPROXYMODEL_H
