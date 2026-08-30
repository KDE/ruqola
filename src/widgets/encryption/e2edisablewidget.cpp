/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisablewidget.h"
#include "encryption/e2ekeymanager.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KSeparator>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
E2eDisableWidget::E2eDisableWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mResetKeysButton(new QPushButton(i18nc("@action:button", "Reset encryption keys"), this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(
        i18n("Disabling E2EE will compromise the privacy of this private group. Access to any encrypted content will be lost for all private group members.\n"
             "Encryption can be re-enabled later. Proceed with caution.\n"
             "If no one is able to access the encrypted content you can reset encryption keys instead."),
        this);
    label->setObjectName(u"label"_s);
    label->setWordWrap(true);
    mainLayout->addWidget(label);

    auto separator = new KSeparator(this);
    separator->setObjectName(u"separator"_s);
    mainLayout->addWidget(separator);

    auto labelReset =
        new QLabel(i18n("Alternatively, resetting encryption keys will keep encryption enabled but access to previously encrypted content may be lost."), this);
    labelReset->setObjectName(u"labelReset"_s);
    labelReset->setWordWrap(true);
    mainLayout->addWidget(labelReset);

    mResetKeysButton->setObjectName(u"pushButton"_s);
    mResetKeysButton->setEnabled(false);
    mainLayout->addWidget(mResetKeysButton);
    connect(mResetKeysButton, &QPushButton::clicked, this, &E2eDisableWidget::slotResetEncryptionKeys);
    mainLayout->addStretch(1);
}

E2eDisableWidget::~E2eDisableWidget() = default;

void E2eDisableWidget::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
    mResetKeysButton->setEnabled(mRocketChatAccount && !mRoomId.isEmpty());
}

void E2eDisableWidget::slotResetEncryptionKeys()
{
    if (!mRocketChatAccount || mRoomId.isEmpty()) {
        return;
    }
    if (KMessageBox::warningContinueCancel(
            this,
            i18n(
                "Resetting the encryption keys is only recommended if no room member has a valid key to regain access to the previously encrypted content. All "
                "members may lose access to previously encrypted content.\nThis action cannot be undone. Do you want to continue?"),
            i18nc("@title:window", "Reset Encryption Keys"),
            KStandardGuiItem::cont(),
            KStandardGuiItem::cancel(),
            u"resetE2eRoomKeys"_s)
        != KMessageBox::Continue) {
        return;
    }
    if (!mRocketChatAccount->e2eKeyManager()->resetRoomKey(mRoomId)) {
        // The key manager logs why it refused (no usable own key, unknown room…).
        KMessageBox::error(this, i18n("The encryption keys of this room could not be reset."), i18nc("@title:window", "Reset Encryption Keys"));
        return;
    }
    Q_EMIT resetEncryptionKeysDone();
}

#include "moc_e2edisablewidget.cpp"
