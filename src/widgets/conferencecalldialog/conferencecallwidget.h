/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceCallWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConferenceCallWidget(QWidget *parent = nullptr);
    ~ConferenceCallWidget() override;
};
