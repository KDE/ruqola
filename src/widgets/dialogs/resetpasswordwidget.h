/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class ResetPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResetPasswordWidget(QWidget *parent = nullptr);
    ~ResetPasswordWidget() override;
};
