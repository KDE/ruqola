/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsounddialog.h"
#include "playsoundwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

PlaySoundDialog::PlaySoundDialog(QWidget *parent)
    : QDialog(parent)
    , mSoundWidget(new PlaySoundWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Play Sound"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSoundWidget->setObjectName(QStringLiteral("mSoundWidget"));
    mainLayout->addWidget(mSoundWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PlaySoundDialog::reject);
    mainLayout->addWidget(buttonBox);
}

PlaySoundDialog::~PlaySoundDialog() = default;

void PlaySoundDialog::setAudioUrl(const QUrl &url)
{
    mSoundWidget->setAudioUrl(url);
    setWindowTitle(i18nc("@title:window", "Sound: %1", url.fileName()));
}

#include "moc_playsounddialog.cpp"
