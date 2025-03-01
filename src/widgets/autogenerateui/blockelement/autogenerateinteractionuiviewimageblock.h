/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "autogenerateui/autogenerateinteractionuiviewimage.h"

// ./src/blocks/layout/ImageBlock.ts
class AutoGenerateInteractionUiViewImageBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewImageBlock();
    ~AutoGenerateInteractionUiViewImageBlock();
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) const override;
    void parse(const QJsonObject &json) override;

    [[nodiscard]] AutoGenerateInteractionUiViewImage image() const;
    void setImage(const AutoGenerateInteractionUiViewImage &newImage);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewImageBlock &other) const;

protected:
    void serializeBlock(QJsonObject &o) const override;

private:
    AutoGenerateInteractionUiViewImage mImage;
    // TODO title?: PlainText;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewImageBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewImageBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageBlock &t);
