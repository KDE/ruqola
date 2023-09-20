/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ModerationConsoleTreeWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorModerationRangeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorModerationRangeWidget(QWidget *parent = nullptr);
    ~AdministratorModerationRangeWidget() override;
};
