/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"

#include "config-ruqola.h"

#if HAVE_KUSERFEEDBACK
namespace KUserFeedback
{
class FeedbackConfigWidget;
}
#endif
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureUserFeedbackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureUserFeedbackWidget(QWidget *parent = nullptr);
    ~ConfigureUserFeedbackWidget() override;

    void save();
    void load();

private:
#if HAVE_KUSERFEEDBACK
    KUserFeedback::FeedbackConfigWidget *const mUserFeedbackWidget;
#endif
};
