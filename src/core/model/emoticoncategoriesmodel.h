/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticons/emoticoncategory.h"
#include "libruqola_private_export.h"
#include <QAbstractListModel>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT EmoticonCategoriesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EmoticonCategoryRoles {
        Name = Qt::UserRole + 1,
        Category,
    };
    Q_ENUM(EmoticonCategoryRoles)

    explicit EmoticonCategoriesModel(QObject *parent = nullptr);
    ~EmoticonCategoriesModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void setCategories(const QVector<EmoticonCategory> &categories);

private:
    Q_DISABLE_COPY(EmoticonCategoriesModel)
    QVector<EmoticonCategory> mCategories;
};
