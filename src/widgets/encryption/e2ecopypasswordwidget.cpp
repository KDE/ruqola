/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypasswordwidget.h"
#include "encryption/e2ekeymanager.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
E2eCopyPasswordWidget::E2eCopyPasswordWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    auto label = new QLabel(this);
    label->setObjectName("label"_L1);
    label->setTextFormat(Qt::RichText);
    QString randomPassword;
    if (account) {
        randomPassword = account->e2eKeyManager()->generateRandomPassword();
    }
    label->setText(
        i18n("Create secure private rooms and direct messages with end-to-end encryption.<br/><br/>Save your password securely, as the key to encode/decode "
             "your messages won't be saved on the server. You'll need to enter it on other devices to use e2e encryption.<br/><br/>Change your password "
             "anytime from "
             "any browser you've entered it on. Remember to store your password before dismissing this message.<br/><br/>Your password is:"));
    label->setWordWrap(true);
    mainLayout->addWidget(label);

    auto passwordLayout = new QHBoxLayout;
    passwordLayout->setObjectName("passwordLayout"_L1);
    passwordLayout->setContentsMargins({});

    mainLayout->addLayout(passwordLayout);

    auto passwordLabel = new QLabel(this);
    passwordLabel->setObjectName("passwordLabel"_L1);
    passwordLabel->setTextFormat(Qt::RichText);
    passwordLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QFont labFont = passwordLabel->font();
    labFont.setBold(true);
    passwordLabel->setFont(labFont);
    passwordLabel->setText(randomPassword);

    passwordLayout->addWidget(passwordLabel);

    auto copyToolButton = new QToolButton(this);
    copyToolButton->setObjectName("copyToolButton"_L1);
    passwordLayout->addWidget(copyToolButton);
    copyToolButton->setAutoRaise(true);
    connect(copyToolButton, &QToolButton::clicked, this, [this, randomPassword]() {
        // TODO copy
    });

    mainLayout->addStretch(1);
}

E2eCopyPasswordWidget::~E2eCopyPasswordWidget() = default;

#include "moc_e2ecopypasswordwidget.cpp"
