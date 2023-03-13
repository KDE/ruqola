/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallwidget.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "video-conference/videoconferencecapabilitiesjob.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QToolButton>

ConferenceCallWidget::ConferenceCallWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mMicroButton(new QToolButton(this))
    , mCameraButton(new QToolButton(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mMicroButton->setObjectName(QStringLiteral("mMicroButton"));
    mMicroButton->setCheckable(true);
    mMicroButton->setAutoRaise(true);
    mMicroButton->setIcon(QIcon::fromTheme(QStringLiteral("mic-off")));
    mCameraButton->setObjectName(QStringLiteral("mCameraButton"));
    mCameraButton->setCheckable(true);
    mCameraButton->setAutoRaise(true);
    mCameraButton->setIcon(QIcon::fromTheme(QStringLiteral("camera-on")));
    connect(mCameraButton, &QToolButton::toggled, this, [this](bool clicked) {
        mCameraButton->setIcon(clicked ? QIcon::fromTheme(QStringLiteral("camera-on")) : QIcon::fromTheme(QStringLiteral("camera-off")));
        mCameraButton->setToolTip(clicked ? i18n("Camera On") : i18n("Camera Off"));
    });
    connect(mMicroButton, &QToolButton::toggled, this, [this](bool clicked) {
        mMicroButton->setIcon(clicked ? QIcon::fromTheme(QStringLiteral("mic-on")) : QIcon::fromTheme(QStringLiteral("mic-off")));
        mMicroButton->setToolTip(clicked ? i18n("Micro On") : i18n("Micro Off"));
    });

    mainLayout->addRow(i18n("Micro"), mMicroButton);
    mainLayout->addRow(i18n("Camera"), mCameraButton);

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
        // qDebug() << "obj  " << obj;
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

ConferenceCallWidget::ConferenceCallStart ConferenceCallWidget::conferenceCallInfo() const
{
    ConferenceCallStart info;
    info.useCamera = mCameraButton->isChecked();
    info.useMic = mMicroButton->isChecked();
    return info;
}

void ConferenceCallWidget::setConferenceCallInfo(const ConferenceCallStart &infoCallStart)
{
    mCameraButton->setChecked(infoCallStart.useCamera);
    mMicroButton->setChecked(infoCallStart.useMic);
}
