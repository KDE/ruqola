/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class KTextEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsAskApplicationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsAskApplicationWidget(QWidget *parent = nullptr);
    ~ApplicationsSettingsAskApplicationWidget() override;

    [[nodiscard]] QString message() const;

private:
    KTextEdit *const mMessage;
};
