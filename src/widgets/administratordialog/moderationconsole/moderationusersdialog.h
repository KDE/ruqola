/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "moderation/moderationreportuserinfos.h"
#include <QDialog>

class ModerationUsersWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationUsersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModerationUsersDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationUsersDialog() override;

    void setModerationReportUserInfos(const ModerationReportUserInfos &infos);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();

    ModerationUsersWidget *const mModerationUsersWidget;
};
