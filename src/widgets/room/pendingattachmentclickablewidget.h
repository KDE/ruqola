/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "accountroomsettings.h"
#include "libruqolawidgets_private_export.h"
#include <QUrl>
#include <QWidget>
class LIBRUQOLAWIDGETS_TESTS_EXPORT PendingAttachmentClickableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PendingAttachmentClickableWidget(const QUrl &url, QWidget *parent = nullptr);
    ~PendingAttachmentClickableWidget() override;

    [[nodiscard]] AccountRoomSettings::PendingAttachmentInfo pendingAttachmentInfo() const;

Q_SIGNALS:
    void removeAttachment(const QUrl &url);

private:
    AccountRoomSettings::PendingAttachmentInfo mPendingAttachmentInfo;
};
