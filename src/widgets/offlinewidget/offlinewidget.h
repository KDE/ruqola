/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT OffLineWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit OffLineWidget(QWidget *parent = nullptr);
    ~OffLineWidget() override;
};
