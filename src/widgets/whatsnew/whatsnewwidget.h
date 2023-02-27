/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WhatsNewWidget(QWidget *parent = nullptr);
    ~WhatsNewWidget() override;
    Q_REQUIRED_RESULT static QString newFeaturesMD5();
};
