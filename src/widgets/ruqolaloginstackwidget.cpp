/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginstackwidget.h"

RuqolaLoginStackWidget::RuqolaLoginStackWidget(QWidget *parent)
    : QStackedWidget(parent)
{
}

RuqolaLoginStackWidget::~RuqolaLoginStackWidget() = default;

#include "moc_ruqolaloginstackwidget.cpp"
