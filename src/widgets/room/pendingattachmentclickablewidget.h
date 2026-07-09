/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "accountroomsettings.h"
#include "libruqolawidgets_private_export.h"
#include <QUrl>
#include <QWidget>
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PendingAttachmentClickableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PendingAttachmentClickableWidget(const AccountRoomSettings::PendingAttachmentInfo &info, QWidget *parent = nullptr);
    explicit PendingAttachmentClickableWidget(const QUrl &url, QWidget *parent = nullptr);
    explicit PendingAttachmentClickableWidget(QWidget *parent);
    ~PendingAttachmentClickableWidget() override;

    [[nodiscard]] AccountRoomSettings::PendingAttachmentInfo pendingAttachmentInfo() const;

Q_SIGNALS:
    void removeAttachment(const QUrl &url);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void setPendingAttachmentInfo(const AccountRoomSettings::PendingAttachmentInfo &pendingAttachment);
    LIBRUQOLAWIDGETS_NO_EXPORT void setUrl(const QUrl &url);
    AccountRoomSettings::PendingAttachmentInfo mPendingAttachmentInfo;
    QLabel *const mIconLabel;
    QLabel *const mNameLabel;
    QLabel *const mSizeLabel;
};
