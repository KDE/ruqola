/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT FeaturePreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FeaturePreviewWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~FeaturePreviewWidget() override;
    void initialize();
};
