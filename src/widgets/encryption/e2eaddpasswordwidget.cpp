/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2eaddpasswordwidget.h"
#include <KLocalizedString>

E2eAddPasswordWidget::E2eAddPasswordWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setPosition(KMessageWidget::Header);
    setText(i18n("Save your encryption password. Click here to view and copy your password."));
}

E2eAddPasswordWidget::~E2eAddPasswordWidget() = default;
