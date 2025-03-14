/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "autogenerateui/autogenerateinteractionuiviewimage.h"

class AutoGenerateInteractionUiViewText;
// ./src/blocks/layout/ImageBlock.ts
class AutoGenerateInteractionUiViewImageBlock : public AutoGenerateInteractionUiViewBlockBase
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewImageBlock(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewImageBlock() override;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] AutoGenerateInteractionUiViewImage image() const;
    void setImage(const AutoGenerateInteractionUiViewImage &newImage);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewImageBlock &other) const;

    [[nodiscard]] AutoGenerateInteractionUiViewText *title() const;
    void setTitle(AutoGenerateInteractionUiViewText *newText);

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;

private:
    AutoGenerateInteractionUiViewImage mImage;
    AutoGenerateInteractionUiViewText *mTitle = nullptr;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewImageBlock)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageBlock &t);
