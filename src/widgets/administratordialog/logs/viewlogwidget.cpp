/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "viewlogwidget.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "misc/stdoutqueuejob.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <QJsonArray>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QVBoxLayout>

ViewLogWidget::ViewLogWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mPlainTextEdit(new QPlainTextEdit(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mPlainTextEdit->setObjectName(QStringLiteral("mPlainTextEdit"));
    mPlainTextEdit->setReadOnly(true);
    mainLayout->addWidget(mPlainTextEdit);
    // TODO add search widget
}

ViewLogWidget::~ViewLogWidget()
{
    if (mStdoutIdentifier != 0) {
        mRocketChatAccount->ddp()->unsubscribe(mStdoutIdentifier);
    }
}

void ViewLogWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous() && !mWasInitialized) {
        mWasInitialized = true;
        initialize();
    }
    QWidget::showEvent(event);
}

void ViewLogWidget::initialize()
{
    {
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("stdout")));
        mStdoutIdentifier = mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-stdout"), params);
    }
    connect(mRocketChatAccount, &RocketChatAccount::insertStdOutInfo, this, &ViewLogWidget::slotInsertStdOutInfo);

    auto job = new RocketChatRestApi::StdoutQueueJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::StdoutQueueJob::stdoutQueueDone, this, &ViewLogWidget::slotStdoutQueueDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start StdoutQueueJob job";
    }
}

void ViewLogWidget::slotInsertStdOutInfo(const QString &str)
{
    if (mHistoryStdoutLoaded) {
        insertLine(str);
    } else {
        mStdoutBeforeLoadingHistory.append(str);
    }
}

void ViewLogWidget::insertLine(const QString &str)
{
    mPlainTextEdit->appendHtml(QStringLiteral("<p white-space:pre\">%1</p>").arg(str));
}

void ViewLogWidget::slotStdoutQueueDone(const QJsonObject &obj)
{
    // qDebug() << " obj" << obj;
    const QJsonArray array = obj[QLatin1String("queue")].toArray();
    mPlainTextEdit->blockSignals(true);
    for (int i = 0; i < array.count(); ++i) {
        const QJsonObject objQueue = array.at(i).toObject();
        insertLine(objQueue[QLatin1String("string")].toString());
    }
    mHistoryStdoutLoaded = true;
    for (const QString &str : std::as_const(mStdoutBeforeLoadingHistory)) {
        slotInsertStdOutInfo(str);
    }
    mStdoutBeforeLoadingHistory.clear();
    mPlainTextEdit->blockSignals(false);
    mPlainTextEdit->verticalScrollBar()->setValue(mPlainTextEdit->verticalScrollBar()->maximum());
}
