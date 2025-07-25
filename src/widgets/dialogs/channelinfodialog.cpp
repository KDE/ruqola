/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfodialog.h"
using namespace Qt::Literals::StringLiterals;

#include "channelinfowidget.h"
#include "room.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myConfigChannelInfoDialogGroupName[] = "ChannelInfoDialog";
}

ChannelInfoDialog::ChannelInfoDialog(Room *room, RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mChannelInfoWidget(new ChannelInfoWidget(room, account, this))
    , mButtonBox(new QDialogButtonBox(this))
{
    setWindowTitle(i18nc("@title:window", "Channel Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mChannelInfoWidget->setObjectName(u"mChannelInfoWidget"_s);
    mainLayout->addWidget(mChannelInfoWidget);
    mChannelInfoWidget->updateUiFromPermission();

    connect(mButtonBox, &QDialogButtonBox::rejected, this, &ChannelInfoDialog::reject);
    connect(mButtonBox, &QDialogButtonBox::accepted, this, &ChannelInfoDialog::accept);
    mainLayout->addWidget(mButtonBox);
    connect(mChannelInfoWidget, &ChannelInfoWidget::channelDeleted, this, &ChannelInfoDialog::close);
    connect(mChannelInfoWidget, &ChannelInfoWidget::fnameChanged, this, &ChannelInfoDialog::slotFnameChanged);
    connect(mChannelInfoWidget, &ChannelInfoWidget::roomNameValid, this, &ChannelInfoDialog::slotRoomNameValid);

    mButtonBox->setStandardButtons(room->canBeModify() ? QDialogButtonBox::Ok | QDialogButtonBox::Cancel : QDialogButtonBox::Close);
    mOkButton = mButtonBox->button(QDialogButtonBox::Ok);
    mIsATeam = room->teamInfo().mainTeam();
    slotFnameChanged(room->displayFName());
    readConfig();
}

ChannelInfoDialog::~ChannelInfoDialog()
{
    writeConfig();
}

RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo ChannelInfoDialog::saveRoomSettingsInfo() const
{
    return mChannelInfoWidget->saveRoomSettingsInfo();
}

void ChannelInfoDialog::slotFnameChanged(const QString &fname)
{
    setWindowTitle(mIsATeam ? i18nc("@title:window", "Team Info about \'%1\'", fname) : i18nc("@title:window", "Channel Info about \'%1\'", fname));
}

void ChannelInfoDialog::slotRoomNameValid(bool state)
{
    Q_ASSERT(mOkButton);
    mOkButton->setEnabled(state);
}

void ChannelInfoDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigChannelInfoDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ChannelInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigChannelInfoDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_channelinfodialog.cpp"
