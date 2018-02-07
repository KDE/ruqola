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

#ifndef STATUSMODEL_H
#define STATUSMODEL_H

#include "user.h"
#include "libruqola_private_export.h"

#include <QAbstractListModel>
#include <QIcon>

struct StatusInfo {
    QString displayText;
    QIcon icon;
    User::PresenceStatus status;
};

class LIBRUQOLACORE_TESTS_EXPORT StatusModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum StatusRoles {
        StatusI18n = Qt::UserRole + 1,
        Status,
        Icon,
    };
    Q_ENUM(StatusRoles)

    explicit StatusModel(QObject *parent = nullptr);
    ~StatusModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    User::PresenceStatus status(int index) const;

    QHash<int, QByteArray> roleNames() const override;

private:
    Q_DISABLE_COPY(StatusModel)
    void fillModel();
    QVector<StatusInfo> mStatusList;
};

#endif // STATUSMODEL_H
