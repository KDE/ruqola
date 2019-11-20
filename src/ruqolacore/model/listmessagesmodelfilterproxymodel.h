/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#ifndef PINNEDMESSAGEMODELFILTERPROXYMODEL_H
#define PINNEDMESSAGEMODELFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "listmessagesmodel.h"
#include "libruqola_private_export.h"
class LIBRUQOLACORE_TESTS_EXPORT ListMessagesModelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasFullList READ hasFullList NOTIFY hasFullListChanged)
    Q_PROPERTY(ListMessagesModel::ListMessageType listMessageType READ listMessageType NOTIFY listMessageTypeChanged)
    Q_PROPERTY(int total READ total NOTIFY totalChanged)
public:
    //Fix me
    explicit ListMessagesModelFilterProxyModel(ListMessagesModel *model = nullptr, QObject *parent = nullptr);
    ~ListMessagesModelFilterProxyModel() override;
    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void setFilterString(const QString &string);


Q_SIGNALS:
    void hasFullListChanged();
    void listMessageTypeChanged();
    void totalChanged();

private:
    Q_REQUIRED_RESULT bool hasFullList() const;
    Q_REQUIRED_RESULT int total() const;
    Q_REQUIRED_RESULT ListMessagesModel::ListMessageType listMessageType() const;
    ListMessagesModel *mModel = nullptr;
};

#endif // PINNEDMESSAGEMODELFILTERPROXYMODEL_H
