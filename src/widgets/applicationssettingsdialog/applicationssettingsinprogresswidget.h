/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
class QProgressBar;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsInProgressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsInProgressWidget(QWidget *parent = nullptr);
    ~ApplicationsSettingsInProgressWidget() override;

private:
    QLabel *const mLabel;
    QProgressBar *const mProgressBar;
};
