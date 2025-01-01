/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundswidget.h"
#include "administratorcustomsoundscreatedialog.h"
#include "connection.h"
#include "custom/customsoundslistjob.h"
#include "ddpapi/ddpclient.h"
#include "misc/methodcalljob.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/admincustomsoundmodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QJsonObject>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>

AdministratorCustomSoundsWidget::AdministratorCustomSoundsWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
{
    mModel = new AdminCustomSoundModel(this);
    mModel->setObjectName(QStringLiteral("mAdminCustomSoundModel"));
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search Custom Sounds"));

    mProxyModelModel = new SearchTreeBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mCustomSoundProxyModel"));
    mTreeView->setModel(mProxyModelModel);
    hideColumns();
    connectModel();
    connect(mTreeView, &QTreeView::doubleClicked, this, &AdministratorCustomSoundsWidget::slotModifyCustomSound);
    connect(mRocketChatAccount, &RocketChatAccount::customSoundRemoved, this, &AdministratorCustomSoundsWidget::slotCustomSoundRemoved);
    connect(mRocketChatAccount, &RocketChatAccount::customSoundAdded, this, &AdministratorCustomSoundsWidget::slotCustomSoundAdded);
    connect(mRocketChatAccount, &RocketChatAccount::customSoundUpdated, this, &AdministratorCustomSoundsWidget::slotCustomSoundUpdated);
}

AdministratorCustomSoundsWidget::~AdministratorCustomSoundsWidget() = default;

void AdministratorCustomSoundsWidget::slotCustomSoundAdded()
{
    // TODO
}

void AdministratorCustomSoundsWidget::slotCustomSoundUpdated()
{
    // TODO
}

void AdministratorCustomSoundsWidget::slotCustomSoundRemoved(const QByteArray &identifier)
{
    mModel->removeElement(identifier);
}

void AdministratorCustomSoundsWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No custom sound found") : displayShowMessage());
}

QString AdministratorCustomSoundsWidget::displayShowMessage() const
{
    QString displayMessageStr = i18np("%1 custom sound (Total: %2)", "%1 custom sounds (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void AdministratorCustomSoundsWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::CustomSoundsListJob(this);
    // https://<url>/api/v1/custom-sounds.list?query={"name":{"$regex":"d","$options":"i"}}&sort={"name":1}&count=25
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
    parameters.setUseSyntaxRc70(mRocketChatAccount->hasAtLeastVersion(7, 0, 0));
    parameters.setSearchString(searchName);
    job->setQueryParameters(parameters);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job, &RocketChatRestApi::CustomSoundsListJob::customSoundsListDone, this, &AdministratorCustomSoundsWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::CustomSoundsListJob::customSoundsListDone, this, &AdministratorCustomSoundsWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start CustomSoundsListJob job";
    }
}

void AdministratorCustomSoundsWidget::slotAddCustomSound()
{
    // TODO use method.call/insertOrUpdateSound
    // Comment for the moment. there is not restapi yet.
    return;
    QPointer<AdministratorCustomSoundsCreateDialog> dlg = new AdministratorCustomSoundsCreateDialog(this);
    if (dlg->exec()) {
        // TODO
    }
    delete dlg;
}

void AdministratorCustomSoundsWidget::slotModifyCustomSound(const QModelIndex &index)
{
    // method.call/uploadCustomSound
    // Comment for the moment. there is not restapi yet.
    return;
    QPointer<AdministratorCustomSoundsCreateDialog> dlg = new AdministratorCustomSoundsCreateDialog(this);
    // TODO add option
    if (dlg->exec()) {
        // TODO
    }
    delete dlg;
}

void AdministratorCustomSoundsWidget::slotRemoveCustomSound(const QModelIndex &index)
{
    if (KMessageBox::warningTwoActions(this,
                                       i18n("Do you want to remove this sound?"),
                                       i18nc("@title", "Remove Custom Sound"),
                                       KStandardGuiItem::remove(),
                                       KStandardGuiItem::cancel())
        == KMessageBox::PrimaryAction) {
        const QModelIndex modelIndex = mModel->index(index.row(), AdminCustomSoundModel::Identifier);
        const QByteArray soundIdentifier = modelIndex.data().toByteArray();
        mRocketChatAccount->ddp()->deleteCustomSound(soundIdentifier);
#if 0
        auto job = new RocketChatRestApi::MethodCallJob(this);
        RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
        info.methodName = QStringLiteral("deleteCustomSound");
        info.anonymous = false;
        QJsonObject obj;
        // TODO
        const QJsonArray params{{QString::fromLatin1(soundIdentifier)}};
        info.messageObj = QJsonArray(soundIdentifier);
        job->setMethodCallJobInfo(std::move(info));
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
        connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [](const QJsonObject &replyObject) {
            qDebug() << " replyObject " << replyObject;
            // TODO update list
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start MethodCallJob deleteCustomSound job";
        }
#endif
        // TODO use method.call/deleteCustomSound
    }
}

void AdministratorCustomSoundsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeView->indexAt(pos);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18nc("@action", "Add…"), this, &AdministratorCustomSoundsWidget::slotAddCustomSound);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18nc("@action", "Modify…"), this, [this, index]() {
            slotModifyCustomSound(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18nc("@action", "Remove"), this, [this, index]() {
            slotRemoveCustomSound(index);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

#include "moc_administratorcustomsoundswidget.cpp"
