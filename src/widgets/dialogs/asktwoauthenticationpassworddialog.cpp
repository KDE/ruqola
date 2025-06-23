/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asktwoauthenticationpassworddialog.h"
using namespace Qt::Literals::StringLiterals;

#include "asktwoauthenticationpasswordwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

AskTwoAuthenticationPasswordDialog::AskTwoAuthenticationPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , mAskTwoAuthicationPasswordWidget(new AskTwoAuthenticationPasswordWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Channel Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mAskTwoAuthicationPasswordWidget->setObjectName(u"mAskTwoAuthicationPasswordWidget"_s);
    mainLayout->addWidget(mAskTwoAuthicationPasswordWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AskTwoAuthenticationPasswordDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AskTwoAuthenticationPasswordDialog::accept);
    mainLayout->addWidget(buttonBox);
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mAskTwoAuthicationPasswordWidget, &AskTwoAuthenticationPasswordWidget::updateButtonOk, mOkButton, &QPushButton::setEnabled);
}

AskTwoAuthenticationPasswordDialog::~AskTwoAuthenticationPasswordDialog() = default;

QString AskTwoAuthenticationPasswordDialog::code() const
{
    return mAskTwoAuthicationPasswordWidget->code();
}

RocketChatAccount *AskTwoAuthenticationPasswordDialog::rocketChatAccount() const
{
    return mAskTwoAuthicationPasswordWidget->rocketChatAccount();
}

void AskTwoAuthenticationPasswordDialog::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAskTwoAuthicationPasswordWidget->setRocketChatAccount(newRocketChatAccount);
}

#include "moc_asktwoauthenticationpassworddialog.cpp"
