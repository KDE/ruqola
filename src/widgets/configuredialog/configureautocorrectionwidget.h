/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
namespace PimCommonAutoCorrection
{
class AutoCorrectionWidget;
}
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureAutoCorrectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureAutoCorrectionWidget(QWidget *parent = nullptr);
    ~ConfigureAutoCorrectionWidget() override;

    void save();
    void load();

private:
    PimCommonAutoCorrection::AutoCorrectionWidget *const mAutoCorrectionWidget;
};
