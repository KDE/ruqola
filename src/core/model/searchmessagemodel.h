/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "messagemodel.h"
#include <QJsonObject>
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT SearchMessageModel : public MessageModel
{
    Q_OBJECT
public:
    explicit SearchMessageModel(const QString &roomID = QStringLiteral("no_room"),
                                RocketChatAccount *account = nullptr,
                                Room *room = nullptr,
                                QObject *parent = nullptr);
    ~SearchMessageModel() override;
    void parse(const QJsonObject &obj);

    Q_REQUIRED_RESULT bool stringNotFound() const;

    void setLoadSearchMessageInProgress(bool loadSearchMessageInProgress);
    Q_REQUIRED_RESULT bool loadSearchMessageInProgress() const;

    void clearModel();
Q_SIGNALS:
    void stringNotFoundChanged();
    void loadingInProgressChanged();

private:
    void checkFullList();
    void setStringNotFound(bool stringNotFound);
    QString mRoomId;
    int mTotal = 0;
    bool mStringNotFound = true;
    bool mLoadSearchMessageInProgress = false;
};
