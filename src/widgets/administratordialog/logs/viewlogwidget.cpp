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
#include "misc/stdoutqueuejob.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>
#include <QJsonArray>
#include <QPlainTextEdit>
#include <QVBoxLayout>

ViewLogWidget::ViewLogWidget(QWidget *parent)
    : QWidget(parent)
    , mPlainTextEdit(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mPlainTextEdit->setObjectName(QStringLiteral("mPlainTextEdit"));
    mPlainTextEdit->setReadOnly(true);
    mainLayout->addWidget(mPlainTextEdit);
}

ViewLogWidget::~ViewLogWidget()
{
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
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::StdoutQueueJob(this);
    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::StdoutQueueJob::stdoutQueueDone, this, &ViewLogWidget::slotStdoutQueueDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start StdoutQueueJob job";
    }
}

void ViewLogWidget::slotStdoutQueueDone(const QJsonObject &obj)
{
    // qDebug() << " obj" << obj;
    QString listLog;
    const QJsonArray array = obj[QLatin1String("queue")].toArray();
    for (int i = 0; i < array.count(); ++i) {
        const QJsonObject objQueue = array.at(i).toObject();
        listLog += objQueue[QLatin1String("string")].toString();
    }
    mPlainTextEdit->setPlainText(listLog);
    // Need to update it.
}

/* RC use this code:

  const foregroundColors = {
        30: 'gray',
        31: 'red',
        32: 'lime',
        33: 'yellow',
        34: '#6B98FF',
        35: '#FF00FF',
        36: 'cyan',
        37: 'white',
};

const ansispan = (str) => {
        str = str
                .replace(/\s/g, '&nbsp;')
                .replace(/(\\n|\n)/g, '<br>')
                .replace(/>/g, '&gt;')
                .replace(/</g, '&lt;')
                .replace(/(.\d{8}-\d\d:\d\d:\d\d\.\d\d\d\(?.{0,2}\)?)/, '<span>$1</span>')
                .replace(/\033\[1m/g, '<strong>')
                .replace(/\033\[22m/g, '</strong>')
                .replace(/\033\[3m/g, '<em>')
                .replace(/\033\[23m/g, '</em>')
                .replace(/\033\[m/g, '</span>')
                .replace(/\033\[0m/g, '</span>')
                .replace(/\033\[39m/g, '</span>');
        return Object.entries(foregroundColors).reduce((str, [ansiCode, color]) => {
                const span = `<span style="color: ${color}">`;
                return str
                        .replace(new RegExp(`\\033\\[${ansiCode}m`, 'g'), span)
                        .replace(new RegExp(`\\033\\[0;${ansiCode}m`, 'g'), span);
        }, str);
};
*/
