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
#include <QToolButton>
#include <QVBoxLayout>

ConferenceCallWidget::ConferenceCallWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mMicroButton(new QToolButton(this))
    , mCameraButton(new QToolButton(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mMicroButton->setObjectName(QStringLiteral("mMicroButton"));
    mCameraButton->setObjectName(QStringLiteral("mCameraButton"));

    mainLayout->addWidget(mMicroButton);
    mainLayout->addWidget(mCameraButton);

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
        mCameraButton->setChecked(useCam);
        mMicroButton->setChecked(useMic);
        // TODO verify if system has mic/cam too
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceCapabilitiesJob job";
    }
}

ConferenceCallWidget::ConferenceCallStart ConferenceCallWidget::startInfo() const
{
    ConferenceCallStart info;
    info.useCamera = mCameraButton->isChecked();
    info.useMic = mMicroButton->isChecked();
    return info;
}
