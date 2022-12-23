/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferenceinfowidget.h"
#include "common/flowlayout.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "video-conference/videoconferenceinfojob.h"
#include "videoconference/videoconferenceinfo.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QIcon>
#include <QLabel>
#include <QScreen>

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

        const QString statusInformation = info.statusInformation();
        if (!statusInformation.isEmpty()) {
            mFormLayout->addRow(i18n("Status:"), new QLabel(statusInformation, this));
        }
        const auto users = info.users();
        if (!users.isEmpty()) {
            auto avatarLayout = new FlowLayout;
            mFormLayout->addItem(avatarLayout);
            for (const auto &user : users) {
                auto avatarLabel = new QLabel(this);
                mFormLayout->addWidget(avatarLabel);
                Utils::AvatarInfo info;
                info.avatarType = Utils::AvatarType::User;
                info.identifier = user.userName();
                const QUrl iconUrlStr = QUrl(mRocketChatAccount->avatarUrl(info));
                const QSize pixmapAvatarSize = QSize(80, 80) * screen()->devicePixelRatio();
                avatarLabel->setPixmap(QIcon(iconUrlStr.toLocalFile()).pixmap(pixmapAvatarSize));
                avatarLabel->setToolTip(user.userName());
            }
        }
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
