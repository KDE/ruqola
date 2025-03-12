/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionutil.h"
using namespace Qt::Literals::StringLiterals;
QJsonObject AutoGenerateInteractionUtil::createViewBlockActionUser(const ViewBlockActionUserInfo &info)
{
    QJsonObject o;
    o["type"_L1] = "blockAction"_L1;
    o["actionId"_L1] = QString::fromLatin1(info.actionId);
    QJsonObject payload;
    payload["blockId"_L1] = QString::fromLatin1(info.blockIdPayload);
    payload["value"_L1] = info.valuePayload;
    o["payload"_L1] = payload;

    QJsonObject container;
    container["type"_L1] = "view"_L1;
    container["id"_L1] = QString::fromLatin1(info.idContainer);
    o["container"_L1] = container;
    o["triggerId"_L1] = QString::fromLatin1(info.triggerId);
    return o;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ViewBlockActionUserInfo &t)
{
    d.space() << "actionId:" << t.actionId;
    d.space() << "blockIdPayload:" << t.blockIdPayload;
    d.space() << "valuePayload:" << t.valuePayload;
    d.space() << "idContainer:" << t.idContainer;
    d.space() << "triggerId:" << t.triggerId;
    return d;
}

QJsonObject AutoGenerateInteractionUtil::createViewClosedUser(const QJsonObject &payload, const QString &triggerId)
{
    QJsonObject o;
    o["type"_L1] = "viewClosed"_L1;
    o["payload"_L1] = payload;
    o["triggerId"_L1] = triggerId;
    return o;
}

/*
export type ViewClosedUserInteraction = {
        type: 'viewClosed';
        payload: {
                viewId: string;
                view: View & {
                        id: string;
                        state: { [blockId: string]: { [key: string]: unknown } };
                };
                isCleared?: boolean;
        };
        triggerId: string;
};
*/

QJsonObject AutoGenerateInteractionUtil::createViewSubmitUser(const ViewBlockActionUserInfo &info)
{
    QJsonObject o;
    o["type"_L1] = "viewSubmit"_L1;
    QJsonObject payload;
    o["payload"_L1] = payload;

    o["triggerId"_L1] = QString::fromLatin1(info.triggerId);
    // TODO
    return o;
}
/*
export type ViewSubmitUserInteraction = {
        type: 'viewSubmit';
        actionId?: undefined;
        payload: {
                view: View & {
                        id: string;
                        state: { [blockId: string]: { [key: string]: unknown } };
                };
        };
        triggerId: string;
        viewId: string;
};

*/

QJsonObject AutoGenerateInteractionUtil::createRoomActionButton(const ActionButtonInfo &info)
{
    // {"type":"actionButton","actionId":"auto-reply-room-action-id","rid":"67b7116fc0984e12f9661a2c","payload":{"context":"roomAction"},"triggerId":"EdEdKAKzxMTu9io9L"}
    QJsonObject o;
    o["type"_L1] = "actionButton"_L1;
    o["actionId"_L1] = QString::fromLatin1(info.actionId);
    QJsonObject payload;
    payload["context"_L1] = "roomAction"_L1;
    o["payload"_L1] = payload;

    o["rid"_L1] = QString::fromLatin1(info.roomId);
    o["triggerId"_L1] = QString::fromLatin1(info.triggerId);
    return o;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ActionButtonInfo &t)
{
    d.space() << "actionId:" << t.actionId;
    d.space() << "triggerId:" << t.triggerId;
    d.space() << "roomId:" << t.roomId;
    return d;
}

QJsonObject AutoGenerateInteractionUtil::createMessageActionButton(const ActionMessageInfo &info)
{
    // {"type":"actionButton","rid":"673d89fbc44b2a134443eb7d","mid":"4javjtcaA9KoaNXWa","actionId":"remind_me_about_this_msg_action","payload":{"context":"messageAction"},"triggerId":"KH6gZae9SouePp3KW"}
    QJsonObject o;
    o["type"_L1] = "actionButton"_L1;
    o["actionId"_L1] = QString::fromLatin1(info.actionId);
    o["mid"_L1] = QString::fromLatin1(info.messageId);
    QJsonObject payload;
    payload["context"_L1] = "messageAction"_L1;
    o["payload"_L1] = payload;

    o["rid"_L1] = QString::fromLatin1(info.roomId);
    o["triggerId"_L1] = QString::fromLatin1(info.triggerId);
    return o;
}
QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ActionMessageInfo &t)
{
    d.space() << "actionId:" << t.actionId;
    d.space() << "triggerId:" << t.triggerId;
    d.space() << "roomId:" << t.roomId;
    d.space() << "messageId:" << t.messageId;
    return d;
}
