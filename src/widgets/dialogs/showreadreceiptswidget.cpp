/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showreadreceiptswidget.h"

#include "connection.h"
#include "misc/methodcalljob.h"
#include "model/readreceiptsfilterproxymodel.h"
#include "model/readreceiptsmodel.h"
#include "rocketchataccount.h"
#include "rooms/roomsbannedusersjob.h"
#include "ruqolawidgets_debug.h"
#include "showreadreceiptsdelegate.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QJsonArray>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ShowReadReceiptsWidget::ShowReadReceiptsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mCurrentRocketChatAccount(account)
    , mShowReadReceiptsLineEdit(new QLineEdit(this))
    , mListReadReceipts(new QListView(this))
    , mModel(new ReadReceiptsModel(this))
    , mReadReceiptsFilterProxyModel(new ReadReceiptsFilterProxyModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto delegate = new ShowReadReceiptsDelegate(this);
    if (mCurrentRocketChatAccount) {
        delegate->setRocketChatAccount(mCurrentRocketChatAccount);
    }
    mListReadReceipts->setItemDelegate(delegate);

    mShowReadReceiptsLineEdit->setObjectName(u"mShowReadReceiptsLineEdit"_s);
    mShowReadReceiptsLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mShowReadReceiptsLineEdit);
    mShowReadReceiptsLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search Read Receipts…"));
    connect(mShowReadReceiptsLineEdit, &QLineEdit::textChanged, this, &ShowReadReceiptsWidget::slotSearchReadReceiptsChanged);
    mainLayout->addWidget(mShowReadReceiptsLineEdit);

    mListReadReceipts->setObjectName(u"mListReadReceipts"_s);
    mainLayout->addWidget(mListReadReceipts);
    mReadReceiptsFilterProxyModel->setSourceModel(mModel);
    mListReadReceipts->setModel(mReadReceiptsFilterProxyModel);
}

ShowReadReceiptsWidget::~ShowReadReceiptsWidget() = default;

void ShowReadReceiptsWidget::slotSearchReadReceiptsChanged(const QString &str)
{
    mReadReceiptsFilterProxyModel->setFilterString(str);
}

void ShowReadReceiptsWidget::setMessageId(const QByteArray &messageId)
{
    auto job = new RocketChatRestApi::MethodCallJob(this);
    RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
    info.methodName = u"getReadReceipts"_s;
    info.anonymous = false;

    //[{\"name\":\"test1\",\"serverURL\":\"http://www.kde.org\",\"username\":\"A\",\"password\":\"A\"}]}
    QJsonObject obj;
    obj["messageId"_L1] = QString::fromLatin1(messageId);
    const QJsonArray params{obj};
    info.messageObj = mCurrentRocketChatAccount->ddp()->generateJsonObject(info.methodName, params);
    job->setMethodCallJobInfo(info);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [](const QJsonObject &replyObject) {
        qDebug() << " replyObject " << replyObject;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start getReadReceipts job";
    }
}

#include "moc_showreadreceiptswidget.cpp"
