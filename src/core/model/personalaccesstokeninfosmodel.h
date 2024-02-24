/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

#include "libruqolacore_export.h"
#include "personalaccesstokens/personalaccesstokeninfos.h"
#include <QList>

class LIBRUQOLACORE_EXPORT PersonalAccessTokenInfosModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PersonalAccessTokenInfosRoles {
        Name,
        CreateAt,
        CreateAtDateTime,
        LastTokenPart,
        ByPassTwoFactor,
        LastColumn = ByPassTwoFactor,
    };
    Q_ENUM(PersonalAccessTokenInfosRoles)

    explicit PersonalAccessTokenInfosModel(QObject *parent = nullptr);
    ~PersonalAccessTokenInfosModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void clear();

    void insertPersonalAccessTokenInfos(const PersonalAccessTokenInfos &infos);

    void removeToken(const QString &tokenName);

private:
    PersonalAccessTokenInfos mPersonalAccessTokenInfos;
};
