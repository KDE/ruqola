/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class NeedUpdateParserHtmlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NeedUpdateParserHtmlWidget(QWidget *parent = nullptr);
    ~NeedUpdateParserHtmlWidget() override = default;
};
