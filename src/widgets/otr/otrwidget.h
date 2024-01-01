/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT OtrWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit OtrWidget(QWidget *parent = nullptr);
    ~OtrWidget() override;

Q_SIGNALS:
    void refreshKeys();
    void closeOtr();
};
