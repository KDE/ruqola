/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customsound/customsoundsinfo.h"
#include "directorybasemodel.h"
#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT AdminCustomSoundModel : public DirectoryBaseModel
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

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;


    Q_REQUIRED_RESULT int total() const override;
    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;

    const CustomSoundsInfo &customSounds() const;
    void setCustomSounds(const CustomSoundsInfo &newCustomSounds);

    void removeElement(const QString &identifier) override;

private:
    Q_DISABLE_COPY(AdminCustomSoundModel)
    void checkFullList();
    CustomSoundsInfo mCustomSounds;
};
