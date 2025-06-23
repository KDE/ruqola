/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelrolesinfowidget.h"
using namespace Qt::Literals::StringLiterals;

#include "roles/roles.h"
#include "room.h"

#include <KLocalizedString>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>

ChannelRolesInfoWidget::ChannelRolesInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mFormLayout(new QFormLayout())
{
    mFormLayout->setObjectName(u"mFormLayout"_s);
    auto box = new QGroupBox(i18n("Roles"), this);
    box->setObjectName(u"box"_s);
    box->setLayout(mFormLayout);
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(box);
}

ChannelRolesInfoWidget::~ChannelRolesInfoWidget() = default;

void ChannelRolesInfoWidget::setRoom(Room *room)
{
    if (room) {
        const Roles rolesForRoom = room->rolesForRooms();
        QStringList listOwners;
        const auto roles = rolesForRoom.roles();
        for (const Role &role : roles) {
            if (role.isOwner()) {
                listOwners.append(role.userName());
            }
        }
        QStringList listModerators;
        for (const Role &role : roles) {
            if (role.isModerator()) {
                listModerators.append(role.userName());
            }
        }
        QStringList listLeaders;
        for (const Role &role : roles) {
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
        if (!listOwners.isEmpty() || !listModerators.isEmpty() || !listLeaders.isEmpty()) {
            show();
        } else {
            hide();
        }
    } else {
        hide();
    }
}

void ChannelRolesInfoWidget::generateInfo(const QString &labelStr, const QStringList &list)
{
    auto label = new QLabel(this);
    label->setTextFormat(Qt::PlainText);
    label->setText(list.join(u','));
    label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label->setWordWrap(true);
    auto labelInfo = new QLabel(this);
    labelInfo->setTextFormat(Qt::RichText);
    labelInfo->setText(u"<qt><b>%1</b></qt>"_s.arg(labelStr));
    mFormLayout->addRow(labelInfo, label);
}

#include "moc_channelrolesinfowidget.cpp"
