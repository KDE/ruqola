/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

namespace RocketChatRestApi
{
class RestApiAbstractJob;
}
class QNetworkRequest;
namespace RuqolaRestApiHelper
{
void verifyAuthentication(RocketChatRestApi::RestApiAbstractJob *job, QNetworkRequest &request);

void verifyDefaultValue(RocketChatRestApi::RestApiAbstractJob *job);
}
