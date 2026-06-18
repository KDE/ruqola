/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showbanneduserswidget.h"

#include "attachment/listattachmentdelegate.h"
#include "model/bannedusersmodel.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ShowBannedUsersWidget::ShowBannedUsersWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchBannedUserLineEdit(new QLineEdit(this))
    , mInfo(new QLabel(this))
    , mListBannedUsers(new QListView(this))
    , mModel(new BannedUsersModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto searchBannedUsersLayout = new QHBoxLayout;
    searchBannedUsersLayout->setObjectName(u"searchBannedUsersLayout"_s);
    searchBannedUsersLayout->setContentsMargins({});
    mainLayout->addLayout(searchBannedUsersLayout);

    mSearchBannedUserLineEdit->setObjectName(u"mSearchBannedUserLineEdit"_s);
    mSearchBannedUserLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchBannedUserLineEdit);
    mSearchBannedUserLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search attachments…"));
    connect(mSearchBannedUserLineEdit, &QLineEdit::textChanged, this, &ShowBannedUsersWidget::slotSearchMessageTextChanged);
    searchBannedUsersLayout->addWidget(mSearchBannedUserLineEdit);

    mInfo->setObjectName(u"mInfo"_s);
    mInfo->setTextFormat(Qt::RichText);
    mInfo->setContextMenuPolicy(Qt::NoContextMenu);
    mainLayout->addWidget(mInfo);
    QFont labFont = mInfo->font();
    labFont.setBold(true);
    mInfo->setFont(labFont);
    connect(mInfo, &QLabel::linkActivated, this, &ShowBannedUsersWidget::loadMoreFileAttachment);

    mListBannedUsers->setObjectName(u"mListBannedUsers"_s);
    mainLayout->addWidget(mListBannedUsers);
    auto delegate = new ListAttachmentDelegate(account, this);
    mListBannedUsers->setItemDelegate(delegate);
}

ShowBannedUsersWidget::~ShowBannedUsersWidget() = default;

#if 0
void ShowBannedUsersWidget::setModel(FilesForRoomFilterProxyModel *model)
{
    mModel = model;
    mModel->resetTypeGroup();
    mListBannedUsers->setModel(model);
    connect(mModel, &FilesForRoomFilterProxyModel::hasFullListChanged, this, &ShowBannedUsersWidget::updateLabel);
    connect(mModel, &FilesForRoomFilterProxyModel::totalChanged, this, &ShowBannedUsersWidget::updateLabel);
    connect(mModel, &FilesForRoomFilterProxyModel::loadingInProgressChanged, this, &ShowBannedUsersWidget::updateLabel);
    updateLabel();
}
#endif

void ShowBannedUsersWidget::slotSearchMessageTextChanged(const QString &str)
{
    // TODO     mModel->setFilterString(str);
}

void ShowBannedUsersWidget::updateLabel()
{
    // TODO mInfo->setText(mModel->attachmentCount() == 0 ? i18n("No Banned Users found") : displayShowMessageInRoom());
}

QString ShowBannedUsersWidget::displayShowMessageInRoom() const
{
#if 0
    QString displayMessageStr = i18np("%1 Banned User in room (Total: %2)", "%1 Banned Users in room (Total: %2)", mModel->attachmentCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += u" <a href=\"loadmoreelement\">%1</a>"_s.arg(i18n("(Click here for Loading more…)"));
    }
    return displayMessageStr;
#else
    return {};
#endif
}

#include "moc_showbanneduserswidget.cpp"
