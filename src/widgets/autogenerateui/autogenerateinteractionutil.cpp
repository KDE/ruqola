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

QJsonObject AutoGenerateInteractionUtil::createViewClosedUser(const ViewBlockActionUserInfo &info)
{
    QJsonObject o;
    o["type"_L1] = "viewClosed"_L1;
    QJsonObject payload;
    o["payload"_L1] = payload;
    o["triggerId"_L1] = QString::fromLatin1(info.triggerId);
    // TODO
    return {};
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
    return {};
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
