/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamchannelsdialog.h"

#include "teamchannelswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myTeamChannelsDialogConfigGroupName[] = "TeamChannelsDialog";
}
using namespace Qt::Literals::StringLiterals;
TeamChannelsDialog::TeamChannelsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mTeamChannelsWidget(new TeamChannelsWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Team Channels"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mTeamChannelsWidget->setObjectName(u"mTeamChannelsWidget"_s);
    mainLayout->addWidget(mTeamChannelsWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &TeamChannelsDialog::reject);
    readConfig();
}

TeamChannelsDialog::~TeamChannelsDialog()
{
    writeConfig();
}

void TeamChannelsDialog::setRoom(Room *room)
{
    mTeamChannelsWidget->setRoom(room);
}

void TeamChannelsDialog::readConfig()
{
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myTeamChannelsDialogConfigGroupName), QSize(400, 300));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myTeamChannelsDialogConfigGroupName), 400, 300);
#endif
}

void TeamChannelsDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myTeamChannelsDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

#include "moc_teamchannelsdialog.cpp"
