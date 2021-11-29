/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractwebsocket.h"

AbstractWebSocket::AbstractWebSocket(QObject *parent)
    : QObject(parent)
{
}

AbstractWebSocket::~AbstractWebSocket() = default;
