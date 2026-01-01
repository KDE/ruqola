/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "apps/appscategoryinfo.h"
#include "libruqolacore_export.h"
#include <QStandardItemModel>

class LIBRUQOLACORE_EXPORT AppsCategoriesModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum AppsCategoriesRoles {
        Identifier = Qt::UserRole + 1,
        Title,
        Hidden,
    };
    Q_ENUM(AppsCategoriesRoles)

    explicit AppsCategoriesModel(QObject *parent = nullptr);
    ~AppsCategoriesModel() override;

    [[nodiscard]] QList<AppsCategoryInfo> appsCategories() const;
    void setAppsCategories(const QList<AppsCategoryInfo> &appsCategories);

    [[nodiscard]] bool wasFilled() const;

    [[nodiscard]] QStringList categoriesSelected() const;

private:
    LIBRUQOLACORE_NO_EXPORT void createItem(const QString &displayStr, const QByteArray &identifier);
    QList<AppsCategoryInfo> mAppsCategories;
};
