/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include "moderation/moderationreportuserinfos.h"
class RocketChatAccount;
class QTextBrowser;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationUsersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModerationUsersWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationUsersWidget() override;

    void setModerationReportUserInfos(const ModerationReportUserInfos &infos);

private:
    QTextBrowser *const mTextBrowser;
    RocketChatAccount *const mCurrentRocketChatAccount;
};
