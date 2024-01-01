/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "listmessagesmodel.h"
#include <QSortFilterProxyModel>
class LIBRUQOLACORE_EXPORT ListMessagesFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ListMessagesFilterProxyModel(ListMessagesModel *model = nullptr, QObject *parent = nullptr);
    ~ListMessagesFilterProxyModel() override;
    void setFilterString(const QString &string);

    ListMessagesModel *listMessageModel() const;
    [[nodiscard]] int total() const;
    [[nodiscard]] bool hasFullList() const;
    [[nodiscard]] bool loadMoreListMessagesInProgress() const;

    [[nodiscard]] int numberOfMessages() const;

    void clear();

Q_SIGNALS:
    void hasFullListChanged();
    void listMessageTypeChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString title() const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT ListMessagesModel::ListMessageType listMessageType() const;
    ListMessagesModel *const mModel;
};
