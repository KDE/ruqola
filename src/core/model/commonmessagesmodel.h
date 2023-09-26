/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "messagesmodel.h"
#include <QJsonObject>
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT CommonMessagesModel : public MessagesModel
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

private:
    void setStringNotFound(bool stringNotFound);
    int mTotal = -1;
    bool mStringNotFound = true;
    bool mLoadingInProgress = false;
};
