/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "moderation/moderationreportinfo.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationMessageInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModerationMessageInfoWidget(QWidget *parent = nullptr);
    ~ModerationMessageInfoWidget() override;

    void showReportInfo(const ModerationReportInfo &info);
};
