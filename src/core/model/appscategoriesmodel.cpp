/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscategoriesmodel.h"
#include <KLocalizedString>

AppsCategoriesModel::AppsCategoriesModel(QObject *parent)
    : QStandardItemModel(parent)
{
}

AppsCategoriesModel::~AppsCategoriesModel() = default;

void AppsCategoriesModel::createItem(const QString &displayStr, const QByteArray &identifier)
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
    QStringList lst;
    const int rowCountNb = rowCount();
    // First one is not a message type
    for (int i = 0; i < rowCountNb; i++) {
        QStandardItem *itemModel = item(i);
        if (itemModel) {
            if (itemModel->isCheckable() && itemModel->checkState() == Qt::Checked) {
                lst.append(itemModel->data(Qt::DisplayRole).toString());
            }
        }
    }
    return lst;
}

#include "moc_appscategoriesmodel.cpp"
