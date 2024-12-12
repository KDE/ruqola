/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QTextBrowser;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsVersionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsVersionWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsVersionWidget() override;

private:
    QTextBrowser *const mTextBrowser;
    RocketChatAccount *const mRocketChatAccount;
};
