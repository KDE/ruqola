/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "messagesmodel.h"

class RocketChatAccount;
class LIBRUQOLACORE_EXPORT CommonMessagesModel : public MessagesModel
{
    Q_OBJECT
public:
    explicit CommonMessagesModel(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~CommonMessagesModel() override;
    bool parse(const QJsonObject &obj, bool clearMessages = true, bool insertListMessages = false);

    void setLoadCommonMessagesInProgress(bool loadSearchMessageInProgress);
    [[nodiscard]] bool loadCommonMessagesInProgress() const;

    void clearModel();
Q_SIGNALS:
    void stringNotFoundChanged();
    void loadingInProgressChanged();

protected:
    [[nodiscard]] virtual QList<Message> extractMessages(const QJsonObject &obj);

private:
    LIBRUQOLACORE_NO_EXPORT void setStringNotFound(bool stringNotFound);
    bool mStringNotFound = true;
    bool mLoadingInProgress = false;
};
