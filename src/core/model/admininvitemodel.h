/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "invite/inviteinfo.h"
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
        CreateAt,
        CreateAtStr,
        Expire,
        Uses,
        MaxUses,
        LastColumn = MaxUses,
    };
    Q_ENUM(AdminInviteRoles)

    explicit AdminInviteModel(QObject *parent = nullptr);
    ~AdminInviteModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] const QVector<InviteInfo> &adminInvites() const;
    void setAdminInvites(const QVector<InviteInfo> &newAdminInvites);

    void removeInvite(const QString &identifier);

private:
    Q_DISABLE_COPY(AdminInviteModel)
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString expireInvitation(const InviteInfo &inviteInfo) const;
    LIBRUQOLACORE_NO_EXPORT void clear();
    QVector<InviteInfo> mAdminInvites;
};
