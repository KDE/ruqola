/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

#include "bannerinfo/bannerinfos.h"
#include "libruqolacore_export.h"
#include <QVector>

class LIBRUQOLACORE_EXPORT BannerInfosModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum BannerInfosRoles {
        Title = Qt::UserRole + 1,
        Text,
        Link,
        Read,
        Identifier,
    };

    Q_ENUM(BannerInfosRoles)

    explicit BannerInfosModel(QObject *parent = nullptr);
    ~BannerInfosModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void clear();

    void insertBannerInfos(const BannerInfos &infos);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString text(const BannerInfo &info) const;
    BannerInfos mBannerInfos;
};
