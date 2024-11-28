/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "apps/appscountinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QProgressBar;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AppsCountInfoWidget : public QWidget
{
    Q_OBJECT
public:
    enum class InfoType : uint8_t {
        Unknown,
        Applications,
        PrivateApps,
    };

    explicit AppsCountInfoWidget(QWidget *parent = nullptr);
    ~AppsCountInfoWidget() override;
    void setAppCountInfo(const AppsCountInfo &info);

private:
    QProgressBar *const mProgressBar;
};
