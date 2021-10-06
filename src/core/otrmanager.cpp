/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "otrmanager.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <KNotification>

OtrManager::OtrManager(QObject *parent)
    : QObject(parent)
{
}

OtrManager::~OtrManager()
{
}

void OtrManager::parseOtr(const QJsonArray &contents)
{
    Otr t;
    t.parseOtr(contents);
    if (t.isValid()) {
        switch (t.type()) {
        case Otr::OtrType::Unknown:
            qCWarning(RUQOLA_LOG) << "It's a bug we can't have otrtype == Unknown";
            break;
        case Otr::OtrType::End: {
            auto notification = new KNotification(QStringLiteral("Otr-end"), KNotification::CloseOnTimeout);
            notification->setTitle(i18n("OTR"));
            notification->setText(i18n("%1 ended the OTR session.", QStringLiteral("test"))); // FIXME use correct name
            notification->sendEvent();
            break;
        }
        case Otr::OtrType::Handshake: {
            auto notification = new KNotification(QStringLiteral("Otr-handshake"), KNotification::CloseOnTimeout);
            notification->setTitle(i18n("OTR"));
            notification->setText(i18n("%1  wants to start OTR. Do you want to accept?.", QStringLiteral("test"))); // FIXME use correct name
            // TODO add action Ok/Reject
            // TODO connect ok to accept.
            notification->sendEvent();
            break;
        }
        case Otr::OtrType::Deny: {
            auto notification = new KNotification(QStringLiteral("Otr-deny"), KNotification::CloseOnTimeout);
            notification->setTitle(i18n("OTR"));
            notification->setText(i18n("%1 denied the OTR session.", QStringLiteral("test"))); // FIXME use correct name
            notification->sendEvent();
            break;
        }
        case Otr::OtrType::AcknowLedge:
            // TODO accept OTR => we need to inform ruqolaaccount
            break;
        }
    }
}
