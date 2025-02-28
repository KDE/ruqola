/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "commands/previewscommandjob.h"
#include "libruqolawidgets_private_export.h"
#include <QPointer>
#include <QWidget>
class QListView;
class PreviewCommandModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CommandPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommandPreviewWidget(QWidget *parent = nullptr);
    ~CommandPreviewWidget() override;

    void setCurrentRocketChatAccount(RocketChatAccount *account);
    void setPreviewCommandInfo(const RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo &info);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotParsePreviewCommandItems(const QJsonObject &replyObject);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDoubleClicked(const QModelIndex &);
    QListView *const mListView;
    PreviewCommandModel *const mPreviewCommandModel;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
