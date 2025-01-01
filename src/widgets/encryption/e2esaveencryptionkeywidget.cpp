/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2esaveencryptionkeywidget.h"
#include <KLocalizedString>

E2eSaveEncryptionKeyWidget::E2eSaveEncryptionKeyWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setPosition(KMessageWidget::Header);
    setTextFormat(Qt::PlainText);
    setText(i18n("Save your encryption password. Click here to view and copy your password."));

    auto saveEncryptionKeyAction = new QAction(i18nc("@action", "Save Encryption Key"), this);
    saveEncryptionKeyAction->setObjectName(QStringLiteral("decodeEncryptionKeyAction"));
    connect(saveEncryptionKeyAction, &QAction::triggered, this, &E2eSaveEncryptionKeyWidget::saveEncrytionKey);
    addAction(saveEncryptionKeyAction);
}

E2eSaveEncryptionKeyWidget::~E2eSaveEncryptionKeyWidget() = default;

#include "moc_e2esaveencryptionkeywidget.cpp"
