/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencedirectcalldialog.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "video-conference/videoconferencecanceljob.h"
#include "video-conference/videoconferencestartjob.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ConferenceDirectCallDialog::ConferenceDirectCallDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mConferenceCallWidget(new ConferenceCallWidget(account, this))
    , mRocketChatAccount(account)
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
    connect(button, &QDialogButtonBox::rejected, this, &ConferenceDirectCallDialog::slotRejected);
    connect(button, &QDialogButtonBox::accepted, this, &ConferenceDirectCallDialog::slotStartVideoConference);
}

ConferenceDirectCallDialog::~ConferenceDirectCallDialog() = default;

void ConferenceDirectCallDialog::slotRejected()
{
    auto job = new RocketChatRestApi::VideoConferenceCancelJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::VideoConferenceCancelJob::videoConferenceCancelDone, this, [this](const QJsonObject &obj) {
        qDebug() << "obj  " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceCancelJob job";
    }
    reject();
}

void ConferenceDirectCallDialog::slotStartVideoConference()
{
    const ConferenceCallWidget::ConferenceCallStart callInfo = mConferenceCallWidget->startInfo();

    auto job = new RocketChatRestApi::VideoConferenceStartJob(this);
    RocketChatRestApi::VideoConferenceStartJob::VideoConferenceStartInfo startInfo;
    startInfo.roomId = mRoomId;
    startInfo.allowRinging = mAllowRinging;
    job->setInfo(startInfo);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::VideoConferenceStartJob::videoConferenceStartDone, this, [this, callInfo](const QJsonObject &obj) {
        qDebug() << " obj " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceStartJob job";
    }
}

bool ConferenceDirectCallDialog::allowRinging() const
{
    return mAllowRinging;
}

void ConferenceDirectCallDialog::setAllowRinging(bool newAllowRinging)
{
    mAllowRinging = newAllowRinging;
}

QString ConferenceDirectCallDialog::roomId() const
{
    return mRoomId;
}

void ConferenceDirectCallDialog::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}
