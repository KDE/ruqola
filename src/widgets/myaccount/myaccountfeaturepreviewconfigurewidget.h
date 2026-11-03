/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountFeaturePreviewConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountFeaturePreviewConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountFeaturePreviewConfigureWidget() override;

    void load();
    void save();

    void initialize();

private:
    RocketChatAccount *const mRocketChatAccount;
    QCheckBox *const mTimeStampCheckBox;
};
