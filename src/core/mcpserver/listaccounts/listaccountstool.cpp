/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listaccountstool.h"
#include "accountmanager.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolToolAnnotations>
using namespace Qt::Literals::StringLiterals;
ListAccountsTool::ListAccountsTool(AccountManager *accountManager)
    : ToolBase(accountManager)
{
}

ListAccountsTool::~ListAccountsTool() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolTool ListAccountsTool::definition() const
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolTool tool =
        TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool::createTool(u"list_accounts"_s, u"List the accounts"_s);

    TextAutoGenerateTextMcpProtocolCore::McpProtocolToolAnnotations annotation;
    annotation.setReadOnlyHint(true);
    annotation.setOpenWorldHint(false);
    tool.setTitle(i18n("List Accounts"));
    tool.setAnnotations(annotation);
    // TODO ToolUtils::addToolMeta(tool, ToolUtils::Mail);
    return tool;
}

void ListAccountsTool::call(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call)
{
    const QString currentAccountName = mAccountManager->currentAccount();
    const RocketChatAccountModel *model = mAccountManager->rocketChatAccountModel();
    const int numberOfAccounts = model->accountNumber();
    QStringList entries;
    entries.reserve(numberOfAccounts);
    for (int i = 0; i < numberOfAccounts; ++i) {
        const RocketChatAccount *account = model->account(i);
        if (!account) {
            continue;
        }
        const QString accountName = account->accountName();
        QString entry =
            u"%1 (display name: %2, server: %3, user: %4)"_s.arg(accountName, account->settings()->displayName(), account->serverUrl(), account->userName());
        if (!account->accountEnabled()) {
            entry += u" [disabled]"_s;
        }
        if (accountName == currentAccountName) {
            entry += u" [current]"_s;
        }
        entries.append(entry);
    }

    if (entries.isEmpty()) {
        call->finishWithText(i18n("No account found"));
    } else {
        call->finishWithText(entries.join(u'\n'));
    }
}
