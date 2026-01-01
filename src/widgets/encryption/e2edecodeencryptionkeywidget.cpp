/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edecodeencryptionkeywidget.h"
using namespace Qt::Literals::StringLiterals;

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
    decodeEncryptionKeyAction->setObjectName(u"decodeEncryptionKeyAction"_s);
    connect(decodeEncryptionKeyAction, &QAction::triggered, this, &E2eDecodeEncryptionKeyWidget::decodeEncrytionKey);
    addAction(decodeEncryptionKeyAction);
}

E2eDecodeEncryptionKeyWidget::~E2eDecodeEncryptionKeyWidget() = default;

#include "moc_e2edecodeencryptionkeywidget.cpp"
