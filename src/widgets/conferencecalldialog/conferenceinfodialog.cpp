/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferenceinfodialog.h"
#include "conferenceinfowidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ConferenceInfoDialog::ConferenceInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mConferenceInfoWidget(new ConferenceInfoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Start a Direct Call"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mConferenceInfoWidget->setObjectName(QStringLiteral("mConferenceInfoWidget"));
    mainLayout->addWidget(mConferenceInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ConferenceInfoDialog::accept);
    connect(button, &QDialogButtonBox::accepted, this, &ConferenceInfoDialog::reject);
}

ConferenceInfoDialog::~ConferenceInfoDialog() = default;
