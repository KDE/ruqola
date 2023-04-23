/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
#include <config-ruqola.h>
#if HAVE_TEXT_AUTOCORRECTION
namespace TextAutoCorrection
{
class AutoCorrectionWidget;
}
#endif
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
namespace TextAutoCorrectionWidgets
{
class AutoCorrectionWidget;
}
#endif

class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureAutoCorrectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureAutoCorrectionWidget(QWidget *parent = nullptr);
    ~ConfigureAutoCorrectionWidget() override;

    void save();
    void load();

private:
#if HAVE_TEXT_AUTOCORRECTION
    TextAutoCorrection::AutoCorrectionWidget *const mAutoCorrectionWidget;
#endif
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    TextAutoCorrectionWidgets::AutoCorrectionWidget *const mAutoCorrectionWidget;
#endif
};
