/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewMessageWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit WhatsNewMessageWidget(QWidget *parent = nullptr);
    ~WhatsNewMessageWidget() override;

private:
    void slotLinkActivated(const QString &contents);
};
