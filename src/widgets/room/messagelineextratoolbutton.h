/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QToolButton>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageLineExtraToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit MessageLineExtraToolButton(QWidget *parent = nullptr);
    ~MessageLineExtraToolButton() override;
};
