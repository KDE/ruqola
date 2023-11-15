/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

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
    void parse(const QJsonObject &obj);

    void setLoadCommonMessagesInProgress(bool loadSearchMessageInProgress);
    [[nodiscard]] bool loadCommonMessagesInProgress() const;

    void clearModel();
Q_SIGNALS:
    void stringNotFoundChanged();
    void loadingInProgressChanged();

protected:
    [[nodiscard]] virtual QVector<Message> extractMessages(const QJsonObject &obj);

private:
    LIBRUQOLACORE_NO_EXPORT void setStringNotFound(bool stringNotFound);
    bool mStringNotFound = true;
    bool mLoadingInProgress = false;
};
