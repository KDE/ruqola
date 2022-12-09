/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecalldialog.h"
#include "conferencecallwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ConferenceCallDialog::ConferenceCallDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mConferenceCallWidget(new ConferenceCallWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Start a Call"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mConferenceCallWidget->setObjectName(QStringLiteral("mConferenceCallWidget"));
    mainLayout->addWidget(mConferenceCallWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ConferenceCallDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ConferenceCallDialog::accept);
}

ConferenceCallDialog::~ConferenceCallDialog() = default;
