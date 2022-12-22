/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferenceinfowidget.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "video-conference/videoconferenceinfojob.h"
#include "videoconference/videoconferenceinfo.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>

ConferenceInfoWidget::ConferenceInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mFormLayout(new QFormLayout(this))
    , mRocketChatAccount(account)
{
    mFormLayout->setContentsMargins({});
    mFormLayout->setObjectName(QStringLiteral("mFormLayout"));
}

ConferenceInfoWidget::~ConferenceInfoWidget() = default;

void ConferenceInfoWidget::initializeInfo()
{
    auto conferenceInfoJob = new RocketChatRestApi::VideoConferenceInfoJob(this);
    conferenceInfoJob->setCallId(mConferenceId);
    mRocketChatAccount->restApi()->initializeRestApiJob(conferenceInfoJob);
    connect(conferenceInfoJob, &RocketChatRestApi::VideoConferenceInfoJob::videoConferenceInfoDone, this, [this](const QJsonObject &obj) {
        VideoConferenceInfo info;
        info.parse(obj);
        auto meetingUrlLabel = new QLabel(this);
        meetingUrlLabel->setText(QStringLiteral("<a href=\"%1\">%1</a>").arg(info.url()));
        meetingUrlLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
        meetingUrlLabel->setOpenExternalLinks(true);

        mFormLayout->addRow(i18n("Meeting URL:"), meetingUrlLabel);
        mFormLayout->addRow(i18n("Provider:"), new QLabel(info.providerName(), this));

        if (info.endedAtDateTime().isValid()) {
            if (info.conferenceType() == VideoConferenceInfo::VideoConferenceType::Direct) {
                mFormLayout->addRow(i18n("Status:"), new QLabel(i18n("Call was not answered"), this));
            } else if (info.conferenceType() == VideoConferenceInfo::VideoConferenceType::Conference && info.users().isEmpty()) {
                mFormLayout->addRow(i18n("Status:"), new QLabel(i18n("Call was not answered"), this));
            }
        } else {
            if (info.conferenceType() == VideoConferenceInfo::VideoConferenceType::Direct && info.status() == 0) {
                mFormLayout->addRow(i18n("Status:"), new QLabel(i18n("Waiting for answer"), this));
            } else {
                mFormLayout->addRow(i18n("Status:"), new QLabel(i18n("Call ongoing"), this));
            }
        }
        // TODO add users.
    });
    if (!conferenceInfoJob->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceInfoJob job";
    }
}

QString ConferenceInfoWidget::conferenceId() const
{
    return mConferenceId;
}

void ConferenceInfoWidget::setConferenceId(const QString &newConferenceId)
{
    mConferenceId = newConferenceId;
}
