/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
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
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    TextAutoCorrectionWidgets::AutoCorrectionWidget *const mAutoCorrectionWidget;
#endif
};
