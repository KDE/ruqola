/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class SearchMessageModel;
class LIBRUQOLACORE_EXPORT SearchMessageFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchMessageFilterProxyModel(SearchMessageModel *model = nullptr, QObject *parent = nullptr);
    ~SearchMessageFilterProxyModel() override;
    Q_REQUIRED_RESULT bool stringNotFound() const;
    Q_REQUIRED_RESULT bool loadSearchMessageInProgress() const;

Q_SIGNALS:
    void stringNotFoundChanged();
    void loadingInProgressChanged();

private:
    SearchMessageModel *const mSearchMessageModel;
};

