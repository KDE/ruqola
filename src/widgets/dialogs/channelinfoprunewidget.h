/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "rooms/saveroomsettingsjob.h"

#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QSpinBox;
class RetentionInfo;
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelInfoPruneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelInfoPruneWidget(QWidget *parent = nullptr);
    ~ChannelInfoPruneWidget() override;

    void setRetentionInfo(RetentionInfo retentionInfo);

    void saveRoomSettingsInfo(RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo &info, Room *mRoom);

private:
    void setOverrideGlobalSettings(bool override);
    QCheckBox *const mExcludePinnedMessages;
    QCheckBox *const mPruneFileOnlyKeepMessages;
    QCheckBox *const mAutomaticPruneOldMessages;
    QCheckBox *const mOverrideGlobalRetentionPolicy;
    QSpinBox *const mMaximumAgeInDay;
};
