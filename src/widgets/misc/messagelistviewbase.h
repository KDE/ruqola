/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "texttospeech/texttospeechenqueueinfo.h"
#include <QList>
#include <QListView>
class PluginTextInterface;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListViewBase : public QListView
{
    Q_OBJECT
public:
    explicit MessageListViewBase(QWidget *parent = nullptr);
    ~MessageListViewBase() override;

    void checkIfAtBottom();
    void updateVerticalPageStep();
    void maybeScrollToBottom();
    void copyMessageToClipboard(const QModelIndex &index = {});
    // The view never takes the keyboard focus (see the constructor), so the
    // Ctrl+C shown in the context menus can only work when the widget which
    // does take it forwards the key press to us.
    void forwardCopyShortcut(QLineEdit *lineEdit);

Q_SIGNALS:
    void errorMessage(const QString &message);
    void successMessage(const QString &message);
    // info is empty when the view has no message information to report: the
    // receiver must still store it (see TextToSpeechEnqueueUtils::enqueue).
    void textToSpeech(const QString &str, const TextToSpeechEnqueueInfo &info = {});
    void needToClearSizeHintCache();

protected:
    void resizeEvent(QResizeEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void leaveEvent(QEvent *event) override;

    [[nodiscard]] bool eventFilter(QObject *watched, QEvent *event) override;

    virtual bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);
    virtual bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);
    [[nodiscard]] QStyleOptionViewItem listViewOptions() const;

    void addTextPlugins(QMenu *menu, const QString &selectedText);
    [[nodiscard]] virtual QString selectedText(const QModelIndex &index);
    [[nodiscard]] virtual bool hasSelection() const;

    void slotStopTextToSpeech(const QModelIndex &index);

    QList<PluginTextInterface *> mPluginTextInterface;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT bool handleMouseEvent(QMouseEvent *event);
    bool mAtBottom = true;
    QPoint mPressedPosition;
    QPersistentModelIndex mCurrentIndex;
    QLineEdit *mCopyShortcutLineEdit = nullptr;
};
