/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QMap>
#include <QWidget>
class FlowLayout;
class PendingAttachmentClickableWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PendingAttachmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PendingAttachmentWidget(QWidget *parent = nullptr);
    ~PendingAttachmentWidget() override;

    void setAttachments(const QStringList &fileNames);
    [[nodiscard]] bool hasAttachments() const;

    void clear();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveAttachment(const QString &fileName);
    FlowLayout *const mFlowLayout;
    QMap<QString, PendingAttachmentClickableWidget *> mMap;
};
