/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsounddialog.h"
using namespace Qt::Literals::StringLiterals;

#include "playsoundwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

PlaySoundDialog::PlaySoundDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mSoundWidget(new PlaySoundWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Play Sound"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mSoundWidget->setObjectName(u"mSoundWidget"_s);
    mainLayout->addWidget(mSoundWidget);
    connect(mSoundWidget, &PlaySoundWidget::updateTitle, this, &PlaySoundDialog::slotUpdateTitle);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PlaySoundDialog::reject);
    mainLayout->addWidget(buttonBox);
}

PlaySoundDialog::~PlaySoundDialog() = default;

void PlaySoundDialog::setAudioPath(const QString &url)
{
    mSoundWidget->setAudioPath(url);
}

void PlaySoundDialog::slotUpdateTitle(const QUrl &url)
{
    setWindowTitle(i18nc("@title:window", "Sound: %1", url.fileName()));
}

#include "moc_playsounddialog.cpp"
