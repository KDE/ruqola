/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencedirectcalldialog.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ConferenceDirectCallDialog::ConferenceDirectCallDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mConferenceCallWidget(new ConferenceCallWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Start a Direct Call"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mConferenceCallWidget->setObjectName(QStringLiteral("mConferenceCallWidget"));
    mainLayout->addWidget(mConferenceCallWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    button->button(QDialogButtonBox::Ok)->setText(i18n("Start"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ConferenceDirectCallDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ConferenceDirectCallDialog::accept);
}

ConferenceDirectCallDialog::~ConferenceDirectCallDialog() = default;
