/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edecodeencryptionkeywidget.h"
#include <KLocalizedString>

E2eDecodeEncryptionKeyWidget::E2eDecodeEncryptionKeyWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setPosition(KMessageWidget::Header);
    setText(i18n("Save your encryption password. Click here to view and copy your password."));

    auto decodeEncryptionKeyAction = new QAction(i18n("Decrypte Encryption Key"), this);
    decodeEncryptionKeyAction->setObjectName(QStringLiteral("decodeEncryptionKeyAction"));
    connect(decodeEncryptionKeyAction, &QAction::triggered, this, &E2eDecodeEncryptionKeyWidget::decodeEncrytionKey);
    addAction(decodeEncryptionKeyAction);
}

E2eDecodeEncryptionKeyWidget::~E2eDecodeEncryptionKeyWidget() = default;

#include "moc_e2edecodeencryptionkeywidget.cpp"
