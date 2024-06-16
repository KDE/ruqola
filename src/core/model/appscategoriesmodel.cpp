/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscategoriesmodel.h"
#include <KLocalizedString>

AppsCategoriesModel::AppsCategoriesModel(QObject *parent)
    : QStandardItemModel(parent)
{
}

AppsCategoriesModel::~AppsCategoriesModel() = default;

void AppsCategoriesModel::createItem(const QString &displayStr, const QString &identifier)
{
    auto item = new QStandardItem(displayStr);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(identifier, Identifier);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
    item->setToolTip(displayStr);
    appendRow(item);
}

QList<AppsCategoryInfo> AppsCategoriesModel::appsCategories() const
{
    return mAppsCategories;
}

void AppsCategoriesModel::setAppsCategories(const QList<AppsCategoryInfo> &appsCategories)
{
    auto item = new QStandardItem(i18n("Categories"));
    item->setSelectable(false);
    appendRow(item);
    for (const AppsCategoryInfo &info : appsCategories) {
        createItem(info.title(), info.identifier());
    }
}

bool AppsCategoriesModel::wasFilled() const
{
    return !mAppsCategories.isEmpty();
}

QStringList AppsCategoriesModel::categoriesSelected() const
{
    // TODO
    return {};
}

#include "moc_appscategoriesmodel.cpp"
