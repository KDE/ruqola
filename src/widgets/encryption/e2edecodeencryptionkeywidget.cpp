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
    setTextFormat(Qt::PlainText);
    setText(i18n("Click here to enter your e2e password."));

    auto decodeEncryptionKeyAction = new QAction(i18nc("@action", "Decode Encryption Key"), this);
    decodeEncryptionKeyAction->setObjectName(QStringLiteral("decodeEncryptionKeyAction"));
    connect(decodeEncryptionKeyAction, &QAction::triggered, this, &E2eDecodeEncryptionKeyWidget::decodeEncrytionKey);
    addAction(decodeEncryptionKeyAction);
}

E2eDecodeEncryptionKeyWidget::~E2eDecodeEncryptionKeyWidget() = default;

#include "moc_e2edecodeencryptionkeywidget.cpp"
