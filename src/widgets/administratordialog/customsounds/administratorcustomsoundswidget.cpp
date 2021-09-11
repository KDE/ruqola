/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "administratorcustomsoundswidget.h"
#include "administratorcustomsoundscreatedialog.h"
#include "connection.h"
#include "custom/customsoundslistjob.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/admincustomsoundmodel.h"
#include "model/directorybasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>

AdministratorCustomSoundsWidget::AdministratorCustomSoundsWidget(QWidget *parent)
    : SearchTreeBaseWidget(parent)
{
    mModel = new AdminCustomSoundModel(this);
    mModel->setObjectName(QStringLiteral("mAdminCustomSoundModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Custom Sounds"));

    mProxyModelModel = new DirectoryBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mCustomSoundProxyModel"));
    mTreeView->setModel(mProxyModelModel);
    hideColumns();
    connectModel();
    connect(mTreeView, &QTreeView::doubleClicked, this, &AdministratorCustomSoundsWidget::slotModifyCustomSound);
}

AdministratorCustomSoundsWidget::~AdministratorCustomSoundsWidget()
{
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
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::CustomSoundsListJob(this);
    // https://<url>/api/v1/custom-sounds.list?query={"name":{"$regex":"d","$options":"i"}}&sort={"name":1}&count=25
    //    if (!searchName.isEmpty()) {
    //        job->setPattern(searchName);
    //    }
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
    QPointer<AdministratorCustomSoundsCreateDialog> dlg = new AdministratorCustomSoundsCreateDialog(this);
    if (dlg->exec()) {
        // TODO
    }
    delete dlg;
}

void AdministratorCustomSoundsWidget::slotModifyCustomSound(const QModelIndex &index)
{
    // TODO
}

void AdministratorCustomSoundsWidget::slotRemoveCustomSound(const QModelIndex &index)
{
    // TODO
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
