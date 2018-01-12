/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "qmlaboutdataauthormodel.h"
#include "ruqola_debug.h"

QmlAboutDataAuthorModel::QmlAboutDataAuthorModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int QmlAboutDataAuthorModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAboutPerson.size();
}

QVariant QmlAboutDataAuthorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return QVariant();
    }
    const int indexRow = index.row();
    if (indexRow >= rowCount()) {
        qCWarning(RUQOLA_LOG) << "ERROR: index out of bounds";
        return QVariant();
    }
    if (role == UsernameRole) {
        return mAboutPerson.at(indexRow).name();
    } else if (role == TaskRole) {
        return mAboutPerson.at(indexRow).task();
    } else if (role == EmailRole) {
        return QStringLiteral("<a href=\"mailto:%1\">%1</a>").arg(mAboutPerson.at(indexRow).emailAddress());
    } else if (role == WebSiteRoles) {
        return QStringLiteral("<a href=\"%1\">%1</a>").arg(mAboutPerson.at(indexRow).webAddress());
    }
    return {};
}

QHash<int, QByteArray> QmlAboutDataAuthorModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UsernameRole] = QByteArrayLiteral("username");
    roles[TaskRole] = QByteArrayLiteral("task");
    roles[EmailRole] = QByteArrayLiteral("email");
    roles[WebSiteRoles] = QByteArrayLiteral("website");
    return roles;
}

void QmlAboutDataAuthorModel::setAboutPerson(const QList<KAboutPerson> &aboutPerson)
{
    if (!aboutPerson.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, aboutPerson.size() - 1);
        mAboutPerson = aboutPerson;
        endInsertRows();
    }
}
