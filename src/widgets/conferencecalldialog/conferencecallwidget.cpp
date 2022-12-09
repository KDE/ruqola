/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallwidget.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "video-conference/videoconferencecapabilitiesjob.h"
#include <KLocalizedString>
#include <QVBoxLayout>

ConferenceCallWidget::ConferenceCallWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    if (mRocketChatAccount) {
        initialize();
    }
}

ConferenceCallWidget::~ConferenceCallWidget() = default;

void ConferenceCallWidget::initialize()
{
    auto job = new RocketChatRestApi::VideoConferenceCapabilitiesJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::VideoConferenceCapabilitiesJob::videoConferenceCapabilitiesDone, this, [this](const QJsonObject &obj) {
        qDebug() << "obj  " << obj;
        // {"capabilities":{"cam":true,"mic":true,"title":true},"providerName":"jitsi","success":true}
        const QJsonObject capabilitiesObj = obj[QLatin1String("capabilities")].toObject();
        const bool useCam = capabilitiesObj[QLatin1String("cam")].toBool();
        const bool useMic = capabilitiesObj[QLatin1String("mic")].toBool();
        // TODO
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceCapabilitiesJob job";
    }
}
