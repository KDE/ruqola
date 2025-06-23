/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "viewlogwidget.h"

#include "config-ruqola.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "misc/stdoutqueuejob.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "viewlogplaintextedit.h"

#include <QJsonArray>
#include <QScrollBar>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ViewLogWidget::ViewLogWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mPlainTextEdit(new ViewLogPlainTextEdit(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mPlainTextEdit->setObjectName(u"mPlainTextEdit"_s);
    mPlainTextEdit->setReadOnly(true);
    mainLayout->addWidget(mPlainTextEdit);
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
        params.append(QJsonValue(u"stdout"_s));
        mStdoutIdentifier = mRocketChatAccount->ddp()->subscribe(u"stream-stdout"_s, params);
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
#if HAVE_TEXT_CUSTOM_EDITOR
    mPlainTextEdit->editor()->appendHtml(u"<p white-space:pre\">%1</p>"_s.arg(str));
#else
    mPlainTextEdit->appendHtml(u"<p white-space:pre\">%1</p>"_s.arg(str));
#endif
}

void ViewLogWidget::slotStdoutQueueDone(const QJsonObject &obj)
{
    // qDebug() << " obj" << obj;
    const QJsonArray array = obj["queue"_L1].toArray();
    mPlainTextEdit->blockSignals(true);
    for (int i = 0; i < array.count(); ++i) {
        const QJsonObject objQueue = array.at(i).toObject();
        insertLine(objQueue["string"_L1].toString());
    }
    mHistoryStdoutLoaded = true;
    for (const QString &str : std::as_const(mStdoutBeforeLoadingHistory)) {
        slotInsertStdOutInfo(str);
    }
    mStdoutBeforeLoadingHistory.clear();
    mPlainTextEdit->blockSignals(false);
#if HAVE_TEXT_CUSTOM_EDITOR
    mPlainTextEdit->editor()->verticalScrollBar()->setValue(mPlainTextEdit->editor()->verticalScrollBar()->maximum());
#else
    mPlainTextEdit->verticalScrollBar()->setValue(mPlainTextEdit->verticalScrollBar()->maximum());
#endif
}

#include "moc_viewlogwidget.cpp"
