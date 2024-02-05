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
    setCloseButtonVisible(true);
    setMessageType(Error);
    setPosition(KMessageWidget::Header);
    setText(i18n("It was not possible to decode your encryption key to be imported. Your encryption password seems wrong."));

    auto decodeEncryptionKeyAction = new QAction(i18n("Decrypte Encryption Key"), this);
    decodeEncryptionKeyAction->setObjectName(QStringLiteral("decodeEncryptionKeyAction"));
    connect(decodeEncryptionKeyAction, &QAction::triggered, this, &E2eDecodeEncryptionKeyFailedWidget::decodeEncrytionKey);
    addAction(decodeEncryptionKeyAction);
}

E2eDecodeEncryptionKeyFailedWidget::~E2eDecodeEncryptionKeyFailedWidget() = default;

#include "moc_e2edecodeencryptionkeyfailedwidget.cpp"
