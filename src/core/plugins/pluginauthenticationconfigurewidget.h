/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QWidget>

class LIBRUQOLACORE_EXPORT PluginAuthenticationConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PluginAuthenticationConfigureWidget(QWidget *parent = nullptr);
    ~PluginAuthenticationConfigureWidget() override;
};
