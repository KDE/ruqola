/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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
#include "libruqolacore_export.h"

#include <QAbstractListModel>
#include <QIcon>

struct StatusInfo {
    QString displayText;
    QIcon icon;
    User::PresenceStatus status;
};

class LIBRUQOLACORE_EXPORT StatusModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentStatus READ currentStatus NOTIFY currentStatusChanged)
public:
    enum StatusRoles {
        StatusI18n = Qt::UserRole + 1,
        Status,
        Icon
    };
    Q_ENUM(StatusRoles)

    explicit StatusModel(QObject *parent = nullptr);
    ~StatusModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_REQUIRED_RESULT User::PresenceStatus status(int index) const;

    Q_REQUIRED_RESULT int currentStatus() const;

    void setCurrentPresenceStatus(User::PresenceStatus status);
    Q_REQUIRED_RESULT User::PresenceStatus currentUserStatus() const;

    Q_REQUIRED_RESULT StatusInfo currentStatusInfo() const;

Q_SIGNALS:
    void currentStatusChanged();

private:
    Q_DISABLE_COPY(StatusModel)
    void fillModel();
    QVector<StatusInfo> mStatusList;
    int mCurrentStatus = 0;
};

#endif // STATUSMODEL_H
