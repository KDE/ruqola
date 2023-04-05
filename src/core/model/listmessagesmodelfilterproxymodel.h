/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT QString title() const;
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT ListMessagesModel::ListMessageType listMessageType() const;
    ListMessagesModel *const mModel;
};
