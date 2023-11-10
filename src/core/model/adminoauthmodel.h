/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

    const QVector<OauthInfo> &adminOauth() const;
    void setAdminOauth(const QVector<OauthInfo> &newAdminInvites);

    void removeOauth(const QString &identifier);

    void addMoreOauth(const OauthInfo &info);

private:
    Q_DISABLE_COPY(AdminOauthModel)
    LIBRUQOLACORE_NO_EXPORT void clear();
    QVector<OauthInfo> mAdminOauth;
};
