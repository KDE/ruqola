/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
namespace Sonnet
{
class ConfigWidget;
}
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureSpellCheckingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureSpellCheckingWidget(QWidget *parent = nullptr);
    ~ConfigureSpellCheckingWidget() override;
    void save();
    void load();

private:
    Sonnet::ConfigWidget *const mConfigWidget;
};
