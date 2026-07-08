/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class LIBRUQOLAWIDGETS_TESTS_EXPORT PendingAttachmentClickableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PendingAttachmentClickableWidget(const QString &filename, QWidget *parent = nullptr);
    ~PendingAttachmentClickableWidget() override;

Q_SIGNALS:
    void removeAttachment(const QString &fileName);
};
