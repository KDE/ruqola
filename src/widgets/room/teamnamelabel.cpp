/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamnamelabel.h"
using namespace Qt::Literals::StringLiterals;

TeamNameLabel::TeamNameLabel(QWidget *parent)
    : QLabel(parent)
{
    setTextFormat(Qt::RichText);
    setContextMenuPolicy(Qt::NoContextMenu);
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(this, &QLabel::linkActivated, this, &TeamNameLabel::slotGoToRoomTeam);
    QMargins margs(contentsMargins());
    margs.setLeft(5);
    setContentsMargins(margs);
}

TeamNameLabel::~TeamNameLabel() = default;

void TeamNameLabel::slotGoToRoomTeam(const QString &contents)
{
    if (contents == "gotoroomteam"_L1) {
        Q_EMIT openTeam(mTeamNameInfo.teamIdentifier);
    }
}

void TeamNameLabel::setTeamRoomInfo(const Room::TeamRoomInfo &newTeamNameInfo)
{
    mTeamNameInfo = newTeamNameInfo;
    setText(u"<a href=\"gotoroomteam\">[%1]</a>"_s.arg(mTeamNameInfo.teamName));
}

#include "moc_teamnamelabel.cpp"
