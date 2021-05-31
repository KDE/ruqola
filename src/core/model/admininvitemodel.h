/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "inviteinfo.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT AdminInviteModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AdminInviteRoles {
        UserIdentifier,
        Identifier,
        RoomId,
        Create,
        Expire,
        Uses,
        MaxUses,
        LastColumn = MaxUses,
    };
    Q_ENUM(AdminInviteRoles)

    explicit AdminInviteModel(QObject *parent = nullptr);
    ~AdminInviteModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    const QVector<InviteInfo> &adminInvites() const;
    void setAdminInvites(const QVector<InviteInfo> &newAdminInvites);

    void removeInvite(const QString &identifier);

private:
    Q_DISABLE_COPY(AdminInviteModel)
    Q_REQUIRED_RESULT QString expireInvitation(const InviteInfo &inviteInfo) const;
    QVector<InviteInfo> mAdminInvites;
};
