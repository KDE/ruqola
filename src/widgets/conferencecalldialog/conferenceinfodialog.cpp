/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferenceinfodialog.h"
using namespace Qt::Literals::StringLiterals;

#include "conferenceinfowidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ConferenceInfoDialog::ConferenceInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mConferenceInfoWidget(new ConferenceInfoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Conference Call Information"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mConferenceInfoWidget->setObjectName(u"mConferenceInfoWidget"_s);
    mainLayout->addWidget(mConferenceInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ConferenceInfoDialog::accept);
}

ConferenceInfoDialog::~ConferenceInfoDialog() = default;

void ConferenceInfoDialog::initializeInfo()
{
    mConferenceInfoWidget->initializeInfo();
}

QString ConferenceInfoDialog::conferenceId() const
{
    return mConferenceInfoWidget->conferenceId();
}

void ConferenceInfoDialog::setConferenceId(const QString &newConferenceId)
{
    mConferenceInfoWidget->setConferenceId(newConferenceId);
}

#include "moc_conferenceinfodialog.cpp"
