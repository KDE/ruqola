/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "custombasemodel.h"
#include "customsound/customsoundsinfo.h"
#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT AdminCustomSoundModel : public CustomBaseModel
{
    Q_OBJECT
public:
    enum CustomSoundsRoles {
        Name,
        Identifier,
        LastColumn = Identifier,
    };
    Q_ENUM(CustomSoundsRoles)

    explicit AdminCustomSoundModel(QObject *parent = nullptr);
    ~AdminCustomSoundModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] int total() const override;
    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;
    [[nodiscard]] QList<int> hideColumns() const override;

    const CustomSoundsInfo &customSounds() const;
    void setCustomSounds(const CustomSoundsInfo &newCustomSounds);

    void removeElement(const QString &identifier) override;

private:
    Q_DISABLE_COPY(AdminCustomSoundModel)
    LIBRUQOLACORE_NO_EXPORT void checkFullList() override;
    LIBRUQOLACORE_NO_EXPORT void clear();
    CustomSoundsInfo mCustomSounds;
};
