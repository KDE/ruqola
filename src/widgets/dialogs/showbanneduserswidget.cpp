/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showbanneduserswidget.h"

#include "attachment/listattachmentdelegate.h"
#include "model/filesforroomfilterproxymodel.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ShowBannedUsersWidget::ShowBannedUsersWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchAttachmentFileLineEdit(new QLineEdit(this))
    , mInfo(new QLabel(this))
    , mListAttachment(new QListView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto searchAttachmentLayout = new QHBoxLayout;
    searchAttachmentLayout->setObjectName(u"searchAttachmentLayout"_s);
    searchAttachmentLayout->setContentsMargins({});
    mainLayout->addLayout(searchAttachmentLayout);

    mSearchAttachmentFileLineEdit->setObjectName(u"mSearchAttachmentFileLineEdit"_s);
    mSearchAttachmentFileLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchAttachmentFileLineEdit);
    mSearchAttachmentFileLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search attachments…"));
    connect(mSearchAttachmentFileLineEdit, &QLineEdit::textChanged, this, &ShowBannedUsersWidget::slotSearchMessageTextChanged);
    searchAttachmentLayout->addWidget(mSearchAttachmentFileLineEdit);

    mInfo->setObjectName(u"mInfo"_s);
    mInfo->setTextFormat(Qt::RichText);
    mInfo->setContextMenuPolicy(Qt::NoContextMenu);
    mainLayout->addWidget(mInfo);
    QFont labFont = mInfo->font();
    labFont.setBold(true);
    mInfo->setFont(labFont);
    connect(mInfo, &QLabel::linkActivated, this, &ShowBannedUsersWidget::loadMoreFileAttachment);

    mListAttachment->setObjectName(u"mListAttachment"_s);
    mainLayout->addWidget(mListAttachment);
    auto delegate = new ListAttachmentDelegate(account, this);
    mListAttachment->setItemDelegate(delegate);
}

ShowBannedUsersWidget::~ShowBannedUsersWidget()
{
    // Reset it
    if (mModel) {
        mModel->setFilterString(QString());
        mModel->clear();
    }
}

void ShowBannedUsersWidget::setModel(FilesForRoomFilterProxyModel *model)
{
    mModel = model;
    mModel->resetTypeGroup();
    mListAttachment->setModel(model);
    connect(mModel, &FilesForRoomFilterProxyModel::hasFullListChanged, this, &ShowBannedUsersWidget::updateLabel);
    connect(mModel, &FilesForRoomFilterProxyModel::totalChanged, this, &ShowBannedUsersWidget::updateLabel);
    connect(mModel, &FilesForRoomFilterProxyModel::loadingInProgressChanged, this, &ShowBannedUsersWidget::updateLabel);
    updateLabel();
}

void ShowBannedUsersWidget::slotSearchMessageTextChanged(const QString &str)
{
    mModel->setFilterString(str);
}

void ShowBannedUsersWidget::updateLabel()
{
    mInfo->setText(mModel->attachmentCount() == 0 ? i18n("No Banned Users found") : displayShowMessageInRoom());
}

QString ShowBannedUsersWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 Attachment in room (Total: %2)", "%1 Banned Users in room (Total: %2)", mModel->attachmentCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += u" <a href=\"loadmoreelement\">%1</a>"_s.arg(i18n("(Click here for Loading more…)"));
    }
    return displayMessageStr;
}

#include "moc_showbanneduserswidget.cpp"
