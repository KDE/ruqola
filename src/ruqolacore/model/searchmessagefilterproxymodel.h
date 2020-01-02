/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef SEARCHMESSAGEFILTERPROXYMODEL_H
#define SEARCHMESSAGEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "libruqola_private_export.h"
class SearchMessageModel;
class LIBRUQOLACORE_TESTS_EXPORT SearchMessageFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool stringNotFound READ stringNotFound NOTIFY stringNotFoundChanged)
public:
    explicit SearchMessageFilterProxyModel(SearchMessageModel *model = nullptr, QObject *parent = nullptr);
    ~SearchMessageFilterProxyModel() override;
    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    Q_REQUIRED_RESULT bool stringNotFound() const;
Q_SIGNALS:
    void stringNotFoundChanged();
private:
    SearchMessageModel *mSearchMessageModel = nullptr;
};

#endif // SEARCHMESSAGEFILTERPROXYMODEL_H
