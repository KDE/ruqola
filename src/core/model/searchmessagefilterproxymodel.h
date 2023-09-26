/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class CommonMessageModel;
class LIBRUQOLACORE_EXPORT SearchMessageFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchMessageFilterProxyModel(CommonMessageModel *model = nullptr, QObject *parent = nullptr);
    ~SearchMessageFilterProxyModel() override;
    [[nodiscard]] bool loadSearchMessageInProgress() const;

    void clearModel();
    void setSearchText(const QString &str);
Q_SIGNALS:
    void stringNotFoundChanged();
    void loadingInProgressChanged();

private:
    CommonMessageModel *const mSearchMessageModel;
};
