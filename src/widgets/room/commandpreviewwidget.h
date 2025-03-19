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

Q_SIGNALS:
    void sendPreviewCommandInfo(const RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo &info);

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotParsePreviewCommandItems(const QJsonObject &replyObject);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDoubleClicked(const QModelIndex &);
    RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo mPreviewCommandInfo;
    QListView *const mListView;
    PreviewCommandModel *const mPreviewCommandModel;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
