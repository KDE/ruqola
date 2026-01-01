/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelcounterinfo.h"
#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomCounterInfoWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit RoomCounterInfoWidget(QWidget *parent = nullptr);
    ~RoomCounterInfoWidget() override;

    [[nodiscard]] const ChannelCounterInfo *channelCounterInfo() const;
    void setChannelCounterInfo(const ChannelCounterInfo *channelCounterInfo);

Q_SIGNALS:
    void markAsRead();
    void jumpToUnreadMessage(qint64 numberOfMessages);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateInfo();
    const ChannelCounterInfo *mChannelCounterInfo = nullptr;
};
