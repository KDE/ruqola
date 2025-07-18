/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomemojiwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorcustomemojicreateorupdatedialog.h"
#include "connection.h"
#include "emoji/emojicustomalljob.h"
#include "emoji/emojicustomcreatejob.h"
#include "emoji/emojicustomdeletejob.h"
#include "emoji/emojicustomupdatejob.h"
#include "emoticons/emojimanager.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/admincustomemojimodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>

AdministratorCustomEmojiWidget::AdministratorCustomEmojiWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
{
    mModel = new AdminCustomEmojiModel(account, this);
    mModel->setObjectName(u"mModel"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search custom emojis"));

    mProxyModelModel = new SearchTreeBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(u"mProxyModelModel"_s);
    mTreeView->setModel(mProxyModelModel);
    hideColumns();
    connectModel();
    connect(mTreeView, &QTreeView::doubleClicked, this, &AdministratorCustomEmojiWidget::slotModifyCustomEmoji);
}

AdministratorCustomEmojiWidget::~AdministratorCustomEmojiWidget() = default;

void AdministratorCustomEmojiWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No custom emoji found") : displayShowMessage());
}

QString AdministratorCustomEmojiWidget::displayShowMessage() const
{
    QString displayMessageStr = i18np("%1 custom emoji (Total: %2)", "%1 custom emojis (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void AdministratorCustomEmojiWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    // https://<server>/api/v1/emoji-custom.all?query={%22name%22:{%22$regex%22:%22t%22,%22$options%22:%22i%22}}&sort={%22name%22:1}&count=25
    // https://<server>/api/v1/emoji-custom.all?query=%7B%22name%22:%22te%22%7D&sort=%7B%22name%22:1%7D
    auto job = new RocketChatRestApi::EmojiCustomAllJob(this);

    RocketChatRestApi::QueryParameters parameters;
    QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
    map.insert(u"name"_s, RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
    parameters.setSorting(map);
    if (offset != -1) {
        parameters.setOffset(offset);
    }
    if (count != -1) {
        parameters.setCount(count);
    }
    // TODO bug in RC 7.0 !
    parameters.setUseSyntaxRc70(mRocketChatAccount->hasAtLeastVersion(7, 0, 0));
    parameters.setSearchString(searchName);

    job->setQueryParameters(parameters);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job, &RocketChatRestApi::EmojiCustomAllJob::emojiCustomAllDone, this, &AdministratorCustomEmojiWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::EmojiCustomAllJob::emojiCustomAllDone, this, &AdministratorCustomEmojiWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start EmojiCustomAllJob job";
    }
}

void AdministratorCustomEmojiWidget::slotAddCustomEmoji()
{
    QPointer<AdministratorCustomEmojiCreateOrUpdateDialog> dlg = new AdministratorCustomEmojiCreateOrUpdateDialog(this);
    if (dlg->exec()) {
        const AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo info = dlg->info();

        RocketChatRestApi::EmojiCustomCreateJob::EmojiInfo emojiInfo;
        emojiInfo.alias = info.alias;
        emojiInfo.name = info.name;
        emojiInfo.fileNameUrl = info.fileNameUrl;
        auto job = new RocketChatRestApi::EmojiCustomCreateJob(this);
        job->setEmojiInfo(emojiInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::EmojiCustomCreateJob::emojiCustomCreateDone, this, [this](const QJsonObject &replyObject) {
            Q_UNUSED(replyObject)
            // qDebug() << " replyObject " << replyObject;
            initialize(); // No info about updating list without reload it
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start EmojiCustomCreateJob job";
        }
    }
    delete dlg;
}

void AdministratorCustomEmojiWidget::slotModifyCustomEmoji(const QModelIndex &index)
{
    QPointer<AdministratorCustomEmojiCreateOrUpdateDialog> dlg = new AdministratorCustomEmojiCreateOrUpdateDialog(this);
    dlg->setType(AdministratorCustomEmojiCreateOrUpdateWidget::AdministratorCustomEmojiCreateOrUpdateType::Update);
    AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo info;

    info.alias = mModel->index(index.row(), AdminCustomEmojiModel::Aliases).data().toString();
    info.name = mModel->index(index.row(), AdminCustomEmojiModel::Name).data().toString();
    const auto icon = mModel->index(index.row(), AdminCustomEmojiModel::Icon).data().value<QIcon>();
    info.icon = icon;
    // TODO info.fileNameUrl =
    dlg->setCustomEmojiInfo(info);
    if (dlg->exec()) {
        const AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo newCustomEmojInfo = dlg->info();

        RocketChatRestApi::EmojiCustomUpdateJob::EmojiInfo emojiInfo;
        emojiInfo.alias = newCustomEmojInfo.alias;
        emojiInfo.name = newCustomEmojInfo.name;
        emojiInfo.emojiId = mModel->index(index.row(), AdminCustomEmojiModel::Identifier).data().toString();
        emojiInfo.fileNameUrl = newCustomEmojInfo.fileNameUrl;
        auto job = new RocketChatRestApi::EmojiCustomUpdateJob(this);
        job->setEmojiInfo(emojiInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::EmojiCustomUpdateJob::emojiCustomUpdateDone, this, [](const QJsonObject &replyObject) {
            qDebug() << " replyObject " << replyObject;
            // TODO update list
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start EmojiCustomUpdateJob job";
        }
    }
    delete dlg;
}

void AdministratorCustomEmojiWidget::slotRemoveCustomEmoji(const QModelIndex &index)
{
    if (KMessageBox::questionTwoActions(this,
                                        i18n("Do you want to remove this emoji?"),
                                        i18nc("@title", "Remove Emoji"),
                                        KStandardGuiItem::remove(),
                                        KStandardGuiItem::cancel())
        == KMessageBox::ButtonCode::PrimaryAction) {
        auto job = new RocketChatRestApi::EmojiCustomDeleteJob(this);
        const QByteArray emojiId = index.data().toByteArray();
        job->setEmojiId(emojiId);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::EmojiCustomDeleteJob::emojiCustomDeleteDone, this, [this, emojiId]() {
            slotEmojiRemoved(emojiId);
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start EmojiCustomDeleteJob job";
        }
    }
}

void AdministratorCustomEmojiWidget::slotEmojiRemoved(const QByteArray &emojiId)
{
    mModel->removeElement(emojiId);
}

void AdministratorCustomEmojiWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeView->indexAt(pos);
    menu.addAction(QIcon::fromTheme(u"list-add"_s), i18nc("@action", "Add…"), this, &AdministratorCustomEmojiWidget::slotAddCustomEmoji);
    if (index.isValid()) {
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);
        menu.addAction(QIcon::fromTheme(u"document-edit"_s), i18nc("@action", "Modify…"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminCustomEmojiModel::Identifier);
            slotModifyCustomEmoji(modelIndex);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Remove"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminCustomEmojiModel::Identifier);
            slotRemoveCustomEmoji(modelIndex);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

#include "moc_administratorcustomemojiwidget.cpp"
