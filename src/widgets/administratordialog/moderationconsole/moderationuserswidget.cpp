/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserswidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QTextBrowser>
#include <QVBoxLayout>

ModerationUsersWidget::ModerationUsersWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTextBrowser(new QTextBrowser(this))
    , mCurrentRocketChatAccount(account)
// TODO add list ? or QTextDocument
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mainLayout->addWidget(mTextBrowser);
}

ModerationUsersWidget::~ModerationUsersWidget() = default;

void ModerationUsersWidget::setModerationReportUserInfos(const ModerationReportUserInfos &infos)
{
    QString html;
    const User user = infos.user();
    if (!infos.user().userEmailsInfo().email.isEmpty()) {
        html = QStringLiteral("<div>") + i18n("Email: %1", infos.user().userEmailsInfo().email) + QStringLiteral("</div>");
    }

    if (!infos.user().roles().join(", "_L1).isEmpty()) {
        html += QStringLiteral("<div>") + i18n("Roles: %2", infos.user().roles().join(", "_L1)) + QStringLiteral("</div>");
    }

    const QList<ModerationReportUserInfo> moderationReportUserInfosList = infos.moderationReportUserInfosList();
    for (const auto &info : moderationReportUserInfosList) {
        // TODO
    }

    qDebug() << " infos.user().userEmailsInfo() " << infos.user().userEmailsInfo();
    qDebug() << " html" << html;
    mTextBrowser->setHtml(html);
    // TODO generate infos
}

#include "moc_moderationuserswidget.cpp"
