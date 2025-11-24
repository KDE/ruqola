/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "offlinewidget.h"
#include <KLocalizedString>

OffLineWidget::OffLineWidget(QWidget *parent)
    : KMessageWidget{parent}
{
    setPosition(KMessageWidget::Header);
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Warning);
    setText(i18n("Network is offline mode."));
    setTextFormat(Qt::PlainText);
}

OffLineWidget::~OffLineWidget() = default;

#include "moc_offlinewidget.cpp"
