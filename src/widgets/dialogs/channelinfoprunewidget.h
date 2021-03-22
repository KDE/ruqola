/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

    void setRetentionInfo(const RetentionInfo &retentionInfo);

    void saveRoomSettingsInfo(RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo &info, Room *mRoom);

private:
    void setOverrideGlobalSettings(bool override);
    QCheckBox *const mExcludePinnedMessages;
    QCheckBox *const mPruneFileOnlyKeepMessages;
    QCheckBox *const mAutomaticPruneOldMessages;
    QCheckBox *const mOverrideGlobalRetentionPolicy;
    QSpinBox *const mMaximumAgeInDay;
};
