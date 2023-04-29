/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelrolesinfowidget.h"
#include "roles/roles.h"
#include "room.h"

#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>

ChannelRolesInfoWidget::ChannelRolesInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mFormLayout(new QFormLayout(this))
{
    mFormLayout->setObjectName(QStringLiteral("mFormLayout"));
    mFormLayout->setContentsMargins({});
}

ChannelRolesInfoWidget::~ChannelRolesInfoWidget() = default;

void ChannelRolesInfoWidget::setRoom(Room *room)
{
    if (room) {
        const Roles roles = room->rolesForRooms();
        QStringList listOwners;
        for (const Role &role : roles.roles()) {
            if (role.isOwner()) {
                listOwners.append(role.userName());
            }
        }
        QStringList listModerators;
        for (const Role &role : roles.roles()) {
            if (role.isModerator()) {
                listModerators.append(role.userName());
            }
        }
        QStringList listLeaders;
        for (const Role &role : roles.roles()) {
            if (role.isLeader()) {
                listLeaders.append(role.userName());
            }
        }
        if (!listOwners.isEmpty()) {
            generateInfo(i18np("Owner:", "Owners:", listOwners.count()), listOwners);
        }
        if (!listModerators.isEmpty()) {
            generateInfo(i18np("Moderator:", "Moderators:", listModerators.count()), listModerators);
        }
        if (!listLeaders.isEmpty()) {
            generateInfo(i18np("Leader:", "Leaders:", listLeaders.count()), listLeaders);
        }
    }
}

void ChannelRolesInfoWidget::generateInfo(const QString &labelStr, const QStringList &list)
{
    auto label = new QLabel(this);
    label->setTextFormat(Qt::PlainText);
    label->setText(list.join(QLatin1Char(',')));
    label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label->setWordWrap(true);
    auto labelInfo = new QLabel(this);
    labelInfo->setTextFormat(Qt::RichText);
    labelInfo->setText(QStringLiteral("<qt><b>%1</b></qt>").arg(labelStr));
    mFormLayout->addRow(labelInfo, label);
}
