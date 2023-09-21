/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] ChannelCounterInfo channelCounterInfo() const;
    void setChannelCounterInfo(const ChannelCounterInfo &channelCounterInfo);

Q_SIGNALS:
    void markAsRead();
    void jumpToUnreadMessage(qint64 numberOfMessages);

private:
    void updateInfo();
    void slotLinkActivated(const QString &contents);
    ChannelCounterInfo mChannelCounterInfo;
};
