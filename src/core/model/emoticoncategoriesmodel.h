/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef EMOTICONCATEGORIESMODEL_H
#define EMOTICONCATEGORIESMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "emoticons/unicodeemoticon.h"
#include "libruqola_private_export.h"
#include "emoticoncategory.h"

class LIBRUQOLACORE_TESTS_EXPORT EmoticonCategoriesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EmoticonCategoryRoles {
        Name = Qt::UserRole + 1,
        Category
    };
    Q_ENUM(EmoticonCategoryRoles)

    explicit EmoticonCategoriesModel(QObject *parent = nullptr);
    ~EmoticonCategoriesModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;
    void setCategories(const QVector<EmoticonCategory> &categories);

private:
    Q_DISABLE_COPY(EmoticonCategoriesModel)
    QVector<EmoticonCategory> mCategories;
};
#endif // EMOTICONCATEGORIESMODEL_H
