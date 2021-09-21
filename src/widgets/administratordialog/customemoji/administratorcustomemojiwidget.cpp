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

AdministratorCustomEmojiWidget::AdministratorCustomEmojiWidget(QWidget *parent)
    : SearchTreeBaseWidget(parent)
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
    qDebug() << " MISSING support for search name " << searchName;
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
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
    job->setQueryParameters(parameters);

    rcAccount->restApi()->initializeRestApiJob(job);
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
        // TODO add custom info + update list
    }
    delete dlg;
}

void AdministratorCustomEmojiWidget::slotModifyCustomEmoji(const QModelIndex &index)
{
    QPointer<AdministratorCustomEmojiCreateDialog> dlg = new AdministratorCustomEmojiCreateDialog(this);
    AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info;
    // TODO fill CustomEmojiCreateInfo
    dlg->setCustomEmojiInfo(info);
    if (dlg->exec()) {
        AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info = dlg->info();
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::EmojiCustomCreateJob(this);
        rcAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::EmojiCustomCreateJob::emojiCustomCreateDone, this, [](const QJsonObject &replyObject) {
            // TODO
            // TODO update list
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start EmojiCustomCreateJob job";
        }
    }
    delete dlg;
}

void AdministratorCustomEmojiWidget::slotRemoveCustomEmoji(const QModelIndex &index)
{
    if (KMessageBox::questionYesNo(this, i18n("Do you want to remove this emoji?"), i18nc("@title", "Remove Emoji")) == KMessageBox::Yes) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::EmojiCustomDeleteJob(this);
        const QString emojiId = index.data().toString();
        job->setEmojiId(emojiId);
        rcAccount->restApi()->initializeRestApiJob(job);
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
