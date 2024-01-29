/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "inputtextmanager.h"
#include <KTextEdit>

#include "libruqolawidgets_private_export.h"
#include <QPointer>
class RocketChatAccount;
class CompletionListView;
class UserAndChannelCompletionDelegate;
/**
 * @brief The MessageTextEdit class is the widget used for typing messages to be sent.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageTextEdit : public KTextEdit
{
    Q_OBJECT
public:
    explicit MessageTextEdit(QWidget *parent = nullptr);
    ~MessageTextEdit() override;

    void setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog);

    void insertEmoji(const QString &text);
    [[nodiscard]] QString text() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void changeText(const QString &str, int cursorPosition);

    QMenu *mousePopupMenu() override;

    void setRoomId(const QString &roomId);
    [[nodiscard]] QString roomId() const;

Q_SIGNALS:

    void sendMessage(const QString &str);
    void keyPressed(QKeyEvent *ev);
    void textEditing(bool clearNotification);
    void textClicked();
    void handleMimeData(const QMimeData *mimeData);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void dropEvent(QDropEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCompletionTypeChanged(InputTextManager::CompletionForType type);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCompletionAvailable();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotComplete(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSetAsBold();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSetAsItalic();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSetAsStrikeOut();
    LIBRUQOLAWIDGETS_NO_EXPORT void insertFormat(QChar formatChar);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotInsertCodeBlock();
    LIBRUQOLAWIDGETS_NO_EXPORT void loadSpellCheckingSettings();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLanguageChanged(const QString &lang);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSpellCheckingEnableChanged(bool b);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotInsertMarkdownUrl();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoginChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void switchAutoCorrectionLanguage(const QString &lang);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateMessageFailed(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSelectFirstTextCompleter();

    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    InputTextManager *mCurrentInputTextManager = nullptr;
    CompletionListView *const mUserAndChannelCompletionListView;
    CompletionListView *const mEmojiCompletionListView;
    CompletionListView *const mCommandCompletionListView;
    UserAndChannelCompletionDelegate *mUserAndChannelCompletionDelegate = nullptr;
    QString mRoomId;
};
