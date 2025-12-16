/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT BannerInfosFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit BannerInfosFilterProxyModel(QObject *parent = nullptr);
    ~BannerInfosFilterProxyModel() override;

    [[nodiscard]] bool showUnread() const;
    void setShowUnread(bool newShowUnread);

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    bool mShowUnread = false;
};
