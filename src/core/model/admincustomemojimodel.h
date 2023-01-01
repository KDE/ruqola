/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customsound/customsoundsinfo.h"
#include "directorybasemodel.h"
#include "emoticons/customemojisinfo.h"
#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT AdminCustomEmojiModel : public DirectoryBaseModel
{
    Q_OBJECT
public:
    enum CustomEmojiRoles {
        Name,
        Identifier,
        Aliases,
        LastColumn = Aliases,
    };
    Q_ENUM(CustomEmojiRoles)

    explicit AdminCustomEmojiModel(QObject *parent = nullptr);
    ~AdminCustomEmojiModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT int total() const override;
    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;

    const CustomEmojisInfo &customEmojis() const;
    void setCustomEmojis(const CustomEmojisInfo &newCustomEmojis);

protected:
    void removeElement(const QString &identifier) override;

private:
    Q_DISABLE_COPY(AdminCustomEmojiModel)
    void checkFullList();
    CustomEmojisInfo mCustomEmojiList;
};
