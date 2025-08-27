/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetextedit.h"
#include "config-ruqola.h"

#include "common/commandcompletiondelegate.h"
#include "common/completionlistview.h"
#include "common/emojicompletiondelegate.h"
#include "common/userandchannelcompletiondelegate.h"
#include "model/inputcompletermodel.h"
#if HAVE_TEXTADDONSWIDGET_RICHTEXTQUICKTEXTFORMAT
#include <TextAddonsWidgets/RichTextQuickTextFormat>
#else
#include "quicktextformatmessage.h"
#endif
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KLocalizedString>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QMenu>
#include <QMimeData>
#include <QTextCursor>
#include <QTextDocument>

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
#include <TextAutoCorrectionCore/AutoCorrection>
#endif
using namespace Qt::Literals::StringLiterals;

MessageTextEdit::MessageTextEdit(QWidget *parent)
    : KTextEdit(parent)
    , mUserAndChannelCompletionListView(new CompletionListView)
    , mEmojiCompletionListView(new CompletionListView)
    , mCommandCompletionListView(new CompletionListView)
{
    setTabChangesFocus(true);
    setAcceptRichText(false);
    enableFindReplace(false); // not needed here, let's instead make sure the Ctrl+F shortcut will search through channel history

    connect(document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged, this, &QWidget::updateGeometry);

    mUserAndChannelCompletionDelegate = new UserAndChannelCompletionDelegate(mUserAndChannelCompletionListView);
    mUserAndChannelCompletionListView->setItemDelegate(mUserAndChannelCompletionDelegate);
    mUserAndChannelCompletionListView->setTextWidget(this);
    connect(mUserAndChannelCompletionListView, &CompletionListView::complete, this, &MessageTextEdit::slotComplete);

    mEmojiCompletionListView->setItemDelegate(new EmojiCompletionDelegate(mEmojiCompletionListView));
    mEmojiCompletionListView->setTextWidget(this);
    connect(mEmojiCompletionListView, &CompletionListView::complete, this, &MessageTextEdit::slotComplete);

    mCommandCompletionListView->setItemDelegate(new CommandCompletionDelegate(mCommandCompletionListView));
    mCommandCompletionListView->setTextWidget(this);
    connect(mCommandCompletionListView, &CompletionListView::complete, this, &MessageTextEdit::slotComplete);
    loadSpellCheckingSettings();
    connect(this, &MessageTextEdit::languageChanged, this, &MessageTextEdit::slotLanguageChanged);
    connect(this, &MessageTextEdit::checkSpellingChanged, this, &MessageTextEdit::slotSpellCheckingEnableChanged);

#if HAVE_TEXTADDONSWIDGET_RICHTEXTQUICKTEXTFORMAT
    auto quicktextformatmessage = new TextAddonsWidgets::RichTextQuickTextFormat(this, this);
    connect(quicktextformatmessage,
            &TextAddonsWidgets::RichTextQuickTextFormat::quickTextFormatRequested,
            this,
            [this](TextAddonsWidgets::RichTextQuickTextFormat::QuickTextFormatType type) {
                switch (type) {
                case TextAddonsWidgets::RichTextQuickTextFormat::QuickTextFormatType::Bold: {
                    slotSetAsBold();
                    break;
                }
                case TextAddonsWidgets::RichTextQuickTextFormat::QuickTextFormatType::Italic: {
                    slotSetAsItalic();
                    break;
                }
                case TextAddonsWidgets::RichTextQuickTextFormat::QuickTextFormatType::StrikeThrough: {
                    slotSetAsStrikeOut();
                    break;
                }
                case TextAddonsWidgets::RichTextQuickTextFormat::QuickTextFormatType::CodeBlock:
                    slotInsertCodeBlock();
                    break;
                case TextAddonsWidgets::RichTextQuickTextFormat::QuickTextFormatType::BlockQuote:
                    slotInsertBlockQuote();
                    break;
                case TextAddonsWidgets::RichTextQuickTextFormat::QuickTextFormatType::InsertLink:
                    slotInsertMarkdownUrl();
                    break;
                }
            });
#else
    auto quicktextformatmessage = new QuickTextFormatMessage(this, this);
    connect(quicktextformatmessage, &QuickTextFormatMessage::quickTextFormatRequested, this, [this](QuickTextFormatMessage::QuickTextFormatType type) {
        switch (type) {
        case QuickTextFormatMessage::QuickTextFormatType::Bold: {
            slotSetAsBold();
            break;
        }
        case QuickTextFormatMessage::QuickTextFormatType::Italic: {
            slotSetAsItalic();
            break;
        }
        case QuickTextFormatMessage::QuickTextFormatType::StrikeThrough: {
            slotSetAsStrikeOut();
            break;
        }
        case QuickTextFormatMessage::QuickTextFormatType::CodeBlock:
            slotInsertCodeBlock();
            break;
        case QuickTextFormatMessage::QuickTextFormatType::BlockQuote:
            slotInsertBlockQuote();
            break;
        case QuickTextFormatMessage::QuickTextFormatType::InsertLink:
            slotInsertMarkdownUrl();
            break;
        }
    });
#endif
}

MessageTextEdit::~MessageTextEdit()
{
    delete mUserAndChannelCompletionListView;
    delete mEmojiCompletionListView;
    delete mCommandCompletionListView;
}

void MessageTextEdit::slotSpellCheckingEnableChanged(bool b)
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group(config, u"Spelling"_s);
    group.writeEntry("checkerEnabledByDefault", b);
}

void MessageTextEdit::slotLanguageChanged(const QString &lang)
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group(config, u"Spelling"_s);
    group.writeEntry("Language", lang);
    switchAutoCorrectionLanguage(lang);
}

void MessageTextEdit::switchAutoCorrectionLanguage(const QString &lang)
{
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    if (!lang.isEmpty()) {
        auto settings = Ruqola::self()->autoCorrection()->autoCorrectionSettings();
        settings->setLanguage(lang);
        Ruqola::self()->autoCorrection()->setAutoCorrectionSettings(settings);
    }
    qDebug() << " MessageTextEdit::switchAutoCorrectionLanguage " << lang;
#endif
}

void MessageTextEdit::loadSpellCheckingSettings()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    if (config->hasGroup("Spelling"_L1)) {
        KConfigGroup group(config, u"Spelling"_s);
        setCheckSpellingEnabled(group.readEntry("checkerEnabledByDefault", false));
        const QString language = group.readEntry("Language", QString());
        setSpellCheckingLanguage(language);
        switchAutoCorrectionLanguage(language);
    }
}

void MessageTextEdit::setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog)
{
    if (mCurrentInputTextManager) {
        disconnect(mCurrentInputTextManager, &InputTextManager::completionTypeChanged, this, &MessageTextEdit::slotCompletionTypeChanged);
        disconnect(mCurrentInputTextManager, &InputTextManager::selectFirstTextCompleter, this, &MessageTextEdit::slotSelectFirstTextCompleter);
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &MessageTextEdit::slotLoginChanged);
    }
    mCurrentRocketChatAccount = account;
    mCurrentInputTextManager = threadMessageDialog ? mCurrentRocketChatAccount->inputThreadMessageTextManager() : mCurrentRocketChatAccount->inputTextManager();
    mUserAndChannelCompletionListView->setModel(mCurrentInputTextManager->inputCompleterModel());
    mEmojiCompletionListView->setModel(mCurrentInputTextManager->emojiCompleterModel());
    mCommandCompletionListView->setModel(mCurrentInputTextManager->commandModel());
    mUserAndChannelCompletionDelegate->setRocketChatAccount(mCurrentRocketChatAccount);
    connect(mCurrentInputTextManager, &InputTextManager::completionTypeChanged, this, &MessageTextEdit::slotCompletionTypeChanged);
    connect(mCurrentInputTextManager, &InputTextManager::selectFirstTextCompleter, this, &MessageTextEdit::slotSelectFirstTextCompleter);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &MessageTextEdit::slotLoginChanged);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::updateMessageFailed, this, &MessageTextEdit::slotUpdateMessageFailed);
}

void MessageTextEdit::slotUpdateMessageFailed(const QString &str)
{
    // Message Update failed => don't lose text!
    setText(str);
}

void MessageTextEdit::slotLoginChanged()
{
    const auto loginStatus = mCurrentRocketChatAccount->loginStatus();
    if (loginStatus != AuthenticationManager::LoggedIn) {
        mUserAndChannelCompletionListView->hide();
        mEmojiCompletionListView->hide();
        mCommandCompletionListView->hide();
    }
}

void MessageTextEdit::insertEmoji(const QString &text)
{
    textCursor().insertText(text + u' ');
    Q_EMIT textEditing(false);
}

QString MessageTextEdit::text() const
{
    return toPlainText();
}

QSize MessageTextEdit::sizeHint() const
{
    // The width of the QTextDocument is the current widget width, so this is somewhat circular logic.
    // But I don't really want to redo the layout with a different width like idealWidth(), seems slow.
    const QSize docSize = document()->size().toSize();
    const int margin = int(document()->documentMargin());
    return {docSize.width() + margin, qMin(300, docSize.height()) + margin};
}

QSize MessageTextEdit::minimumSizeHint() const
{
    const int margin = int(document()->documentMargin());
    return {300, fontMetrics().height() + margin};
}

void MessageTextEdit::changeText(const QString &newText, int cursorPosition)
{
    setPlainText(newText);

    QTextCursor cursor(document());
    cursor.setPosition(cursorPosition);
    setTextCursor(cursor);

    mCurrentInputTextManager->setInputTextChanged(roomId(), text(), cursorPosition);
}

QMenu *MessageTextEdit::mousePopupMenu()
{
    QMenu *menu = KTextEdit::mousePopupMenu();

    QClipboard *clip = QApplication::clipboard();
    const QMimeData *mimeData = clip->mimeData();
    if (mimeData->hasImage()) {
        menu->addSeparator();
        menu->addAction(i18n("Paste Image"), this, [this, mimeData]() {
            Q_EMIT handleMimeData(mimeData);
        });
    }
    menu->addSeparator();

    auto formatMenu = new QMenu(menu);
    formatMenu->setTitle(i18n("Change Text Format"));
    menu->addMenu(formatMenu);
    formatMenu->addAction(QIcon::fromTheme(u"format-text-bold"_s), i18n("Bold"), this, &MessageTextEdit::slotSetAsBold);
    formatMenu->addAction(QIcon::fromTheme(u"format-text-italic"_s), i18n("Italic"), this, &MessageTextEdit::slotSetAsItalic);
    formatMenu->addAction(QIcon::fromTheme(u"format-text-strikethrough"_s), i18n("Strike-out"), this, &MessageTextEdit::slotSetAsStrikeOut);
    formatMenu->addSeparator();
    formatMenu->addAction(i18n("Code Block"), this, &MessageTextEdit::slotInsertCodeBlock);
    formatMenu->addSeparator();
    formatMenu->addAction(i18n("Markdown Url"), this, &MessageTextEdit::slotInsertMarkdownUrl);
    return menu;
}

void MessageTextEdit::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QByteArray MessageTextEdit::roomId() const
{
    return mRoomId;
}

void MessageTextEdit::slotInsertMarkdownUrl()
{
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection()) {
        const QString mardownUrlStr{u"[text](%1)"_s.arg(cursor.selectedText())};
        cursor.insertText(mardownUrlStr);
        cursor.setPosition(cursor.position() - mardownUrlStr.length() + 1);
    } else {
        const QString mardownUrlStr{u"[text](url)"_s};
        cursor.insertText(mardownUrlStr);
        cursor.setPosition(cursor.position() - mardownUrlStr.length() + 1);
    }
    setTextCursor(cursor);
}

void MessageTextEdit::slotInsertBlockQuote()
{
    const QString textQuoteBlock{u'`'};
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection()) {
        const QString newText = textQuoteBlock + cursor.selectedText() + textQuoteBlock;
        cursor.insertText(newText);
    } else {
        cursor.insertText(QString(textQuoteBlock + u" "_s + textQuoteBlock));
    }
    cursor.setPosition(cursor.position());
    setTextCursor(cursor);
}

void MessageTextEdit::slotInsertCodeBlock()
{
    const QString textCodeBlock{u"```"_s};
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection()) {
        const QString newText = textCodeBlock + u'\n' + cursor.selectedText() + u'\n' + textCodeBlock;
        cursor.insertText(newText);
    } else {
        cursor.insertText(QString(textCodeBlock + u"\n\n"_s + textCodeBlock));
    }
    cursor.setPosition(cursor.position() - 4);
    setTextCursor(cursor);
}

void MessageTextEdit::slotSetAsStrikeOut()
{
    insertFormat(u'~');
}

void MessageTextEdit::slotSetAsBold()
{
    insertFormat(u'*');
}

void MessageTextEdit::slotSetAsItalic()
{
    insertFormat(u'_');
}

void MessageTextEdit::insertFormat(QChar formatChar)
{
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection()) {
        const QString newText = formatChar + cursor.selectedText() + formatChar;
        cursor.insertText(newText);
    } else {
        cursor.insertText(QString(formatChar) + QString(formatChar));
    }
    cursor.setPosition(cursor.position());
    setTextCursor(cursor);
}

void MessageTextEdit::keyPressEvent(QKeyEvent *e)
{
    const int key = e->key();

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    if (Ruqola::self()->autoCorrection()->autoCorrectionSettings()->isEnabledAutoCorrection()) {
        if ((key == Qt::Key_Space) || (key == Qt::Key_Enter) || (key == Qt::Key_Return)) {
            if (!textCursor().hasSelection()) {
                int position = textCursor().position();
                // no Html format in subject. => false
                const bool addSpace = Ruqola::self()->autoCorrection()->autocorrect(false, *document(), position);
                QTextCursor cur = textCursor();
                cur.setPosition(position);
                if (key == Qt::Key_Space) {
                    if (addSpace) {
                        cur.insertText(u" "_s);
                        setTextCursor(cur);
                    }
                    return;
                }
            }
        }
    }
#endif

    if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        if ((key == Qt::Key_Enter && (e->modifiers() == Qt::KeypadModifier)) || !e->modifiers()) {
            Q_EMIT sendMessage(text());
            // We send text => we will clear => we will send textEditing is empty => clear notification
            Q_EMIT textEditing(true);
            clear();
        } else {
            textCursor().insertBlock();
            ensureCursorVisible();
        }
        e->accept();
        return;
    } else if (key == Qt::Key_Up || key == Qt::Key_Down) {
        if (!(e->modifiers() & Qt::AltModifier)) {
            // document()->lineCount() is > 1 if the user used Shift+Enter
            // firstBlockLayout->lineCount() is > 1 if a single long line wrapped around
            const QTextLayout *firstBlockLayout = document()->firstBlock().layout();
            if (document()->lineCount() > 1 || firstBlockLayout->lineCount() > 1) {
                KTextEdit::keyPressEvent(e);
                return;
            }
        }
    }
    e->ignore();
    // Check if the listview or room widget want to handle the key (e.g Esc, PageUp)
    Q_EMIT keyPressed(e);
    if (e->isAccepted()) {
        return;
    }
    // Assign key to KTextEdit first otherwise text() doesn't return correct text
    KTextEdit::keyPressEvent(e);
    if (key == Qt::Key_Delete || key == Qt::Key_Backspace) {
        if (textCursor().hasSelection() && textCursor().selectedText() == text()) {
            // We will clear all text => we will send textEditing is empty => clear notification
            Q_EMIT textEditing(true);
        } else {
            mCurrentInputTextManager->setInputTextChanged(roomId(), text(), textCursor().position());
            Q_EMIT textEditing(document()->isEmpty());
        }
    } else {
        if (!e->text().isEmpty() || e->matches(QKeySequence::Paste) || e->matches(QKeySequence::Redo) || e->matches(QKeySequence::Undo)) {
            mCurrentInputTextManager->setInputTextChanged(roomId(), text(), textCursor().position());
            Q_EMIT textEditing(document()->isEmpty());
        }
    }
}

void MessageTextEdit::mousePressEvent(QMouseEvent *ev)
{
    if (ev->buttons().testFlag(Qt::LeftButton)) {
        Q_EMIT textClicked();
    }
    KTextEdit::mousePressEvent(ev);
}

void MessageTextEdit::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        event->accept();
        // Don't let QTextEdit move the cursor around
        return;
    }
    KTextEdit::dragEnterEvent(event);
}

void MessageTextEdit::dragMoveEvent(QDragMoveEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        event->accept();
        // Don't let QTextEdit move the cursor around
        return;
    }
    KTextEdit::dragMoveEvent(event);
}

void MessageTextEdit::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        Q_EMIT handleMimeData(mimeData);
        event->accept();
        return;
    }
    KTextEdit::dropEvent(event);
}

void MessageTextEdit::slotCompletionTypeChanged(InputTextManager::CompletionForType type)
{
    if (type == InputTextManager::CompletionForType::Emoji) {
        // show emoji completion popup when typing ':'
        mEmojiCompletionListView->slotCompletionAvailable();
        mUserAndChannelCompletionListView->hide();
        mCommandCompletionListView->hide();
    } else if (type == InputTextManager::CompletionForType::None) {
        mUserAndChannelCompletionListView->hide();
        mEmojiCompletionListView->hide();
        mCommandCompletionListView->hide();
    } else if (type == InputTextManager::CompletionForType::Command) {
        mUserAndChannelCompletionListView->hide();
        mEmojiCompletionListView->hide();
        mCommandCompletionListView->slotCompletionAvailable();
    } else {
        // the user and channel completion inserts rows when typing '@' so it will trigger slotCompletionAvailable automatically
        mEmojiCompletionListView->hide();
        mCommandCompletionListView->hide();
    }
}

void MessageTextEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(InputCompleterModel::CompleterName).toString();
    if (completerName.isEmpty()) {
        return;
    }
    int textPos = textCursor().position();
    const QString newText = mCurrentInputTextManager->applyCompletion(completerName + u' ', text(), &textPos);

    mUserAndChannelCompletionListView->hide();
    mEmojiCompletionListView->hide();
    mCommandCompletionListView->hide();

    changeText(newText, textPos);
}

void MessageTextEdit::slotSelectFirstTextCompleter()
{
    const QModelIndex idx = mUserAndChannelCompletionListView->model()->index(0, 0);
    // qDebug() << " idx " << idx;
    mUserAndChannelCompletionListView->setCurrentIndex(idx);
}

#include "moc_messagetextedit.cpp"
