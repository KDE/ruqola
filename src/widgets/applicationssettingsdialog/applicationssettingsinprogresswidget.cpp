/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsinprogresswidget.h"
#include <KPixmapSequenceLoader>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <kiconloader.h>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsInProgressWidget::ApplicationsSettingsInProgressWidget(QWidget *parent)
    : QWidget{parent}
    , mLabel(new QLabel(this))
    , mProgressTimer(new QTimer(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mLabel->setObjectName("label"_L1);
    mainLayout->addWidget(mLabel);

    mProgressPix = KPixmapSequenceLoader::load(QStringLiteral("process-working"), KIconLoader::SizeSmallMedium);
    connect(mProgressTimer, &QTimer::timeout, this, &ApplicationsSettingsInProgressWidget::slotTimerDone);
}

ApplicationsSettingsInProgressWidget::~ApplicationsSettingsInProgressWidget() = default;

void ApplicationsSettingsInProgressWidget::slotTimerDone()
{
    mLabel->setPixmap(mProgressPix.frameAt(mProgressCount));
    ++mProgressCount;
    if (mProgressCount == 8) {
        mProgressCount = 0;
    }

    mProgressTimer->start(300);
}

void ApplicationsSettingsInProgressWidget::startAnimation()
{
    mProgressCount = 0;
    mProgressTimer->start(300);
}

void ApplicationsSettingsInProgressWidget::stopAnimation()
{
    if (mProgressTimer->isActive()) {
        mProgressTimer->stop();
    }
}

#include "moc_applicationssettingsinprogresswidget.cpp"
