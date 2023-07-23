/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "offlinewidget.h"

OffLineWidget::OffLineWidget(QWidget *parent)
    : KMessageWidget{parent}
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
}

OffLineWidget::~OffLineWidget() = default;
