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
    mainLayout->setObjectName(u"mainLayout"_s);
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
        html += u"<div><b>"_s + u"%1 (@%2)"_s.arg(user.name(), user.userName()) + u"</div>"_s;
        html += u"<br/>"_s;
        html += u"<div><b>"_s + i18n("Email:") + u"</b>"_s + u" %1"_s.arg(infos.user().userEmailsInfo().email) + u"</div>"_s;
        html += u"<br/>"_s;
    }

    if (!user.roles().isEmpty()) {
        html += u"<div><b>"_s + i18n("Roles:") + u"</b>"_s + u" %1"_s.arg(infos.user().roles().join(", "_L1)) + u"</div>"_s;
        html += u"<br/>"_s;
    }

    const QList<ModerationReportUserInfo> moderationReportUserInfosList = infos.moderationReportUserInfosList();
    int i = 1;
    for (const auto &info : moderationReportUserInfosList) {
        html += u"<div><a style='background-color:"_s + codeBackgroundColor.name() + "'>"_L1 + i18n("Report #%1", i) + u"</a></div>"_s;
        html += u"<div>"_s + info.description() + u"</div>"_s;
        QLocale l;
        const QString createAtDisplayDateTime = l.toString(QDateTime::fromMSecsSinceEpoch(info.timeStamp()), QLocale::ShortFormat);
        html += u"<div><i>"_s + info.reportedBy().userName() + u' ' + createAtDisplayDateTime + u"</i></div>"_s;
        html += u"<br/>"_s;
        ++i;
    }

    mTextBrowser->setHtml(html);
}

#include "moc_moderationuserswidget.cpp"
