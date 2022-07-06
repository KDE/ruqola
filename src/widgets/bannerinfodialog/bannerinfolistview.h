/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QListView>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerInfoListView : public QListView
{
    Q_OBJECT
public:
    explicit BannerInfoListView(QWidget *parent = nullptr);
    ~BannerInfoListView() override;
};
