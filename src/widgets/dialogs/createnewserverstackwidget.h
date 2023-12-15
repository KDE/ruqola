/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QStackedWidget>
class CreateNewServerCheckUrlWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerStackWidget(QWidget *parent = nullptr);
    ~CreateNewServerStackWidget() override;

private:
    CreateNewServerCheckUrlWidget *const mCreateNewServerCheckUrlWidget;
};
