/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class CommonMessagesModel;
class LIBRUQOLACORE_EXPORT CommonMessageFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommonMessageFilterProxyModel(CommonMessagesModel *model = nullptr, QObject *parent = nullptr);
    ~CommonMessageFilterProxyModel() override;
    [[nodiscard]] bool loadCommonMessagesInProgress() const;

    void clearModel();
    void setSearchText(const QString &str);
Q_SIGNALS:
    void stringNotFoundChanged();
    void loadingInProgressChanged();

private:
    CommonMessagesModel *const mCommonMessagesModel;
};
