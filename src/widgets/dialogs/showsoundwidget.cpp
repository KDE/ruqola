/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showsoundwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QToolButton>

ShowSoundWidget::ShowSoundWidget(QWidget *parent)
    : QWidget{parent}
    , mPlayButton(new QPushButton(this))
    , mPositionSlider(new QSlider(Qt::Horizontal, this))
    , mErrorLabel(new QLabel(this))
    , mSoundButton(new QToolButton(this))
    , mSoundSlider(new QSlider(Qt::Horizontal, this))
    , mLabelDuration(new QLabel(this))
    , mLabelPercentSound(new QLabel(this))

{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mLabelDuration->setObjectName(QStringLiteral("mLabelDuration"));

    auto controlLayout = new QHBoxLayout;
    controlLayout->setObjectName(QStringLiteral("controlLayout"));
    controlLayout->setContentsMargins({});
    mainLayout->addLayout(controlLayout);

    mPlayButton->setObjectName(QStringLiteral("mPlayButton"));
    mPlayButton->setEnabled(false);
    mPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    mErrorLabel->setTextFormat(Qt::PlainText);
    mLabelDuration->setTextFormat(Qt::PlainText);
    mLabelPercentSound->setTextFormat(Qt::PlainText);

    controlLayout->addWidget(mPlayButton);
    connect(mPlayButton, &QAbstractButton::clicked, this, &ShowSoundWidget::play);

    mPositionSlider->setObjectName(QStringLiteral("mPositionSlider"));
    mPositionSlider->setRange(0, 0);
    controlLayout->addWidget(mPositionSlider);
    controlLayout->addWidget(mLabelDuration);

    controlLayout->addWidget(mSoundButton);
    controlLayout->addWidget(mSoundSlider);
}

ShowSoundWidget::~ShowSoundWidget()
{
}

void ShowSoundWidget::setSoundUrl(const QUrl &videoPath)
{
    // TODO
}

QUrl ShowSoundWidget::soundUrl() const
{
    // TODO
    return {};
}

void ShowSoundWidget::play()
{
}
