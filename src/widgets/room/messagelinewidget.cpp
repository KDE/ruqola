/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelinewidget.h"
#include "messagemaximumsizedialog/messagemaximumsizedialog.h"
#include "messagetextedit.h"
#include "misc/emoticonmenuwidget.h"
#include "ownuser/ownuserpreferences.h"
#include "rocketchataccount.h"
#include "ruqolaglobalconfig.h"
#include "ruqolaserverconfig.h"
#include "ruqolawidgets_debug.h"
#include <kwidgetsaddons_version.h>

#include <KLocalizedString>
#include <KMessageBox>

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

MessageLineWidget::MessageLineWidget(QWidget *parent)
    : QWidget(parent)
    , mMessageTextEdit(new MessageTextEdit(this))
    , mSendFile(new QToolButton(this))
    , mEmoticonButton(new QToolButton(this))
    , mSendMessageButton(new QToolButton(this))
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

    mSendFile->setAutoRaise(true);
    mSendFile->setObjectName(QStringLiteral("mSendFile"));
    mainLayout->addWidget(mSendFile);
    mSendFile->setIcon(QIcon::fromTheme(QStringLiteral("document-send-symbolic")));
    connect(mSendFile, &QToolButton::clicked, this, &MessageLineWidget::slotSendFile);

    mEmoticonButton->setAutoRaise(true);
    mEmoticonButton->setObjectName(QStringLiteral("mEmoticonButton"));
    mEmoticonButton->setIcon(QIcon::fromTheme(QStringLiteral("smiley-add")));
    mEmoticonButton->setPopupMode(QToolButton::InstantPopup);
    mainLayout->addWidget(mEmoticonButton);

    mSendMessageButton->setAutoRaise(true);
    mSendMessageButton->setObjectName(QStringLiteral("mSendMessageButton"));
    mSendMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("mail-sent")));
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
    connect(emoticonMenu, &QMenu::aboutToShow, mEmoticonMenuWidget, &EmoticonMenuWidget::loadRecentUsed);
    connect(mEmoticonMenuWidget, &EmoticonMenuWidget::insertEmoticons, mMessageTextEdit, &MessageTextEdit::insertEmoji);
    connect(mMessageTextEdit, &MessageTextEdit::handleMimeData, this, &MessageLineWidget::handleMimeData);

    setFocusProxy(mMessageTextEdit);
}

MessageLineWidget::~MessageLineWidget() = default;

void MessageLineWidget::slotSendMessage(const QString &msg)
{
    if (!msg.isEmpty()) {
        if (mMessageIdBeingEdited.isEmpty() && mQuotePermalink.isEmpty()) {
            if (msg.startsWith(QLatin1Char('/'))) {
                // a command ?
                if (mCurrentRocketChatAccount->runCommand(msg, mRoomId, mThreadMessageId)) {
                    setMode(MessageLineWidget::EditingMode::NewMessage);
                    return;
                }
            }
            if (msg.size() > mCurrentRocketChatAccount->messageMaximumAllowedSize()) {
                if (mCurrentRocketChatAccount->messageAllowConvertLongMessagesToAttachment()) {
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
                    if (KMessageBox::ButtonCode::PrimaryAction
                        == KMessageBox::questionTwoActions(this,
#else
                    if (KMessageBox::Yes
                        == KMessageBox::questionYesNo(this,
#endif
                                                           i18n("Do you want to convert this big text as attachment?"),
                                                           i18n("Message Too Big"),
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
                mCurrentRocketChatAccount->sendMessage(mRoomId, msg);
            } else {
                mCurrentRocketChatAccount->replyOnThread(mRoomId, mThreadMessageId, msg);
                if (!mReplyInThreadDialogBox) {
                    setThreadMessageId({});
                }
            }
        } else if (!mMessageIdBeingEdited.isEmpty()) {
            // TODO check message size
            mCurrentRocketChatAccount->updateMessage(mRoomId, mMessageIdBeingEdited, msg);
            clearMessageIdBeingEdited();
        } else if (!mQuotePermalink.isEmpty()) {
            const QString newMessage = QStringLiteral("[ ](%1) %2").arg(mQuotePermalink, msg);
            if (mThreadMessageId.isEmpty()) {
                mCurrentRocketChatAccount->sendMessage(mRoomId, newMessage);
            } else {
                mCurrentRocketChatAccount->replyOnThread(mRoomId, mThreadMessageId, newMessage);
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
    info.roomId = mRoomId;
    info.threadMessageId = mThreadMessageId;
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
    MessageModel *model = messageModel();
    const QModelIndex index = model->indexForMessage(mMessageIdBeingEdited);
    if (index.isValid()) {
        model->setData(index, false, MessageModel::MessageInEditMode);
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
    MessageModel *model = messageModel();
    if (!mMessageIdBeingEdited.isEmpty()) {
        const QModelIndex index = model->indexForMessage(mMessageIdBeingEdited);
        if (index.isValid()) {
            model->setData(index, false, MessageModel::MessageInEditMode);
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
        MessageModel *model = messageModel();
        const QModelIndex index = model->indexForMessage(mMessageIdBeingEdited);
        if (index.isValid()) {
            model->setData(index, true, MessageModel::MessageInEditMode);
        }
    }
    setMode(messageId.isEmpty() ? MessageLineWidget::EditingMode::NewMessage : MessageLineWidget::EditingMode::EditMessage);
    setText(text);
    setFocus();
}

void MessageLineWidget::slotPublicSettingChanged()
{
    mSendFile->setVisible(mCurrentRocketChatAccount->uploadFileEnabled());
}

void MessageLineWidget::slotOwnUserPreferencesChanged()
{
    mEmoticonButton->setVisible(mCurrentRocketChatAccount->ownUserPreferences().useEmojis());
}

void MessageLineWidget::setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog)
{
    mCurrentRocketChatAccount = account;
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

void MessageLineWidget::slotSendFile()
{
    QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
    const qint64 maximumFileSize = mCurrentRocketChatAccount->ruqolaServerConfig()->fileMaxFileSize();
    if (dlg->exec()) {
        const UploadFileDialog::UploadFileInfo result = dlg->fileInfo();
        if (result.fileUrl.isLocalFile()) {
            const QFileInfo info(result.fileUrl.toLocalFile());
            if (info.size() > maximumFileSize) {
                KMessageBox::error(this, i18n("File selected is too big (Maximum size %1)", KIO::convertSize(maximumFileSize)), i18n("File upload"));
                delete dlg;
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
    mCurrentRocketChatAccount->textEditing(mRoomId, clearNotification);
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
    return mRoomId;
}

void MessageLineWidget::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
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
        QTemporaryFile tempFile(QDir::tempPath() + QLatin1String("/XXXXXX.png"));
        if (tempFile.open()) {
            auto image = mimeData->imageData().value<QImage>();
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

MessageModel *MessageLineWidget::messageModel() const
{
    MessageModel *model =
        mThreadMessageId.isEmpty() ? mCurrentRocketChatAccount->messageModelForRoom(mRoomId) : mCurrentRocketChatAccount->threadMessageModel();
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
        MessageModel *model = messageModel();
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
        mCurrentRocketChatAccount->markRoomAsRead(mRoomId);
    }
}
