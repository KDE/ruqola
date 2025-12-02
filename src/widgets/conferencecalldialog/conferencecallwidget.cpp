/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QFormLayout>
#include <QToolButton>

ConferenceCallWidget::ConferenceCallWidget(QWidget *parent)
    : QWidget{parent}
    , mMicroButton(new QToolButton(this))
    , mCameraButton(new QToolButton(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainLayout"_s);

    mMicroButton->setObjectName(u"mMicroButton"_s);
    mMicroButton->setCheckable(true);
    mMicroButton->setAutoRaise(true);
    mMicroButton->setIcon(QIcon::fromTheme(u"mic-off"_s));
    mCameraButton->setObjectName(u"mCameraButton"_s);
    mCameraButton->setCheckable(true);
    mCameraButton->setAutoRaise(true);
    mCameraButton->setIcon(QIcon::fromTheme(u"camera-on"_s));
    connect(mCameraButton, &QToolButton::toggled, this, [this](bool clicked) {
        mCameraButton->setIcon(clicked ? QIcon::fromTheme(u"camera-on"_s) : QIcon::fromTheme(u"camera-off"_s));
        mCameraButton->setToolTip(clicked ? i18n("Camera On") : i18n("Camera Off"));
    });
    connect(mMicroButton, &QToolButton::toggled, this, [this](bool clicked) {
        mMicroButton->setIcon(clicked ? QIcon::fromTheme(u"mic-on"_s) : QIcon::fromTheme(u"mic-off"_s));
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

void ConferenceCallWidget::setConferenceCallInfo(ConferenceCallStart infoCallStart)
{
    mCameraButton->setChecked(infoCallStart.useCamera);
    mMicroButton->setChecked(infoCallStart.useMic);
}

#include "moc_conferencecallwidget.cpp"
