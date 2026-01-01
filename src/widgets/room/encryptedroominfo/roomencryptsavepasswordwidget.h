/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomEncryptSavePasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoomEncryptSavePasswordWidget(QWidget *parent = nullptr);
    ~RoomEncryptSavePasswordWidget() override;
};
