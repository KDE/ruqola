/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QUrl>
#include <QWidget>
class LIBRUQOLAWIDGETS_TESTS_EXPORT PendingAttachmentClickableWidget : public QWidget
{
    Q_OBJECT
public:
    struct PendingAttachmentInfo {
        QUrl fileUrl;
        QString fileName;
        QString name;
        QString alternativeText;
        [[nodiscard]] bool isValid() const
        {
            return fileUrl.isValid() && fileUrl.isLocalFile();
        }
    };
    explicit PendingAttachmentClickableWidget(const QUrl &url, QWidget *parent = nullptr);
    ~PendingAttachmentClickableWidget() override;

    [[nodiscard]] PendingAttachmentInfo pendingAttachmentInfo() const;

Q_SIGNALS:
    void removeAttachment(const QUrl &url);

private:
    PendingAttachmentInfo mPendingAttachmentInfo;
};
Q_DECLARE_TYPEINFO(PendingAttachmentClickableWidget::PendingAttachmentInfo, Q_RELOCATABLE_TYPE);
