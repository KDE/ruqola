/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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

#include "libruqolacore_export.h"
#include "listmessagesmodel.h"
#include <QSortFilterProxyModel>
class LIBRUQOLACORE_EXPORT ListMessagesModelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ListMessagesModelFilterProxyModel(ListMessagesModel *model = nullptr, QObject *parent = nullptr);
    ~ListMessagesModelFilterProxyModel() override;
    void setFilterString(const QString &string);

    ListMessagesModel *listMessageModel() const;
    Q_REQUIRED_RESULT int total() const;
    Q_REQUIRED_RESULT bool hasFullList() const;
    Q_REQUIRED_RESULT bool loadMoreListMessagesInProgress() const;

    Q_REQUIRED_RESULT int numberOfMessages() const;

Q_SIGNALS:
    void hasFullListChanged();
    void listMessageTypeChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    Q_REQUIRED_RESULT QString title() const;
    Q_REQUIRED_RESULT ListMessagesModel::ListMessageType listMessageType() const;
    ListMessagesModel *const mModel;
};

