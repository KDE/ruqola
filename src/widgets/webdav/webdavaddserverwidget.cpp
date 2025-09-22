/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "webdavaddserverwidget.h"
#include <KAuthorized>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>

using namespace Qt::Literals::StringLiterals;
WebDavAddServerWidget::WebDavAddServerWidget(QWidget *parent)
    : QWidget{parent}
    , mName(new QLineEdit(this))
    , mUrl(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mPasswordLineEdit(new KPasswordLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mName->setObjectName(u"mName"_s);
    mUrl->setObjectName(u"mUrl"_s);
    mUserName->setObjectName(u"mUserName"_s);
    KLineEditEventHandler::catchReturnKey(mName);
    KLineEditEventHandler::catchReturnKey(mUrl);
    KLineEditEventHandler::catchReturnKey(mUserName);
    KLineEditEventHandler::catchReturnKey(mPasswordLineEdit->lineEdit());
    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("Url:"), mUrl);
    mainLayout->addRow(i18n("UserName:"), mUserName);
    mainLayout->addRow(i18n("Password:"), mPasswordLineEdit);

    mPasswordLineEdit->setObjectName(u"mPasswordLineEdit"_s);
    mPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(u"lineedit_reveal_password"_s) ? KPassword::RevealMode::OnlyNew
                                                                                                   : KPassword::RevealMode::Never);
    connect(mName, &QLineEdit::textChanged, this, &WebDavAddServerWidget::slotUpdateOkButtonStatus);
    connect(mUrl, &QLineEdit::textChanged, this, &WebDavAddServerWidget::slotUpdateOkButtonStatus);
    connect(mUserName, &QLineEdit::textChanged, this, &WebDavAddServerWidget::slotUpdateOkButtonStatus);
    connect(mPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &WebDavAddServerWidget::slotUpdateOkButtonStatus);
}

WebDavAddServerWidget::~WebDavAddServerWidget() = default;

void WebDavAddServerWidget::slotUpdateOkButtonStatus()
{
    Q_EMIT okButtonEnabled(addServerInfo().isValid());
}

WebDavAddServerWidget::WebDavAddServerInfo WebDavAddServerWidget::addServerInfo() const
{
    return {.name = mName->text().trimmed(), .url = mUrl->text().trimmed(), .userName = mUserName->text().trimmed(), .password = mPasswordLineEdit->password()};
}

bool WebDavAddServerWidget::WebDavAddServerInfo::isValid() const
{
    return !name.isEmpty() && !url.isEmpty() && !userName.isEmpty() && !password.isEmpty();
}

#include "moc_webdavaddserverwidget.cpp"
