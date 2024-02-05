/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edecodeencryptionkeyfailedwidget.h"
#include <KLocalizedString>

E2eDecodeEncryptionKeyFailedWidget::E2eDecodeEncryptionKeyFailedWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Error);
    setPosition(KMessageWidget::Header);
    // TODO setText(i18n("Save your encryption password. Click here to view and copy your password."));
}

E2eDecodeEncryptionKeyFailedWidget::~E2eDecodeEncryptionKeyFailedWidget() = default;
