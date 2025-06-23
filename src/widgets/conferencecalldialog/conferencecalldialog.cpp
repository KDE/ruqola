/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecalldialog.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ConferenceCallDialog::ConferenceCallDialog(QWidget *parent)
    : QDialog(parent)
    , mConferenceCallWidget(new ConferenceCallWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Start a Call"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mConferenceCallWidget->setObjectName(u"mConferenceCallWidget"_s);
    mainLayout->addWidget(mConferenceCallWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    button->button(QDialogButtonBox::Ok)->setText(i18nc("@action:button", "Start"));
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
