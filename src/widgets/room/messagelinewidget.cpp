/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelinewidget.h"
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#include "dialogs/createsoundmessagewizard.h"
#include "dialogs/createvideomessagewizard.h"
#endif
#include "messagemaximumsizedialog/messagemaximumsizedialog.h"
#include "messagetextedit.h"
#include "misc/emoticonmenuwidget.h"
#include "ownuser/ownuserpreferences.h"
#include "rocketchataccount.h"
#include "ruqolaglobalconfig.h"
#include "ruqolaserverconfig.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QMimeDatabase>

#include <KIO/Global>
#include <QClipboard>
#include <QDir>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QImageWriter>
#include <QMenu>
#include <QMimeData>
#include <QScreen>
#include <QTemporaryFile>
#include <QToolButton>
#include <QWidgetAction>

MessageLineWidget::MessageLineWidget(QWidget *parent)
    : QWidget(parent)
    , mMessageTextEdit(new MessageTextEdit(this))
    , mSendFileButton(new QToolButton(this))
    , mEmoticonButton(new QToolButton(this))
    , mSendMessageButton(new QToolButton(this))
    , mVideoMessageButton(new QToolButton(this))
    , mSoundMessageButton(new QToolButton(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mMessageTextEdit->setObjectName(QStringLiteral("mMessageTextEdit"));
    mainLayout->addWidget(mMessageTextEdit);
    connect(mMessageTextEdit, &MessageTextEdit::sendMessage, this, &MessageLineWidget::slotSendMessage);
    connect(mMessageTextEdit, &MessageTextEdit::keyPressed, this, &MessageLineWidget::keyPressedInLineEdit);
    connect(mMessageTextEdit, &MessageTextEdit::textEditing, this, &MessageLineWidget::slotTextEditing);
    connect(mMessageTextEdit, &MessageTextEdit::textClicked, this, &MessageLineWidget::textEditClicked);

    mSendFileButton->setAutoRaise(true);
    mSendFileButton->setObjectName(QStringLiteral("mSendFileButton"));
    mainLayout->addWidget(mSendFileButton);
#ifndef QT_NO_ACCESSIBILITY
    mSendFileButton->setAccessibleName(i18n("Attach File"));
#endif

    mSendFileButton->setIcon(QIcon::fromTheme(QStringLiteral("document-send-symbolic")));
    connect(mSendFileButton, &QToolButton::clicked, this, &MessageLineWidget::slotSendFile);

    mVideoMessageButton->setAutoRaise(true);
    mVideoMessageButton->setObjectName(QStringLiteral("mVideoMessageButton"));
#ifndef QT_NO_ACCESSIBILITY
    mVideoMessageButton->setAccessibleName(i18n("Video Message"));
#endif

    mainLayout->addWidget(mVideoMessageButton);
    mVideoMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("camera-video")));
    connect(mVideoMessageButton, &QToolButton::clicked, this, &MessageLineWidget::slotSendVideoMessage);

    mSoundMessageButton->setAutoRaise(true);
    mSoundMessageButton->setObjectName(QStringLiteral("mSoundMessageButton"));
#ifndef QT_NO_ACCESSIBILITY
    mSoundMessageButton->setAccessibleName(i18n("Sound Message"));
#endif

    mainLayout->addWidget(mSoundMessageButton);
    mSoundMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("audio-input-microphone")));
    connect(mSoundMessageButton, &QToolButton::clicked, this, &MessageLineWidget::slotSendSoundMessage);

    mEmoticonButton->setAutoRaise(true);
    mEmoticonButton->setObjectName(QStringLiteral("mEmoticonButton"));
    mEmoticonButton->setIcon(QIcon::fromTheme(QStringLiteral("smiley-add")));
    mEmoticonButton->setPopupMode(QToolButton::InstantPopup);
#ifndef QT_NO_ACCESSIBILITY
    mEmoticonButton->setAccessibleName(i18n("Add Emoticon"));
#endif
    mainLayout->addWidget(mEmoticonButton);

    mSendMessageButton->setAutoRaise(true);
    mSendMessageButton->setObjectName(QStringLiteral("mSendMessageButton"));
    mSendMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("mail-sent")));
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    mVideoMessageButton->setVisible(false);
    mSoundMessageButton->setVisible(false);
#endif
}

MessageLineWidget::~MessageLineWidget() = default;

void MessageLineWidget::slotSendMessage(const QString &msg)
{
    if (!msg.isEmpty()) {
        if (mMessageIdBeingEdited.isEmpty() && mQuotePermalink.isEmpty()) {
            if (msg.startsWith(QLatin1Char('/'))) {
                // a command ?
                if (mCurrentRocketChatAccount->runCommand(msg, roomId(), mThreadMessageId)) {
                    setMode(MessageLineWidget::EditingMode::NewMessage);
                    return;
                }
            }
            if (msg.size() > mCurrentRocketChatAccount->messageMaximumAllowedSize()) {
                if (mCurrentRocketChatAccount->messageAllowConvertLongMessagesToAttachment()) {
                    if (KMessageBox::ButtonCode::PrimaryAction
                        == KMessageBox::questionTwoActions(this,
                                                           i18n("Do you want to convert this big text as attachment?"),
                                                           i18nc("@title:window", "Message Too Big"),
                                                           KStandardGuiItem::ok(),
                                                           KStandardGuiItem::cancel())) {
                        QPointer<MessageMaximumSizeDialog> dlg = new MessageMaximumSizeDialog(this);
                        if (dlg->exec()) {
                            QTemporaryFile tempFile(QDir::tempPath() + QStringLiteral("/XXXXXX.txt"));
                            tempFile.setAutoRemove(false);
                            if (tempFile.open()) {
                                QTextStream stream(&tempFile);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                stream.setCodec("UTF-8");
#endif
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
            const QString newMessage = QStringLiteral("[ ](%1) %2").arg(mQuotePermalink, msg);
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

    Q_EMIT createUploadJob(info);
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

QString MessageLineWidget::quotePermalink() const
{
    return mQuotePermalink;
}

void MessageLineWidget::clearMessageIdBeingEdited()
{
    MessagesModel *model = messageModel();
    if (!mMessageIdBeingEdited.isEmpty()) {
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

void MessageLineWidget::setEditMessage(const QString &messageId, const QString &text)
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
    mSendFileButton->setVisible(mCurrentRocketChatAccount->uploadFileEnabled());
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    mSoundMessageButton->setVisible(mCurrentRocketChatAccount->audioRecorderEnabled());
    mVideoMessageButton->setVisible(mCurrentRocketChatAccount->videoRecorderEnabled());
#endif
}

void MessageLineWidget::slotOwnUserPreferencesChanged()
{
    mEmoticonButton->setVisible(mCurrentRocketChatAccount->ownUserPreferences().useEmojis());
}

void MessageLineWidget::slotPrivateSettingsChanged()
{
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    mSoundMessageButton->setVisible(mCurrentRocketChatAccount->audioRecorderEnabled());
    mVideoMessageButton->setVisible(mCurrentRocketChatAccount->videoRecorderEnabled());
#endif
}

void MessageLineWidget::setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::privateSettingsChanged, this, &MessageLineWidget::slotPrivateSettingsChanged);
    }
    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::privateSettingsChanged, this, &MessageLineWidget::slotPrivateSettingsChanged);
    mMessageTextEdit->setCurrentRocketChatAccount(account, threadMessageDialog);
    mEmoticonMenuWidget->setCurrentRocketChatAccount(account);
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
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    QPointer<CreateSoundMessageWizard> dlg = new CreateSoundMessageWizard(this);
    if (dlg->exec()) {
        const CreateSoundMessageWizard::CreateSoundMessageInfo info = dlg->soundMessageInfo();
        UploadFileDialog::UploadFileInfo result;
        result.description = info.mDescription;
        result.fileUrl = info.mFilePath;
        result.fileName = info.mFileName;
        result.deleteTemporaryFile = true;
        sendFile(result);
    }
    delete dlg;
#endif
}

void MessageLineWidget::slotSendVideoMessage()
{
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    QPointer<CreateVideoMessageWizard> dlg = new CreateVideoMessageWizard(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        const CreateVideoMessageWizard::CreateVideoMessageInfo info = dlg->videoMessageInfo();
        if (info.isValid()) {
            UploadFileDialog::UploadFileInfo result;
            result.description = info.mDescription;
            result.fileUrl = info.mFilePath;
            result.deleteTemporaryFile = true;
            result.fileName = info.mFileName;
            sendFile(result);
        }
    }
    delete dlg;
#endif
}

void MessageLineWidget::slotSendFile()
{
    QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
    QStringList whiteList = mCurrentRocketChatAccount->ruqolaServerConfig()->mediaWhiteList();
    const QStringList blackList = mCurrentRocketChatAccount->ruqolaServerConfig()->mediaBlackList();
    for (const auto &mediaType : blackList) {
        if (whiteList.contains(mediaType)) {
            whiteList.removeAll(mediaType);
        }
    }
    // Disable for the moment dlg->setAuthorizedMediaTypes(whiteList);
    // qDebug() << " whiteList " << whiteList << " blackList " << blackList;
    if (dlg->exec()) {
        const UploadFileDialog::UploadFileInfo result = dlg->fileInfo();
        if (result.fileUrl.isLocalFile()) {
            const QFileInfo info(result.fileUrl.toLocalFile());
            const qint64 maximumFileSize = mCurrentRocketChatAccount->ruqolaServerConfig()->fileMaxFileSize();
            if (info.size() > maximumFileSize) {
                KMessageBox::error(this, i18n("File selected is too big (Maximum size %1)", KIO::convertSize(maximumFileSize)), i18n("File upload"));
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

QString MessageLineWidget::threadMessageId() const
{
    return mThreadMessageId;
}

void MessageLineWidget::setThreadMessageId(const QString &threadMessageId, const QString &text, bool replyInDialogBox)
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
            mSendMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("mail-sent")));
            break;
        case EditingMode::EditMessage:
            mSendMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-symbolic")));
            break;
        }
    }
}

void MessageLineWidget::slotTextEditing(bool clearNotification)
{
    mSendMessageButton->setEnabled(!clearNotification);
    mCurrentRocketChatAccount->textEditing(roomId(), clearNotification);
}

QString MessageLineWidget::messageIdBeingEdited() const
{
    return mMessageIdBeingEdited;
}

void MessageLineWidget::setMessageIdBeingEdited(const QString &messageIdBeingEdited)
{
    mMessageIdBeingEdited = messageIdBeingEdited;
}

QString MessageLineWidget::roomId() const
{
    return mMessageTextEdit->roomId();
}

void MessageLineWidget::setRoomId(const QString &roomId)
{
    mMessageTextEdit->setRoomId(roomId);
}

bool MessageLineWidget::handleMimeData(const QMimeData *mimeData)
{
    auto uploadFile = [this](const QUrl &url, const QPixmap &pix) {
        QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
        dlg->setFileUrl(url);
        dlg->setPixmap(pix);
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
                uploadFile(url, QPixmap());
            }
        }
        return true;
    } else if (mimeData->hasImage()) {
        QTemporaryFile tempFile(QDir::tempPath() + QLatin1String("/XXXXXX.png"));
        if (tempFile.open()) {
            const auto image = mimeData->imageData().value<QImage>();
            QImageWriter writer(&tempFile, "PNG");
            if (writer.write(image)) {
                const QUrl url = QUrl::fromLocalFile(tempFile.fileName());
                tempFile.close();
                const QSize pixmapAvatarSize = QSize(120, 120) * screen()->devicePixelRatio();
                uploadFile(url, QPixmap::fromImage(image).scaled(pixmapAvatarSize, Qt::KeepAspectRatio));
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
            return mCurrentRocketChatAccount->isMessageEditable(msg);
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

#include "moc_messagelinewidget.cpp"
