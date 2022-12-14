/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencedirectcalldialog.h"

#include <KLocalizedString>
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
}

ConferenceDirectCallDialog::~ConferenceDirectCallDialog()
{
}
