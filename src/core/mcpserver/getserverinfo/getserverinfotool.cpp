/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "getserverinfotool.h"
#include "rocketchataccount.h"
#include "ruqolaserverconfig.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolToolAnnotations>

using namespace Qt::Literals::StringLiterals;
GetServerInfoTool::GetServerInfoTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

GetServerInfoTool::~GetServerInfoTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool GetServerInfoTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool = TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(
        u"get_server_info"_s,
        u"Get information about the Rocket.Chat server of an account"_s,
        QMap<QString, QJsonObject>{
            {u"accountName"_s, McpProtocolServerTool::schemaProperty(u"string"_s, u"Account name (see list_accounts); current account if omitted"_s)}});

    TextAutoGenerateTextMcpProtocolCore::McpProtocolToolAnnotations annotation;
    annotation.setReadOnlyHint(true);
    annotation.setOpenWorldHint(false);
    tool.setTitle(i18n("Get Server Info"));
    tool.setAnnotations(annotation);
    return tool;
}

void GetServerInfoTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    const RocketChatAccount *account = accountFromArguments(call);
    if (!account) {
        call->finishWithText(i18n("No account found"), true);
        return;
    }
    const RuqolaServerConfig *config = account->ruqolaServerConfig();
    const auto yesNo = [](bool value) {
        return value ? u"yes"_s : u"no"_s;
    };

    QStringList entries;
    entries.append(u"account: %1"_s.arg(account->accountName()));
    entries.append(u"server url: %1"_s.arg(account->serverUrl()));
    entries.append(u"login status: %1"_s.arg(account->loginStatusText()));
    entries.append(u"user: %1"_s.arg(account->userName()));
    if (const QString siteName = config->siteName(); !siteName.isEmpty()) {
        entries.append(u"site name: %1"_s.arg(siteName));
    }
    if (const QString siteUrl = config->siteUrl(); !siteUrl.isEmpty()) {
        entries.append(u"site url: %1"_s.arg(siteUrl));
    }
    if (const QString serverVersion = config->serverVersion(); !serverVersion.isEmpty()) {
        entries.append(u"server version: %1"_s.arg(serverVersion));
    }
    entries.append(u"enterprise: %1"_s.arg(yesNo(config->hasEnterpriseSupport())));
    entries.append(u"federation enabled: %1"_s.arg(yesNo(config->federationEnabled())));
    entries.append(u"encryption enabled: %1"_s.arg(yesNo(config->encryptionEnabled())));
    entries.append(u"threads enabled: %1"_s.arg(yesNo(config->threadsEnabled())));
    entries.append(u"discussions enabled: %1"_s.arg(yesNo(config->discussionEnabled())));
    entries.append(u"auto translate enabled: %1"_s.arg(yesNo(config->autoTranslateEnabled())));
    entries.append(u"two factor authentication enabled: %1"_s.arg(yesNo(config->twoFactorAuthenticationEnabled())));
    entries.append(u"file upload enabled: %1"_s.arg(yesNo(config->uploadFileEnabled())));
    if (const qint64 fileMaxFileSize = config->fileMaxFileSize(); fileMaxFileSize > 0) {
        entries.append(u"file max size (bytes): %1"_s.arg(fileMaxFileSize));
    }
    if (const int messageMaximumAllowedSize = config->messageMaximumAllowedSize(); messageMaximumAllowedSize > 0) {
        entries.append(u"message maximum allowed size: %1"_s.arg(messageMaximumAllowedSize));
    }

    call->finishWithText(entries.join(u'\n'));
}
