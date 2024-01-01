/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomReplyThreadWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit RoomReplyThreadWidget(QWidget *parent = nullptr);
    ~RoomReplyThreadWidget() override;
    void setMessageText(const QString &str);
Q_SIGNALS:
    void cancelReplyingInThread();
};
