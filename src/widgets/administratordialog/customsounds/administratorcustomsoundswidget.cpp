/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundswidget.h"
#include "administratorcustomsoundscreatedialog.h"
#include "connection.h"
#include "custom/customsoundslistjob.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/admincustomsoundmodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>
#include <kwidgetsaddons_version.h>

AdministratorCustomSoundsWidget::AdministratorCustomSoundsWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
{
    mModel = new AdminCustomSoundModel(this);
    mModel->setObjectName(QStringLiteral("mAdminCustomSoundModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Custom Sounds"));

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

void AdministratorCustomSoundsWidget::slotCustomSoundRemoved(const QString &identifier)
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
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    if (KMessageBox::warningTwoActions(this,
#else
    if (KMessageBox::warningYesNo(this,
#endif
                                       i18n("Do you want to remove this sound?"),
                                       i18nc("@title", "Remove Custom Sound"),
                                       KStandardGuiItem::remove(),
                                       KStandardGuiItem::cancel())
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
        == KMessageBox::PrimaryAction) {
#else
        == KMessageBox::Yes) {
#endif
        const QModelIndex modelIndex = mModel->index(index.row(), AdminCustomSoundModel::Identifier);
        const QString soundIdentifier = modelIndex.data().toString();
        mRocketChatAccount->ddp()->deleteCustomSound(soundIdentifier);
    }
}

void AdministratorCustomSoundsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeView->indexAt(pos);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorCustomSoundsWidget::slotAddCustomSound);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
            slotModifyCustomSound(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            slotRemoveCustomSound(index);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}
