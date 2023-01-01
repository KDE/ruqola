/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticoncategoriesmodel.h"
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

// Model showing emojis from a single category
class LIBRUQOLACORE_EXPORT EmoticonFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EmoticonFilterModel(QObject *parent = nullptr);
    ~EmoticonFilterModel() override;

    void setCurrentCategory(const QString &category);
    Q_REQUIRED_RESULT QString currentCategory() const;

    Q_REQUIRED_RESULT EmoticonCategoriesModel *emoticonCategoriesModel() const;

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Q_DISABLE_COPY(EmoticonFilterModel)
    QString mCurrentCategory;
    EmoticonCategoriesModel *const mEmoticonCategoriesModel;
};
