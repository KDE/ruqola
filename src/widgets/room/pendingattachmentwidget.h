/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "pendingattachmentclickablewidget.h"
#include <QMap>
#include <QUrl>
#include <QWidget>
class FlowLayout;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PendingAttachmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PendingAttachmentWidget(QWidget *parent = nullptr);
    ~PendingAttachmentWidget() override;

    void setAttachments(const QList<QUrl> &urls);
    [[nodiscard]] bool hasAttachments() const;

    void clear();
    [[nodiscard]] QList<PendingAttachmentClickableWidget::PendingAttachmentInfo> attachmentsInfo() const;

Q_SIGNALS:
    void attachmentsChanged(bool state);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveAttachment(const QUrl &url);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateAttachments();
    FlowLayout *const mFlowLayout;
    QMap<QUrl, PendingAttachmentClickableWidget *> mMap;
};
