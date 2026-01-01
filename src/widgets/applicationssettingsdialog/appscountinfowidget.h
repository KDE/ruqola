/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "apps/appscountinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QProgressBar;
class QLabel;
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

    [[nodiscard]] InfoType infotype() const;
    void setInfotype(InfoType newInfotype);

private:
    QProgressBar *const mProgressBar;
    QLabel *const mApplicationInfo;
    QLabel *const mEnableAppsLabel;
    InfoType mInfotype = InfoType::Unknown;
};
