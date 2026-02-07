/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
#include "rocketchatbackend.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QJsonObject>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>
#include <qfileinfo.h>

using namespace Qt::Literals::StringLiterals;
AdministratorCustomSoundsWidget::AdministratorCustomSoundsWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
{
    mModel = new AdminCustomSoundModel(this);
    mModel->setObjectName(u"mAdminCustomSoundModel"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search Custom Sounds"));

    mProxyModelModel = new SearchTreeBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(u"mCustomSoundProxyModel"_s);
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
    map.insert(u"name"_s, RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
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
    QPointer<AdministratorCustomSoundsCreateDialog> dlg = new AdministratorCustomSoundsCreateDialog(this);
    if (dlg->exec()) {
        /// api/v1/method.call/uploadCustomSound when we upload new sound file
        ///
#if 0
        NEED TO IMPLEMENT stream-notify-all  QJsonObject({"collection":"stream-notify-all","fields":{"args":[["updateCustomSound",[{"soundData":{"_id":"6985ce1fd5a24b2ff76acf15","extension":"mp3","name":"vvvvvvvvvvv","newFile":true,"random":839}}]]],"eventName":"public-info"},"id":"id","msg":"changed"})
        NEED TO IMPLEMENT stream-notify-all  QJsonObject({"collection":"stream-notify-all","fields":{"args":[["updateCustomSound",[{"soundData":{"_id":"6985ce1fd5a24b2ff76acf15","extension":"mp3","name":"vvvvvvvvvvv","newFile":true,"random":839}}]]],"eventName":"public-info"},"id":"id","msg":"changed"})

#endif

        auto job = new RocketChatRestApi::MethodCallJob(this);
        RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
        info.methodName = u"insertOrUpdateSound"_s;
        info.anonymous = false;
        QJsonArray params;
        QJsonObject obj;
        obj["newFile"_L1] = true;
        const auto customSoundInfo = dlg->customSoundInfo();
        const QFileInfo fileInfo(customSoundInfo.fileNameUrl.toLocalFile());
        fileInfo.completeSuffix();
        const QString customSoundInfoName = customSoundInfo.name;
        const QString customSoundInfoExtension = fileInfo.completeSuffix();

        obj["name"_L1] = customSoundInfoName;
        obj["extension"_L1] = customSoundInfoExtension;
        params.append(obj);
        //{"message":"{\"msg\":\"method\",\"id\":\"34\",\"method\":\"insertOrUpdateSound\",
        // \"params\":[{\"name\":\"vvvvvvvvvvv\",\"extension\":\"mp3\",\"newFile\":true}]}"}
        info.messageObj = mRocketChatAccount->ddp()->generateJsonObject(info.methodName, params);

        job->setMethodCallJobInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
        connect(job,
                &RocketChatRestApi::MethodCallJob::methodCallDone,
                this,
                [this, customSoundInfoExtension, customSoundInfoName, fileInfo](const QJsonObject &root) {
                    // TODO upload file
                    qDebug() << "root " << root;
                    const QString soundIdentifier = root["result"_L1].toString();
                    auto uploadSoundFileJob = new RocketChatRestApi::MethodCallJob(this);
                    RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
                    info.methodName = u"uploadCustomSound"_s;
                    info.anonymous = false;
                    QJsonArray params;
                    QJsonObject obj;
                    obj["name"_L1] = customSoundInfoName;
                    obj["extension"_L1] = customSoundInfoExtension;
                    obj["newFile"_L1] = true;
                    obj["_id"_L1] = soundIdentifier;
                    // obj["random"_L1] = "todo";
                    // TODO convert file as base64 ?

                    // TODO readall file params.append()
                    params.append(obj);
                    info.messageObj = mRocketChatAccount->ddp()->generateJsonObject(info.methodName, params);
                    uploadSoundFileJob->setMethodCallJobInfo(info);
                    qDebug() << " info.messageObj " << info.messageObj;
                    mRocketChatAccount->restApi()->initializeRestApiJob(uploadSoundFileJob);
                    connect(uploadSoundFileJob, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this](const QJsonObject &root) {
                        qDebug() << " RESULT " << root;
                    });
                    if (!uploadSoundFileJob->start()) {
                        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start uploadSoundFileJob job";
                    }
                });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start insertOrUpdateSound job";
        }
    }
    delete dlg;
}

void AdministratorCustomSoundsWidget::slotModifyCustomSound(const QModelIndex &index)
{
    QPointer<AdministratorCustomSoundsCreateDialog> dlg = new AdministratorCustomSoundsCreateDialog(this);
    const QModelIndex nameModelIndex = mModel->index(index.row(), AdminCustomSoundModel::Name);
    const AdministratorCustomSoundsCreateWidget::CustomSoundInfo originalCustomSoundInfo{.name = nameModelIndex.data().toString(), .fileNameUrl = {}};
    dlg->setCustomSoundInfo(originalCustomSoundInfo);
    if (dlg->exec()) {
        const AdministratorCustomSoundsCreateWidget::CustomSoundInfo newCustomInfo = dlg->customSoundInfo();
#if 1
        /// api/v1/method.call/uploadCustomSound when we upload new sound file
        ///

        const QModelIndex modelIndex = mModel->index(index.row(), AdminCustomSoundModel::Identifier);
        const QByteArray soundIdentifier = modelIndex.data().toByteArray();

        //{"message":"{\"msg\":\"method\",\"id\":\"19\",\"method\":\"insertOrUpdateSound\",\"params\"
        // :[{\"_id\":\"6985976bd5a24b2ff76aceed\",\"name\":\"sdfgh\",\"extension\":\"sdf\",
        //\"previousName\":\"sdf\",\"previousExtension\":\"mp3\",
        // \"previousSound\":{\"_id\":\"6985976bd5a24b2ff76aceed\",\"name\":\"sdf\",\"extension\":\"mp3\",
        // \"_updatedAt\":\"2026-02-06T07:25:31.351Z\"},\"newFile\":false}]}"}
        auto job = new RocketChatRestApi::MethodCallJob(this);
        RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
        info.methodName = u"insertOrUpdateSound"_s;
        info.anonymous = false;
        QJsonArray params;
        QJsonObject obj;
        obj["newFile"_L1] = false;
        obj["_id"_L1] = QString::fromLatin1(soundIdentifier);
        obj["name"_L1] = newCustomInfo.name;
        obj["extension"_L1] = newCustomInfo.name; // TODO extension
        QJsonObject previewSound;
        previewSound["_id"_L1] = QString::fromLatin1(soundIdentifier);
        previewSound["name"_L1] = originalCustomSoundInfo.name;
        previewSound["extension"_L1] = originalCustomSoundInfo.name; // TODO extension
        obj["previousSound"_L1] = previewSound;
        params.append(obj);
        // TODO add
        info.messageObj = mRocketChatAccount->ddp()->generateJsonObject(info.methodName, params);
        job->setMethodCallJobInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
        connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this](const QJsonObject &root) {
            // TODO upload file
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start insertOrUpdateSound job";
        }
#endif
    }
    delete dlg;
}

void AdministratorCustomSoundsWidget::slotRemoveCustomSound(const QModelIndex &index)
{
#if 0
    NEED TO IMPLEMENT stream-notify-all  QJsonObject({"collection":"stream-notify-all","fields":{"args":[["deleteCustomSound",[{"soundData":{"_id":"664455f73b610b8aa5d35ba6","_updatedAt":{"$date":1715754487794},"extension":"mp3","name":"test1"}}]]],"eventName":"public-info"},"id":"id","msg":"changed"})
    NEED TO IMPLEMENT stream-notify-all  QJsonObject({"collection":"stream-notify-all","fields":{"args":[["deleteCustomSound",[{"soundData":{"_id":"664455f73b610b8aa5d35ba6","_updatedAt":{"$date":1715754487794},"extension":"mp3","name":"test1"}}]]],"eventName":"public-info"},"id":"id","msg":"changed"})

#endif
    if (KMessageBox::warningTwoActions(this,
                                       i18n("Do you want to remove this sound?"),
                                       i18nc("@title", "Remove Custom Sound"),
                                       KStandardGuiItem::remove(),
                                       KStandardGuiItem::cancel())
        == KMessageBox::PrimaryAction) {
        const QModelIndex modelIndex = mModel->index(index.row(), AdminCustomSoundModel::Identifier);
        const QByteArray soundIdentifier = modelIndex.data().toByteArray();
        auto job = new RocketChatRestApi::MethodCallJob(this);
        RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
        info.methodName = u"deleteCustomSound"_s;
        info.anonymous = false;
        const QJsonArray params{{QString::fromLatin1(soundIdentifier)}};
        info.messageObj = mRocketChatAccount->ddp()->generateJsonObject(info.methodName, params);
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
    }
}

void AdministratorCustomSoundsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeView->indexAt(pos);
    menu.addAction(QIcon::fromTheme(u"list-add"_s), i18nc("@action", "Add…"), this, &AdministratorCustomSoundsWidget::slotAddCustomSound);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(u"document-edit"_s), i18nc("@action", "Modify…"), this, [this, index]() {
            slotModifyCustomSound(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Remove"), this, [this, index]() {
            slotRemoveCustomSound(index);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

#include "moc_administratorcustomsoundswidget.cpp"
