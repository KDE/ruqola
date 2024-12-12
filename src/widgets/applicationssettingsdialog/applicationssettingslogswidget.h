/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QTextBrowser;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsLogsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsLogsWidget(QWidget *parent = nullptr);
    ~ApplicationsSettingsLogsWidget() override;

private:
    QTextBrowser *const mTextBrowser;
};
