/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "apps/appscountinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT AppCountInfoWidget : public QWidget
{
    Q_OBJECT
public:
    enum class InfoType : uint8_t {
        Unknown,
        Applications,
        PrivateApps,
    };

    explicit AppCountInfoWidget(QWidget *parent = nullptr);
    ~AppCountInfoWidget() override;
    void setAppCountInfo(const AppsCountInfo &info);
};
