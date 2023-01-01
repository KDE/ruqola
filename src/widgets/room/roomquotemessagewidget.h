/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KMessageWidget>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomQuoteMessageWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit RoomQuoteMessageWidget(QWidget *parent = nullptr);
    ~RoomQuoteMessageWidget() override;

    void setMessageText(const QString &str);

Q_SIGNALS:
    void cancelQuoteMessage();
};
