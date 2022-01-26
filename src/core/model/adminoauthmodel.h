/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "oauth/oauthinfo.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT AdminOauthModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AdminOauthRoles {
        Identifier,
        Name,
        ClientId,
        ClientSecret,
        RedirectUri,
        Active,
        LastColumn = Active,
    };
    Q_ENUM(AdminOauthRoles)

    explicit AdminOauthModel(QObject *parent = nullptr);
    ~AdminOauthModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    const QVector<OauthInfo> &adminOauth() const;
    void setAdminOauth(const QVector<OauthInfo> &newAdminInvites);

    void removeOauth(const QString &identifier);

private:
    Q_DISABLE_COPY(AdminOauthModel)
    // Q_REQUIRED_RESULT QString expireInvitation(const InviteInfo &inviteInfo) const;
    QVector<OauthInfo> mAdminOauth;
};
