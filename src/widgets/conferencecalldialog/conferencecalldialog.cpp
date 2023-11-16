/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecalldialog.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
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

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    button->button(QDialogButtonBox::Ok)->setText(i18n("Start"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ConferenceCallDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ConferenceCallDialog::accept);
}

ConferenceCallDialog::~ConferenceCallDialog() = default;

ConferenceCallWidget::ConferenceCallStart ConferenceCallDialog::conferenceCallInfo() const
{
    return mConferenceCallWidget->conferenceCallInfo();
}

void ConferenceCallDialog::setConferenceCallInfo(const ConferenceCallWidget::ConferenceCallStart &infoCallStart)
{
    mConferenceCallWidget->setConferenceCallInfo(infoCallStart);
}

#include "moc_conferencecalldialog.cpp"
