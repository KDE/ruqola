/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class BannerInfoListView;
class BannerInfoListSearchLineWidget;
class BannerInfosFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BannerInfoWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~BannerInfoWidget() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeProxyModel();
    BannerInfoListSearchLineWidget *const mBannerInfoListSearchLineWidget;
    BannerInfoListView *const mBannerInfoListView;
    BannerInfosFilterProxyModel *const mBannerInfosFilterProxyModel;
    RocketChatAccount *const mRocketChatAccount;
};
