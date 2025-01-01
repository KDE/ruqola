/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QList>
#include <QListView>
class PluginTextInterface;
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

Q_SIGNALS:
    void errorMessage(const QString &message);
    void successMessage(const QString &message);
    void textToSpeech(const QString &str);
    void needToClearSizeHintCache();

protected:
    void resizeEvent(QResizeEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void leaveEvent(QEvent *event) override;

    virtual bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);
    virtual bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);
    [[nodiscard]] QStyleOptionViewItem listViewOptions() const;

    void addTextPlugins(QMenu *menu, const QString &selectedText);

    QList<PluginTextInterface *> mPluginTextInterface;

    [[nodiscard]] virtual QString selectedText(const QModelIndex &index);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void handleMouseEvent(QMouseEvent *event);
    bool mAtBottom = true;
    QPoint mPressedPosition;
    QPersistentModelIndex mCurrentIndex = {};
};
