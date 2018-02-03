/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "usercompletermodelfiltermodelproxy.h"
#include "usercompletermodel.h"

UserCompleterModelFilterModelProxy::UserCompleterModelFilterModelProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(UserCompleterModel::UserName);
    sort(0);
}

UserCompleterModelFilterModelProxy::~UserCompleterModelFilterModelProxy()
{
}

QHash<int, QByteArray> UserCompleterModelFilterModelProxy::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel()) {
        return source->roleNames();
    }
    return QHash<int, QByteArray>();
}

bool UserCompleterModelFilterModelProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, UserCompleterModel::UserName).toString();
        const QString rightString = sourceModel()->data(right, UserCompleterModel::UserName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

int UserCompleterModelFilterModelProxy::numberOfUsers() const
{
    qDebug() << "int UserCompleterModelFilterModelProxy::numberOfUsers() const" ;
    if (QAbstractItemModel *source = sourceModel()) {
        qDebug() << " dssssssssssssssssssssssssssssssss";
        return source->rowCount(QModelIndex());
    }
    return 0;
}
