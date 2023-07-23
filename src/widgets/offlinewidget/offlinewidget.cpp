/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "offlinewidget.h"
#include <KLocalizedString>

OffLineWidget::OffLineWidget(QWidget *parent)
    : KMessageWidget{parent}
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    // TODO correct i18n ?
    setText(i18n("We use offline mode."));
}

OffLineWidget::~OffLineWidget() = default;
