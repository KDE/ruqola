/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "ruqolacentralwidget.h"
#include "ruqolaloginwidget.h"
#include "ruqolamainwidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include <QVBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <KMessageBox>
#include <KLocalizedString>

RuqolaCentralWidget::RuqolaCentralWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mRuqolaMainWidget = new RuqolaMainWidget(this);
    mRuqolaMainWidget->setObjectName(QStringLiteral("mRuqolaMainWidget"));
    mStackedWidget->addWidget(mRuqolaMainWidget);

    mRuqolaLoginWidget = new RuqolaLoginWidget(this);
    mRuqolaLoginWidget->setObjectName(QStringLiteral("mRuqolaLoginWidget"));
    mStackedWidget->addWidget(mRuqolaLoginWidget);

    mStackedWidget->setCurrentWidget(mRuqolaLoginWidget);

    //Fix me multiaccount
    connect(Ruqola::self()->rocketChatAccount(), &RocketChatAccount::loginStatusChanged, this, &RuqolaCentralWidget::slotLoginStatusChanged);
    connect(Ruqola::self()->rocketChatAccount(), &RocketChatAccount::jobFailed, this, &RuqolaCentralWidget::slotJobFailedInfo);
}

RuqolaCentralWidget::~RuqolaCentralWidget()
{
}

void RuqolaCentralWidget::slotJobFailedInfo(const QString &messageError)
{
    //TODO fix i18n
    KMessageBox::error(this, messageError, i18n("Job Failed"));
}

QString RuqolaCentralWidget::roomId() const
{
    return mRuqolaMainWidget->roomId();
}

void RuqolaCentralWidget::slotLoginStatusChanged()
{
    /*        NotConnected,
        LoggingIn,
        LoggedIn,
        LoginFailed,
        LoginCodeRequired,
        LoggedOut,
        FailedToLoginPluginProblem
        */
    if (Ruqola::self()->rocketChatAccount()->loginStatus() == DDPClient::LoggedIn) {
        mStackedWidget->setCurrentWidget(mRuqolaMainWidget);
    } else {
        mStackedWidget->setCurrentWidget(mRuqolaLoginWidget);
        mRuqolaLoginWidget->initialize();
        mRuqolaLoginWidget->setLogginStatus(Ruqola::self()->rocketChatAccount()->loginStatus());
        //TODO assign value in mRuqolaLoginWidget directly
    }
    //TODO
}
