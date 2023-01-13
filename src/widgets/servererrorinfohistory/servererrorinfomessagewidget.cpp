/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagewidget.h"

ServerErrorInfoMessageWidget::ServerErrorInfoMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    setMessageType(Error);
}

ServerErrorInfoMessageWidget::~ServerErrorInfoMessageWidget() = default;
