/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelinewidget.h"

#include "connection.h"
#include "dialogs/createsoundmessagewizard.h"
#include "dialogs/createvideomessagewizard.h"
#include "messagemaximumsizedialog/messagemaximumsizedialog.h"
#include "messagetextedit.h"
#include "misc/emoticonmenuwidget.h"
#include "ownuser/ownuserpreferences.h"
#include "rocketchataccount.h"
#include "room/messagelineextratoolbutton.h"
#include "room/plugins/plugintextinterface.h"
#include "room/plugins/plugintool.h"
#include "ruqolaglobalconfig.h"
#include "ruqolaserverconfig.h"
#include "ruqolawidgets_debug.h"
#include "timestampinmessages/timestampinmessagesdialog.h"
#include "toolspluginmanager.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QFileDialog>
#include <QMimeDatabase>

#include <KIO/Global>
#include <QClipboard>
#include <QDir>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QImageWriter>
#include <QMenu>
#include <QMimeData>
#include <QTemporaryFile>
#include <QToolButton>
#include <QWidgetAction>
using namespace Qt::Literals::StringLiterals;

MessageLineWidget::MessageLineWidget(QWidget *parent)
    : QWidget(parent)
    , mMessageTextEdit(new MessageTextEdit(this))
    , mSendFileButton(new QToolButton(this))
    , mVideoMessageButton(new QToolButton(this))
    , mSoundMessageButton(new QToolButton(this))
    , mEmoticonButton(new QToolButton(this))
    , mSendMessageButton(new QToolButton(this))
    , mMessageLineExtraToolButton(new MessageLineExtraToolButton(this))
{
    QList<PluginTool *> plugins = ToolsPluginManager::self()->pluginsList();
    if (plugins.count() > 1) {
        std::sort(plugins.begin(), plugins.end(), [](PluginTool *left, PluginTool *right) {
            return left->order() < right->order();
        });
    }

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mMessageTextEdit->setObjectName(u"mMessageTextEdit"_s);
    mainLayout->addWidget(mMessageTextEdit);
    connect(mMessageTextEdit, &MessageTextEdit::sendMessage, this, &MessageLineWidget::slotSendMessage);
    connect(mMessageTextEdit, &MessageTextEdit::keyPressed, this, &MessageLineWidget::keyPressedInLineEdit);
    connect(mMessageTextEdit, &MessageTextEdit::textEditing, this, &MessageLineWidget::slotTextEditing);
    connect(mMessageTextEdit, &MessageTextEdit::textClicked, this, &MessageLineWidget::textEditClicked);

    mSendFileButton->setAutoRaise(true);
    mSendFileButton->setObjectName(u"mSendFileButton"_s);
    mSendFileButton->setToolTip(i18nc("@info:tooltip", "Attach a file…"));
    mainLayout->addWidget(mSendFileButton);
#ifndef QT_NO_ACCESSIBILITY
    mSendFileButton->setAccessibleName(i18n("Attach File"));
#endif

    mSendFileButton->setIcon(QIcon::fromTheme(u"mail-attachment-symbolic"_s));
    connect(mSendFileButton, &QToolButton::clicked, this, &MessageLineWidget::slotSendFile);

    mVideoMessageButton->setAutoRaise(true);
    mVideoMessageButton->setObjectName(u"mVideoMessageButton"_s);
    mVideoMessageButton->setToolTip(i18nc("@info:tooltip", "Send a video message…"));
#ifndef QT_NO_ACCESSIBILITY
    mVideoMessageButton->setAccessibleName(i18n("Video Message"));
#endif

    mainLayout->addWidget(mVideoMessageButton);
    mVideoMessageButton->setIcon(QIcon::fromTheme(u"camera-video"_s));
    connect(mVideoMessageButton, &QToolButton::clicked, this, &MessageLineWidget::slotSendVideoMessage);

    mSoundMessageButton->setAutoRaise(true);
    mSoundMessageButton->setObjectName(u"mSoundMessageButton"_s);
    mSoundMessageButton->setToolTip(i18nc("@info:tooltip", "Send a sound message…"));
#ifndef QT_NO_ACCESSIBILITY
    mSoundMessageButton->setAccessibleName(i18n("Sound Message"));
#endif

    mainLayout->addWidget(mSoundMessageButton);
    mSoundMessageButton->setIcon(QIcon::fromTheme(u"audio-input-microphone"_s));
    mSoundMessageButton->setToolTip(i18nc("@info:tooltip", "Send a sound message…"));
    connect(mSoundMessageButton, &QToolButton::clicked, this, &MessageLineWidget::slotSendSoundMessage);

    mEmoticonButton->setAutoRaise(true);
    mEmoticonButton->setObjectName(u"mEmoticonButton"_s);
    mEmoticonButton->setIcon(QIcon::fromTheme(u"smiley-add"_s));
    mEmoticonButton->setPopupMode(QToolButton::InstantPopup);
    mEmoticonButton->setToolTip(i18nc("@info:tooltip", "Add emoticon…"));
#ifndef QT_NO_ACCESSIBILITY
    mEmoticonButton->setAccessibleName(i18n("Add Emoticon"));
#endif
    mainLayout->addWidget(mEmoticonButton);

    mSendMessageButton->setAutoRaise(true);
    mSendMessageButton->setObjectName(u"mSendMessageButton"_s);
    mSendMessageButton->setIcon(QIcon::fromTheme(u"mail-sent"_s));
    mSendMessageButton->setToolTip(i18nc("@info:tooltip", "Send message"));
#ifndef QT_NO_ACCESSIBILITY
    mSendMessageButton->setAccessibleName(i18n("Send Message"));
#endif
    mainLayout->addWidget(mSendMessageButton);
    mSendMessageButton->setEnabled(false);
    connect(mSendMessageButton, &QToolButton::clicked, this, [this]() {
        slotSendMessage(mMessageTextEdit->text());
        mMessageTextEdit->clear();
    });

    auto emoticonMenu = new QMenu(this);
    auto action = new QWidgetAction(emoticonMenu);
    mEmoticonMenuWidget = new EmoticonMenuWidget(this);
    action->setDefaultWidget(mEmoticonMenuWidget);
    emoticonMenu->addAction(action);
    mEmoticonButton->setMenu(emoticonMenu);
    connect(emoticonMenu, &QMenu::aboutToShow, mEmoticonMenuWidget, &EmoticonMenuWidget::forceLineEditFocus);
    connect(mEmoticonMenuWidget, &EmoticonMenuWidget::insertEmojiIdentifier, mMessageTextEdit, &MessageTextEdit::insertEmoji);
    connect(mMessageTextEdit, &MessageTextEdit::handleMimeData, this, &MessageLineWidget::handleMimeData);

    setFocusProxy(mMessageTextEdit);

    for (PluginTool *plugin : std::as_const(plugins)) {
        if (plugin->enabled()) {
            if (plugin->toolType() == PluginTool::ToolType::MessageViewToolBar) {
                auto pluginButton = new QToolButton(this);
                pluginButton->setAutoRaise(true);
                const QString desc = plugin->description();
                if (desc.isEmpty()) {
                    pluginButton->setText(desc);
                }
                pluginButton->setIcon(QIcon::fromTheme(plugin->iconName()));
                pluginButton->setToolTip(plugin->toolTip());
                auto interface = plugin->createInterface(this, this);
                mPluginToolInterface.append(interface);
                connect(interface, &PluginToolInterface::activateRequested, this, [this, interface]() {
                    const PluginToolInterface::PluginToolInfo info{
                        .roomId = roomId(),
                        .accountName = mCurrentRocketChatAccount->accountName(),
                        .tmid = mThreadMessageId,
                        .msgId = mMessageIdBeingEdited,
                    };
                    interface->setInfo(info);
                    interface->activateTool();
                });
                if (plugin->hasMenu()) {
                    pluginButton->setMenu(interface->menu());
                    pluginButton->setPopupMode(QToolButton::InstantPopup);
                } else {
                    connect(pluginButton, &QToolButton::clicked, interface, &PluginToolInterface::activateRequested);
                }
                mainLayout->addWidget(pluginButton);
            }
        }
    }
    mMessageLineExtraToolButton->setObjectName(u"mMessageLineExtraToolButton"_s);
    mainLayout->addWidget(mMessageLineExtraToolButton);
    connect(mMessageLineExtraToolButton, &MessageLineExtraToolButton::uiInteractionRequested, this, &MessageLineWidget::uiInteractionRequested);
    connect(mMessageLineExtraToolButton, &MessageLineExtraToolButton::addWebDavServer, this, &MessageLineWidget::addWebDavServer);
    connect(mMessageLineExtraToolButton, &MessageLineExtraToolButton::insertTimeStamp, this, &MessageLineWidget::insertTimeStamp);
}

MessageLineWidget::~MessageLineWidget()
{
    qDeleteAll(mPluginToolInterface);
}

void MessageLineWidget::insertTimeStamp()
{
    QPointer<TimeStampInMessagesDialog> dlg = new TimeStampInMessagesDialog(this);
    if (dlg->exec()) {
        const TimeStampInMessagesUtils::TimeStampInfo timeStampInfo = dlg->timeStampInfo();
        const QString result = TimeStampInMessagesUtils::generateTimeStampStr(timeStampInfo);
        setText(result);
    }
    delete dlg;
}

bool MessageLineWidget::runCommand(const QString &msg, const QByteArray &roomId, const QByteArray &tmid)
{
    const RocketChatRestApi::RunCommandJob::RunCommandInfo info = RocketChatRestApi::RunCommandJob::parseString(msg, roomId, tmid);
    if (info.isValid()) {
        auto job = new RocketChatRestApi::RunCommandJob(this);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        job->setRunCommandInfo(info);
        connect(job, &RocketChatRestApi::RunCommandJob::runCommandFailed, this, &MessageLineWidget::slotRunCommandFailed);
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RunCommandJob job";
        }
        return true;
    }
    return false;
}

void MessageLineWidget::slotRunCommandFailed(const RocketChatRestApi::RunCommandJob::RunCommandInfo &info)
{
    if (KMessageBox::questionTwoActions(this,
                                        i18n("Command was failed. Do you want to copy command in clipboard ?"),
                                        i18nc("@title:window", "Command Error"),
                                        KStandardGuiItem::ok(),
                                        KStandardGuiItem::cancel())
        == KMessageBox::ButtonCode::PrimaryAction) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        const QString msg = u"/%1 %2"_s.arg(info.commandName, info.params);
        clipboard->setText(msg, QClipboard::Clipboard);
        clipboard->setText(msg, QClipboard::Selection);
    }
}

bool MessageLineWidget::hasCommandPreview(const QString &msg, const QByteArray &roomId)
{
    const RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo info = RocketChatRestApi::PreviewsCommandJob::parseString(msg, roomId);
    if (info.isValid()) {
        if (mCurrentRocketChatAccount->commandHasPreview(info.commandName)) {
            Q_EMIT showCommandPreview(info);
            return true;
        }
        return false;
    }
    return false;
}

void MessageLineWidget::setSendPreviewCommandInfo(const RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo &info)
{
    auto job = new RocketChatRestApi::PreviewsCommandJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    job->setPreviewsCommandInfo(info);
    // connect(job, &RocketChatRestApi::PreviewsCommandJob::previewsCommandDone, this, &CommandPreviewWidget::slotParsePreviewCommandItems);
    if (!job->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start PreviewsCommandJob job";
    }
    setMode(MessageLineWidget::EditingMode::NewMessage);
}

void MessageLineWidget::slotSendMessage(const QString &msg)
{
    if (!msg.isEmpty()) {
        if (mMessageIdBeingEdited.isEmpty() && mQuotePermalink.isEmpty()) {
            if (msg.startsWith(u'/')) {
                if (!msg.startsWith("//"_L1) && !msg.startsWith("/*"_L1)) {
                    // a command ?
                    if (hasCommandPreview(msg, roomId())) {
                        return;
                    }

                    if (runCommand(msg, roomId(), mThreadMessageId)) {
                        setMode(MessageLineWidget::EditingMode::NewMessage);
                        return;
                    }
                }
            }
            if (msg.size() > mCurrentRocketChatAccount->ruqolaServerConfig()->messageMaximumAllowedSize()) {
                if (mCurrentRocketChatAccount->ruqolaServerConfig()->messageAllowConvertLongMessagesToAttachment()) {
                    if (KMessageBox::ButtonCode::PrimaryAction
                        == KMessageBox::questionTwoActions(this,
                                                           i18n("Do you want to convert this big text as attachment?"),
                                                           i18nc("@title:window", "Message Too Big"),
                                                           KStandardGuiItem::ok(),
                                                           KStandardGuiItem::cancel())) {
                        QPointer<MessageMaximumSizeDialog> dlg = new MessageMaximumSizeDialog(this);
                        if (dlg->exec()) {
                            QTemporaryFile tempFile(QDir::tempPath() + u"/XXXXXX.txt"_s);
                            tempFile.setAutoRemove(false);
                            if (tempFile.open()) {
                                QTextStream stream(&tempFile);
                                stream << msg;
                                tempFile.close();

                                QFile f(tempFile.fileName());
                                if (!f.rename(dlg->fileName())) {
                                    qCWarning(RUQOLAWIDGETS_LOG) << "Rename file failed" << tempFile.fileName() << " dlg->fileName()" << dlg->fileName();
                                }
                                UploadFileDialog::UploadFileInfo uploadFileInfo;
                                uploadFileInfo.description = dlg->description();
                                uploadFileInfo.fileUrl = QUrl::fromLocalFile(f.fileName());
                                uploadFileInfo.deleteTemporaryFile = true;
                                sendFile(uploadFileInfo);
                            }
                        }
                        delete dlg;
                        // We need to send as file here.
                        return;
                    } else {
                        return;
                    }
                } else {
                    return;
                }
            }
            if (mThreadMessageId.isEmpty()) {
                mCurrentRocketChatAccount->sendMessage(roomId(), msg);
            } else {
                mCurrentRocketChatAccount->replyOnThread(roomId(), mThreadMessageId, msg);
                if (!mReplyInThreadDialogBox) {
                    setThreadMessageId({});
                }
            }
        } else if (!mMessageIdBeingEdited.isEmpty()) {
            // TODO check message size
            mCurrentRocketChatAccount->updateMessage(roomId(), mMessageIdBeingEdited, msg);
            clearMessageIdBeingEdited();
        } else if (!mQuotePermalink.isEmpty()) {
            const QString newMessage = u"[ ](%1) %2"_s.arg(mQuotePermalink, msg);
            if (mThreadMessageId.isEmpty()) {
                mCurrentRocketChatAccount->sendMessage(roomId(), newMessage);
            } else {
                mCurrentRocketChatAccount->replyOnThread(roomId(), mThreadMessageId, newMessage);
                if (!mReplyInThreadDialogBox) {
                    setThreadMessageId({});
                }
            }
            setQuoteMessage({}, {});
            clearMessageIdBeingEdited();
        }
        setMode(MessageLineWidget::EditingMode::NewMessage);
    }
}

void MessageLineWidget::sendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo)
{
    RocketChatRestApi::UploadFileJob::UploadFileInfo info;
    info.description = uploadFileInfo.description;
    info.messageText = QString();
    info.filenameUrl = uploadFileInfo.fileUrl;
    info.roomId = roomId();
    info.threadMessageId = mThreadMessageId;
    info.fileName = uploadFileInfo.fileName;
    info.deleteTemporaryFile = uploadFileInfo.deleteTemporaryFile;
    info.rc80Server = mCurrentRocketChatAccount->hasAtLeastVersion(8, 0, 0);
    if (!mReplyInThreadDialogBox) {
        setThreadMessageId({});
    }
    Q_EMIT createUploadJob(std::move(info));
}

void MessageLineWidget::setQuoteMessage(const QString &permalink, const QString &text)
{
    clearMessageIdBeingEdited();
    mQuotePermalink = permalink;
    mQuoteText = text;
    Q_EMIT quoteMessageChanged(mQuotePermalink, text);
}

void MessageLineWidget::clearEditingMode()
{
    // Remove old mark as editing
    MessagesModel *model = messageModel();
    const QModelIndex index = model->indexForMessage(mMessageIdBeingEdited);
    if (index.isValid()) {
        model->setData(index, false, MessagesModel::MessageInEditMode);
    }
}

QString MessageLineWidget::quoteText() const
{
    return mQuoteText;
}

void MessageLineWidget::setRoomName(const QString &roomName, bool thread)
{
    mMessageTextEdit->setPlaceholderText(thread ? i18n("Message %1 in thread", roomName) : i18n("Send a message in %1…", roomName));
}

QString MessageLineWidget::quotePermalink() const
{
    return mQuotePermalink;
}

void MessageLineWidget::clearMessageIdBeingEdited()
{
    if (!mMessageIdBeingEdited.isEmpty()) {
        MessagesModel *model = messageModel();
        const QModelIndex index = model->indexForMessage(mMessageIdBeingEdited);
        if (index.isValid()) {
            model->setData(index, false, MessagesModel::MessageInEditMode);
        }
        mMessageIdBeingEdited.clear();
    }
    mQuotePermalink.clear();
    mQuoteText.clear();
    setText(QString());
    setMode(MessageLineWidget::EditingMode::NewMessage);
}

void MessageLineWidget::setEditMessage(const QByteArray &messageId, const QString &text)
{
    // Remove old mark as editing
    clearEditingMode();
    mMessageIdBeingEdited = messageId;
    if (!mMessageIdBeingEdited.isEmpty()) {
        MessagesModel *model = messageModel();
        const QModelIndex index = model->indexForMessage(mMessageIdBeingEdited);
        if (index.isValid()) {
            model->setData(index, true, MessagesModel::MessageInEditMode);
        }
    }
    setMode(messageId.isEmpty() ? MessageLineWidget::EditingMode::NewMessage : MessageLineWidget::EditingMode::EditMessage);
    setText(text);
    setFocus();
}

void MessageLineWidget::slotPublicSettingChanged()
{
    mSendFileButton->setVisible(mCurrentRocketChatAccount->ruqolaServerConfig()->uploadFileEnabled());
    slotPrivateSettingsChanged();
}

void MessageLineWidget::slotOwnUserPreferencesChanged()
{
    mEmoticonButton->setVisible(mCurrentRocketChatAccount->ownUserPreferences().useEmojis());
}

void MessageLineWidget::slotPrivateSettingsChanged()
{
    mSoundMessageButton->setVisible(mCurrentRocketChatAccount->ruqolaServerConfig()->audioRecorderEnabled());
    mVideoMessageButton->setVisible(mCurrentRocketChatAccount->ruqolaServerConfig()->videoRecorderEnabled());
}

void MessageLineWidget::setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::privateSettingsChanged, this, &MessageLineWidget::slotPrivateSettingsChanged);
#if ADD_OFFLINE_SUPPORT
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::offlineModeChanged, this, &MessageLineWidget::slotOfflineModeChanged);
#endif
    }
    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::privateSettingsChanged, this, &MessageLineWidget::slotPrivateSettingsChanged);
#if ADD_OFFLINE_SUPPORT
    connect(mCurrentRocketChatAccount, &RocketChatAccount::offlineModeChanged, this, &MessageLineWidget::slotOfflineModeChanged);
    slotOfflineModeChanged();
#endif

    mMessageTextEdit->setCurrentRocketChatAccount(account, threadMessageDialog);
    mEmoticonMenuWidget->setCurrentRocketChatAccount(account);
    mMessageLineExtraToolButton->setCurrentRocketChatAccount(account);
}

void MessageLineWidget::setText(const QString &text)
{
    mMessageTextEdit->changeText(text, text.length());
}

QString MessageLineWidget::text() const
{
    return mMessageTextEdit->text();
}

MessageTextEdit *MessageLineWidget::messageTextEdit() const
{
    return mMessageTextEdit;
}

void MessageLineWidget::slotSendSoundMessage()
{
    QPointer<CreateSoundMessageWizard> dlg = new CreateSoundMessageWizard(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        const CreateSoundMessageWizard::CreateSoundMessageInfo info = dlg->soundMessageInfo();
        UploadFileDialog::UploadFileInfo result;
        result.description = info.mDescription;
        result.fileUrl = info.mFileUrl;
        result.fileName = info.mFileName;
        result.deleteTemporaryFile = true;
        sendFile(result);
    }
    delete dlg;
}

void MessageLineWidget::slotSendVideoMessage()
{
    QPointer<CreateVideoMessageWizard> dlg = new CreateVideoMessageWizard(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        const CreateVideoMessageWizard::CreateVideoMessageInfo info = dlg->videoMessageInfo();
        if (info.isValid()) {
            UploadFileDialog::UploadFileInfo result;
            result.description = info.mDescription;
            result.fileUrl = info.mFileUrl;
            result.deleteTemporaryFile = true;
            result.fileName = info.mFileName;
            sendFile(result);
        }
    }
    delete dlg;
}

void MessageLineWidget::slotSendFile()
{
    const QList<QUrl> urls = QFileDialog::getOpenFileUrls(this, i18nc("@title:window", "Upload File"));
    if (urls.isEmpty()) {
        return;
    }
    QStringList whiteList = mCurrentRocketChatAccount->ruqolaServerConfig()->mediaWhiteList();
    const QStringList blackList = mCurrentRocketChatAccount->ruqolaServerConfig()->mediaBlackList();
    for (const auto &mediaType : blackList) {
        if (whiteList.contains(mediaType)) {
            whiteList.removeAll(mediaType);
        }
    }
    for (const auto &url : urls) {
        QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
        dlg->setFileUrl(url);
        // Disable for the moment dlg->setAuthorizedMediaTypes(whiteList);
        // qDebug() << " whiteList " << whiteList << " blackList " << blackList;
        if (dlg->exec()) {
            const UploadFileDialog::UploadFileInfo result = dlg->fileInfo();
            if (result.fileUrl.isLocalFile()) {
                const QFileInfo info(result.fileUrl.toLocalFile());
                const qint64 maximumFileSize = mCurrentRocketChatAccount->ruqolaServerConfig()->fileMaxFileSize();
                if (info.size() > maximumFileSize) {
                    KMessageBox::error(this,
                                       i18n("File selected is too big (Maximum size %1)", KIO::convertSize(maximumFileSize)),
                                       i18nc("@title:window", "File upload"));
                    delete dlg;
                    return;
                }
                auto invalidMedia = [this, dlg]() {
                    KMessageBox::error(this, i18n("Server doesn't authorized this file (invalid mimetype)"));
                    delete dlg;
                };

                QMimeDatabase mimeDatabase;
                const QString mimeTypeName = mimeDatabase.mimeTypeForFile(result.fileUrl.toLocalFile()).name();
#if 0 // Disable for the moment "image/*" is not a valid MIME type for example
                qDebug() << " mimeTypeName" << mimeTypeName << " whiteList " << whiteList;
                if (!whiteList.isEmpty()) {
                    if (!whiteList.contains(mimeTypeName)) {
                        invalidMedia();
                        return;
                    }
                }
#endif
                if (blackList.contains(mimeTypeName)) {
                    invalidMedia();
                    return;
                }
            }

            sendFile(result);
        }
        delete dlg;
    }
}

QByteArray MessageLineWidget::threadMessageId() const
{
    return mThreadMessageId;
}

void MessageLineWidget::setThreadMessageId(const QByteArray &threadMessageId, const QString &text, bool replyInDialogBox)
{
    mReplyInThreadDialogBox = replyInDialogBox;

    if (mThreadMessageId == threadMessageId) {
        return;
    }

    mThreadMessageId = threadMessageId;
    Q_EMIT threadMessageIdChanged(mThreadMessageId, text);
}

MessageLineWidget::EditingMode MessageLineWidget::mode() const
{
    return mMode;
}

void MessageLineWidget::setMode(EditingMode mode)
{
    if (mMode != mode) {
        mMode = mode;
        switch (mMode) {
        case EditingMode::NewMessage:
            mSendMessageButton->setIcon(QIcon::fromTheme(u"mail-sent"_s));
            break;
        case EditingMode::EditMessage:
            mSendMessageButton->setIcon(QIcon::fromTheme(u"edit-symbolic"_s));
            break;
        }
    }
}

void MessageLineWidget::slotTextEditing(bool clearNotification)
{
    mSendMessageButton->setEnabled(!clearNotification);
    mCurrentRocketChatAccount->textEditing(roomId(), clearNotification);
}

QByteArray MessageLineWidget::messageIdBeingEdited() const
{
    return mMessageIdBeingEdited;
}

void MessageLineWidget::setMessageIdBeingEdited(const QByteArray &messageIdBeingEdited)
{
    mMessageIdBeingEdited = messageIdBeingEdited;
}

QByteArray MessageLineWidget::roomId() const
{
    return mMessageTextEdit->roomId();
}

void MessageLineWidget::setRoomId(const QByteArray &roomId)
{
    mMessageTextEdit->setRoomId(roomId);
    mMessageLineExtraToolButton->setRoomId(roomId);
}

bool MessageLineWidget::handleMimeData(const QMimeData *mimeData)
{
    auto uploadFile = [this](const QUrl &url) {
        QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
        dlg->setFileUrl(url);
        if (dlg->exec()) {
            const UploadFileDialog::UploadFileInfo uploadFileInfo = dlg->fileInfo();
            sendFile(uploadFileInfo);
        }
        delete dlg;
    };
    if (mimeData->hasUrls()) {
        const QList<QUrl> urls = mimeData->urls();
        for (const QUrl &url : urls) {
            if (url.isLocalFile()) {
                uploadFile(url);
            }
        }
        return true;
    } else if (mimeData->hasImage()) {
        QTemporaryFile tempFile(QDir::tempPath() + "/XXXXXX.png"_L1);
        if (tempFile.open()) {
            const auto image = mimeData->imageData().value<QImage>();
            QImageWriter writer(&tempFile, "PNG");
            if (writer.write(image)) {
                const QUrl url = QUrl::fromLocalFile(tempFile.fileName());
                tempFile.close();
                uploadFile(url);
                return true;
            }
        }
    }
    return false;
}

MessagesModel *MessageLineWidget::messageModel() const
{
    MessagesModel *model =
        mThreadMessageId.isEmpty() ? mCurrentRocketChatAccount->messageModelForRoom(roomId()) : mCurrentRocketChatAccount->threadMessageModel();
    Q_ASSERT(model);
    return model;
}

void MessageLineWidget::keyPressedInLineEdit(QKeyEvent *ev)
{
    const int key = ev->key();
    if (key == Qt::Key_Escape) {
        if (!mMessageIdBeingEdited.isEmpty() || !mQuotePermalink.isEmpty()) {
            clearMessageIdBeingEdited();
            ev->accept();
        } else {
            Q_EMIT keyPressed(ev);
        }
    } else if (ev->matches(QKeySequence::Paste)) {
        const QMimeData *mimeData = qApp->clipboard()->mimeData();
        if (handleMimeData(mimeData)) {
            ev->accept();
        }
    } else if ((key == Qt::Key_Up || key == Qt::Key_Down) && ev->modifiers() & Qt::AltModifier) {
        MessagesModel *model = messageModel();
        auto isEditable = [this](const Message &msg) {
            // Don't allow to edit no own message with Up+ALT (we still can edit with mouse)
            return (msg.userId() == mCurrentRocketChatAccount->userId()) && mCurrentRocketChatAccount->isMessageEditable(msg);
        };
        if (key == Qt::Key_Up) {
            const Message &msg = model->findLastMessageBefore(mMessageIdBeingEdited, isEditable);
            setEditMessage(msg.messageId(), msg.originalMessageOrAttachmentDescription());
        } else {
            const Message &msg = model->findNextMessageAfter(mMessageIdBeingEdited, isEditable);
            setEditMessage(msg.messageId(), msg.originalMessageOrAttachmentDescription());
        }
        ev->accept();
    } else {
        Q_EMIT keyPressed(ev);
    }
}

void MessageLineWidget::textEditClicked()
{
    if (RuqolaGlobalConfig::self()->markAsReadOnTextClicked()) {
        mCurrentRocketChatAccount->markRoomAsRead(roomId());
    }
}

void MessageLineWidget::slotOfflineModeChanged()
{
#if ADD_OFFLINE_SUPPORT
    setEnabled(!mCurrentRocketChatAccount->offlineMode());
#endif
}

#include "moc_messagelinewidget.cpp"
