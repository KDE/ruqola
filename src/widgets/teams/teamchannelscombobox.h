/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QComboBox>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamChannelsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit TeamChannelsComboBox(QWidget *parent = nullptr);
    ~TeamChannelsComboBox() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void init();
};
