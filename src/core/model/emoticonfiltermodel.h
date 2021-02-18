/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "emoticoncategoriesmodel.h"
#include "emoticons/unicodeemoticon.h"
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

    EmoticonCategoriesModel *emoticonCategoriesModel() const;

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Q_DISABLE_COPY(EmoticonFilterModel)
    QString mCurrentCategory;
    EmoticonCategoriesModel *const mEmoticonCategoriesModel;
};

