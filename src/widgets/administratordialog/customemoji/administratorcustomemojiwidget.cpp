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

#include "administratorcustomemojiwidget.h"
#include "administratorcustomemojicreatedialog.h"
#include "connection.h"
#include "emoji/emojicustomalljob.h"
#include "emoji/emojicustomcreatejob.h"
#include "emoji/emojicustomdeletejob.h"
#include "emoji/emojicustomupdatejob.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/admincustomemojimodel.h"
#include "model/directorybasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
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
    mModel = new AdminCustomEmojiModel(this);
    mModel->setObjectName(QStringLiteral("mModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Custom Sounds"));

    mProxyModelModel = new DirectoryBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mProxyModelModel"));
    mTreeView->setModel(mProxyModelModel);
    hideColumns();
    connectModel();
    connect(mTreeView, &QTreeView::doubleClicked, this, &AdministratorCustomEmojiWidget::slotModifyCustomEmoji);
}

AdministratorCustomEmojiWidget::~AdministratorCustomEmojiWidget()
{
}

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
    map.insert(QStringLiteral("name"), RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
    parameters.setSorting(map);
    if (offset != -1) {
        parameters.setOffset(offset);
    }
    if (count != -1) {
        parameters.setCount(count);
    }
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
    QPointer<AdministratorCustomEmojiCreateDialog> dlg = new AdministratorCustomEmojiCreateDialog(this);
    if (dlg->exec()) {
        const AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info = dlg->info();

        RocketChatRestApi::EmojiCustomCreateJob::EmojiInfo emojiInfo;
        emojiInfo.alias = info.alias;
        emojiInfo.name = info.name;
        emojiInfo.fileNameUrl = info.fileNameUrl;
        auto job = new RocketChatRestApi::EmojiCustomCreateJob(this);
        job->setEmojiInfo(emojiInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::EmojiCustomCreateJob::emojiCustomCreateDone, this, [](const QJsonObject &replyObject) {
            qDebug() << " replyObject " << replyObject;
            // TODO update list
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start EmojiCustomCreateJob job";
        }
    }
    delete dlg;
}

void AdministratorCustomEmojiWidget::slotModifyCustomEmoji(const QModelIndex &index)
{
    QPointer<AdministratorCustomEmojiCreateDialog> dlg = new AdministratorCustomEmojiCreateDialog(this);
    AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info;

    info.alias = mModel->index(index.row(), AdminCustomEmojiModel::Aliases).data().toString();
    info.name = mModel->index(index.row(), AdminCustomEmojiModel::Name).data().toString();
    // TODO info.fileNameUrl =
    dlg->setCustomEmojiInfo(info);
    if (dlg->exec()) {
        const AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info = dlg->info();

        RocketChatRestApi::EmojiCustomUpdateJob::EmojiInfo emojiInfo;
        emojiInfo.alias = info.alias;
        emojiInfo.name = info.name;
        emojiInfo.emojiId = mModel->index(index.row(), AdminCustomEmojiModel::Identifier).data().toString();
        // TODO emojiInfo.fileNameUrl = ???
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
    if (KMessageBox::questionYesNo(this,
                                   i18n("Do you want to remove this emoji?"),
                                   i18nc("@title", "Remove Emoji"),
                                   KStandardGuiItem::remove(),
                                   KStandardGuiItem::cancel())
        == KMessageBox::Yes) {
        auto job = new RocketChatRestApi::EmojiCustomDeleteJob(this);
        const QString emojiId = index.data().toString();
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

void AdministratorCustomEmojiWidget::slotEmojiRemoved(const QString &emojiId)
{
    mModel->removeElement(emojiId);
}

void AdministratorCustomEmojiWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeView->indexAt(pos);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorCustomEmojiWidget::slotAddCustomEmoji);
    if (index.isValid()) {
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminCustomEmojiModel::Identifier);
            slotModifyCustomEmoji(modelIndex);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminCustomEmojiModel::Identifier);
            slotRemoveCustomEmoji(modelIndex);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}
