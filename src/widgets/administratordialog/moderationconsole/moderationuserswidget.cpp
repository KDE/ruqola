/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "colorsandmessageviewstyle.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QTextBrowser>
#include <QVBoxLayout>

ModerationUsersWidget::ModerationUsersWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTextBrowser(new QTextBrowser(this))
    , mCurrentRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mainLayout->addWidget(mTextBrowser);
}

ModerationUsersWidget::~ModerationUsersWidget() = default;

void ModerationUsersWidget::setModerationReportUserInfos(const ModerationReportUserInfos &infos)
{
    // TODO add user icons !
    const QColor codeBackgroundColor = ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::NegativeBackground).color();
    QString html;
    const User user = infos.user();
    if (!infos.user().userEmailsInfo().email.isEmpty()) {
        html += QStringLiteral("<div><b>") + QStringLiteral("%1 (@%2)").arg(user.name(), user.userName()) + QStringLiteral("</div>");
        html += QStringLiteral("<br/>");
        html += QStringLiteral("<div><b>") + i18n("Email:") + QStringLiteral("</b>") + QStringLiteral(" %1").arg(infos.user().userEmailsInfo().email)
            + QStringLiteral("</div>");
        html += QStringLiteral("<br/>");
    }

    if (!user.roles().isEmpty()) {
        html += QStringLiteral("<div><b>") + i18n("Roles:") + QStringLiteral("</b>") + QStringLiteral(" %1").arg(infos.user().roles().join(", "_L1))
            + QStringLiteral("</div>");
        html += QStringLiteral("<br/>");
    }

    const QList<ModerationReportUserInfo> moderationReportUserInfosList = infos.moderationReportUserInfosList();
    int i = 1;
    for (const auto &info : moderationReportUserInfosList) {
        html +=
            QStringLiteral("<div><a style='background-color:") + codeBackgroundColor.name() + "'>"_L1 + i18n("Report #%1", i) + QStringLiteral("</a></div>");
        html += QStringLiteral("<div>") + info.description() + QStringLiteral("</div>");
        QLocale l;
        const QString createAtDisplayDateTime = l.toString(QDateTime::fromMSecsSinceEpoch(info.timeStamp()), QLocale::ShortFormat);
        html += QStringLiteral("<div><i>") + info.reportedBy().userName() + QLatin1Char(' ') + createAtDisplayDateTime + QStringLiteral("</i></div>");
        html += QStringLiteral("<br/>");
        ++i;
    }

    mTextBrowser->setHtml(html);
}

#include "moc_moderationuserswidget.cpp"
