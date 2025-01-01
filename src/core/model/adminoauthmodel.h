/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

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
        CreatedBy,
        CreatedAt,
        CreatedAtStr,
        Active,
        LastColumn = Active,
    };
    Q_ENUM(AdminOauthRoles)

    explicit AdminOauthModel(QObject *parent = nullptr);
    ~AdminOauthModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] const QList<OauthInfo> &adminOauth() const;
    void setAdminOauth(const QList<OauthInfo> &newAdminInvites);

    void removeOauth(const QString &identifier);

    void addMoreOauth(const OauthInfo &info);

private:
    LIBRUQOLACORE_NO_EXPORT void clear();
    QList<OauthInfo> mAdminOauth;
};
