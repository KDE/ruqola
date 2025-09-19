/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "webdavaddserverwidget.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

using namespace Qt::Literals::StringLiterals;
WebDavAddServerWidget::WebDavAddServerWidget(QWidget *parent)
    : QWidget{parent}
    , mName(new QLineEdit(this))
    , mUrl(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
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
    mainLayout->addRow(i18n("Name:"), mName);
    mainLayout->addRow(i18n("Url:"), mUrl);
    mainLayout->addRow(i18n("UserName:"), mUserName);
    // TODO add password
}

WebDavAddServerWidget::~WebDavAddServerWidget() = default;

WebDavAddServerWidget::WebDavAddServerInfo WebDavAddServerWidget::addServerInfo() const
{
    return {};
}

#include "moc_webdavaddserverwidget.cpp"
