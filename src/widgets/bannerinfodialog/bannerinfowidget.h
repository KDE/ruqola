/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class BannerInfoListView;
class BannerInfoListSearchLineWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BannerInfoWidget(QWidget *parent = nullptr);
    ~BannerInfoWidget() override;

private:
    BannerInfoListSearchLineWidget *const mBannerInfoListSearchLineWidget;
    BannerInfoListView *const mBannerInfoListView;
};
