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
namespace TextAddonsWidgets
{
class TextAddonsWidgetFlowLayout;
}
class LIBRUQOLAWIDGETS_TESTS_EXPORT PendingAttachmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PendingAttachmentWidget(QWidget *parent = nullptr);
    ~PendingAttachmentWidget() override;

    void setAttachments(const QList<QUrl> &urls);
    void addAttachment(const QUrl &url);
    void addAttachment(const AccountRoomSettings::PendingAttachmentInfo &info);
    [[nodiscard]] bool hasAttachments() const;

    void clear();
    [[nodiscard]] QList<AccountRoomSettings::PendingAttachmentInfo> attachmentsInfo() const;

    void setPendingAttachmentInfos(const QList<AccountRoomSettings::PendingAttachmentInfo> &infos);
Q_SIGNALS:
    void attachmentsChanged(bool state);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveAttachment(const QUrl &url);
    static LIBRUQOLAWIDGETS_NO_EXPORT bool verifyExistingFile(const QUrl &fileUrl);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateAttachments();
    TextAddonsWidgets::TextAddonsWidgetFlowLayout *const mFlowLayout;
    QMap<QUrl, PendingAttachmentClickableWidget *> mMap;
};
