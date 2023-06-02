/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

CreateSoundMessageWidget::CreateSoundMessageWidget(QWidget *parent)
    : QWidget{parent}
    , mRecordButton(new QToolButton(this))
    , mPauseButton(new QToolButton(this))
    , mStopButton(new QToolButton(this))
    , mLabelDuration(new QLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mStopButton->setObjectName(QStringLiteral("mStopButton"));
    mStopButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-stop")));
    mainLayout->addWidget(mStopButton);
    connect(mStopButton, &QToolButton::clicked, this, &CreateSoundMessageWidget::stop);

    mPauseButton->setObjectName(QStringLiteral("mPauseButton"));
    mPauseButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-pause")));
    mainLayout->addWidget(mPauseButton);
    connect(mPauseButton, &QToolButton::clicked, this, &CreateSoundMessageWidget::pause);

    mRecordButton->setObjectName(QStringLiteral("mRecordButton"));
    mRecordButton->setIcon(QIcon::fromTheme(QStringLiteral("media-record")));
    mainLayout->addWidget(mRecordButton);
    connect(mRecordButton, &QToolButton::clicked, this, &CreateSoundMessageWidget::record);

    mLabelDuration->setObjectName(QStringLiteral("mLabelDuration"));
    mLabelDuration->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(mLabelDuration);
}

CreateSoundMessageWidget::~CreateSoundMessageWidget() = default;

void CreateSoundMessageWidget::stop()
{
    // TODO
}

void CreateSoundMessageWidget::record()
{
    // TODO
}

void CreateSoundMessageWidget::pause()
{
    // TODO
}
