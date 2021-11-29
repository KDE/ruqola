/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "messagetextedit.h"
#include "common/commandcompletiondelegate.h"
#include "common/completionlistview.h"
#include "common/emojicompletiondelegate.h"
#include "model/inputcompletermodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include <KLocalizedString>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QAbstractTextDocumentLayout>
#include <QKeyEvent>
#include <QMenu>
#include <QTextCursor>
#include <QTextDocument>

MessageTextEdit::MessageTextEdit(QWidget *parent)
    : KTextEdit(parent)
    , mUserAndChannelCompletionListView(new CompletionListView)
    , mEmojiCompletionListView(new CompletionListView)
    , mCommandCompletionListView(new CompletionListView)
{
    setAcceptRichText(false);
    enableFindReplace(false); // not needed here, let's instead make sure the Ctrl+F shortcut will search through channel history

    connect(document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged, this, &QWidget::updateGeometry);

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
    KConfigGroup group(config, "Spelling");
    group.writeEntry("checkerEnabledByDefault", b);
}

void MessageTextEdit::slotLanguageChanged(const QString &lang)
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group(config, "Spelling");
    group.writeEntry("Language", lang);
}

void MessageTextEdit::loadSpellCheckingSettings()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    if (config->hasGroup("Spelling")) {
        KConfigGroup group(config, "Spelling");
        setCheckSpellingEnabled(group.readEntry("checkerEnabledByDefault", false));
        setSpellCheckingLanguage(group.readEntry("Language", QString()));
    }
}

void MessageTextEdit::setCurrentRocketChatAccount(RocketChatAccount *account, bool threadMessageDialog)
{
    if (mCurrentInputTextManager) {
        disconnect(mCurrentInputTextManager, &InputTextManager::completionTypeChanged, this, &MessageTextEdit::slotCompletionTypeChanged);
    }
    mCurrentRocketChatAccount = account;
    mCurrentInputTextManager = threadMessageDialog ? mCurrentRocketChatAccount->inputThreadMessageTextManager() : mCurrentRocketChatAccount->inputTextManager();
    mUserAndChannelCompletionListView->setModel(mCurrentInputTextManager->inputCompleterModel());
    mEmojiCompletionListView->setModel(mCurrentInputTextManager->emojiCompleterModel());
    mCommandCompletionListView->setModel(mCurrentInputTextManager->commandModel());
    connect(mCurrentInputTextManager, &InputTextManager::completionTypeChanged, this, &MessageTextEdit::slotCompletionTypeChanged);
}

void MessageTextEdit::insertEmoji(const QString &text)
{
    textCursor().insertText(text + QLatin1Char(' '));
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

    mCurrentInputTextManager->setInputTextChanged(text(), cursorPosition);
}

QMenu *MessageTextEdit::mousePopupMenu()
{
    QMenu *menu = KTextEdit::mousePopupMenu();
    menu->addSeparator();
    auto formatMenu = new QMenu(menu);
    formatMenu->setTitle(i18n("Change Text Format"));
    menu->addMenu(formatMenu);
    formatMenu->addAction(QIcon::fromTheme(QStringLiteral("format-text-bold")), i18n("Bold"), this, &MessageTextEdit::slotSetAsBold);
    formatMenu->addAction(QIcon::fromTheme(QStringLiteral("format-text-italic")), i18n("Italic"), this, &MessageTextEdit::slotSetAsItalic);
    formatMenu->addAction(QIcon::fromTheme(QStringLiteral("format-text-strikethrough")), i18n("Strike-out"), this, &MessageTextEdit::slotSetAsStrikeOut);
    formatMenu->addSeparator();
    formatMenu->addAction(i18n("Code Block"), this, &MessageTextEdit::slotInsertCodeBlock);
    return menu;
}

void MessageTextEdit::slotInsertCodeBlock()
{
    const QString textCodeBlock{QStringLiteral("```")};
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection()) {
        const QString text = textCodeBlock + QLatin1Char('\n') + cursor.selectedText() + QLatin1Char('\n') + textCodeBlock;
        cursor.insertText(text);
    } else {
        cursor.insertText(QString(textCodeBlock + QStringLiteral("\n\n") + textCodeBlock));
    }
    cursor.setPosition(cursor.position() - 4);
    setTextCursor(cursor);
}

void MessageTextEdit::slotSetAsStrikeOut()
{
    insertFormat(QLatin1Char('~'));
}

void MessageTextEdit::slotSetAsBold()
{
    insertFormat(QLatin1Char('*'));
}

void MessageTextEdit::slotSetAsItalic()
{
    insertFormat(QLatin1Char('_'));
}

void MessageTextEdit::insertFormat(QChar formatChar)
{
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection()) {
        const QString text = formatChar + cursor.selectedText() + formatChar;
        cursor.insertText(text);
    } else {
        cursor.insertText(QString(formatChar + formatChar));
    }
    cursor.setPosition(cursor.position() - 1);
    setTextCursor(cursor);
}

void MessageTextEdit::keyPressEvent(QKeyEvent *e)
{
    const int key = e->key();
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
        // document()->lineCount() is > 1 if the user used Shift+Enter
        // firstBlockLayout->lineCount() is > 1 if a single long line wrapped around
        const QTextLayout *firstBlockLayout = document()->firstBlock().layout();
        if (document()->lineCount() > 1 || firstBlockLayout->lineCount() > 1) {
            KTextEdit::keyPressEvent(e);
            return;
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
            mCurrentInputTextManager->setInputTextChanged(text(), textCursor().position());
            Q_EMIT textEditing(document()->isEmpty());
        }
    } else {
        if (!e->text().isEmpty() || e->matches(QKeySequence::Paste) || e->matches(QKeySequence::Redo) || e->matches(QKeySequence::Undo)) {
            mCurrentInputTextManager->setInputTextChanged(text(), textCursor().position());
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

void MessageTextEdit::slotCompletionTypeChanged(InputTextManager::CompletionForType type)
{
    if (type == InputTextManager::Emoji) {
        // show emoji completion popup when typing ':'
        mEmojiCompletionListView->slotCompletionAvailable();
        mUserAndChannelCompletionListView->hide();
        mCommandCompletionListView->hide();
    } else if (type == InputTextManager::None) {
        mUserAndChannelCompletionListView->hide();
        mEmojiCompletionListView->hide();
        mCommandCompletionListView->hide();
    } else if (type == InputTextManager::Command) {
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
    int textPos = textCursor().position();
    const QString newText = mCurrentInputTextManager->applyCompletion(completerName + QLatin1Char(' '), text(), &textPos);

    mUserAndChannelCompletionListView->hide();
    mEmojiCompletionListView->hide();
    mCommandCompletionListView->hide();

    changeText(newText, textPos);
}
