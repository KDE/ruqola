/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "toolbase.h"
#include "accountmanager.h"
#include "connection.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolServerToolCall>
using namespace Qt::Literals::StringLiterals;
namespace
{
constexpr int defaultSearchCount = 25;
constexpr int maximumSearchCount = 100;
}

ToolBase::ToolBase(AccountManager *accountManager)
    : TextAutoGenerateTextMcpProtocolCore::McpProtocolServerTool()
    , mAccountManager(accountManager)
{
}

ToolBase::~ToolBase() = default;

RocketChatAccount *ToolBase::accountFromArguments(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call) const
{
    const QJsonValue accountNameValue = call->arguments().value(u"accountName"_s);
    if (!accountNameValue.isString()) {
        return mAccountManager->account();
    }
    return mAccountManager->accountFromName(accountNameValue.toString());
}

void ToolBase::searchDirectory(RocketChatAccount *account,
                               TextAutoGenerateTextMcpProtocolCore::McpProtocolServerToolCall *call,
                               RocketChatRestApi::DirectoryJob::SearchType searchType,
                               const std::function<void(const QJsonObject &)> &done) const
{
    if (account->loginStatus() != AuthenticationManager::LoggedIn) {
        call->finishWithText(i18n("Account is not logged in"), true);
        return;
    }
    QString pattern = call->arguments().value(u"pattern"_s).toString().trimmed();
    if (pattern.startsWith(searchType == RocketChatRestApi::DirectoryJob::SearchType::Users ? u'@' : u'#')) {
        pattern.removeFirst();
    }
    if (pattern.isEmpty()) {
        call->finishWithText(i18n("Missing search pattern"), true);
        return;
    }
    const int count = std::clamp(call->arguments().value(u"count"_s).toInt(defaultSearchCount), 1, maximumSearchCount);

    // Job is owned by call: it is deleted when the client cancels the request
    auto job = new RocketChatRestApi::DirectoryJob(call);
    job->setDirectoryInfo({.searchType = searchType, .pattern = pattern});
    RocketChatRestApi::QueryParameters parameters;
    parameters.setSorting({{u"name"_s, RocketChatRestApi::QueryParameters::SortOrder::Ascendant}});
    parameters.setCount(count);
    parameters.setUseSyntaxRc70(account->hasAtLeastVersion(7, 0, 0));
    job->setQueryParameters(parameters);
    account->restApi()->initializeRestApiJob(job);
    QObject::connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, call, done);
    QObject::connect(job, &RocketChatRestApi::DirectoryJob::failed, call, [call](const QString &serverErrorStr, const QString &descriptionError) {
        call->finishWithText(i18n("Search failed: %1", descriptionError.isEmpty() ? serverErrorStr : descriptionError), true);
    });
    if (!job->start()) {
        call->finishWithText(i18n("Impossible to start search"), true);
    }
}
