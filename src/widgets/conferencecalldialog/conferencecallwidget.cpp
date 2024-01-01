/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallwidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QToolButton>

ConferenceCallWidget::ConferenceCallWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mMicroButton(new QToolButton(this))
    , mCameraButton(new QToolButton(this))
{
    Q_UNUSED(account);
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
}

ConferenceCallWidget::~ConferenceCallWidget() = default;

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

#include "moc_conferencecallwidget.cpp"
