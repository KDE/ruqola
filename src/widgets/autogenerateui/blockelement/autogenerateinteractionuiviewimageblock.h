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
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewImageBlock(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewImageBlock() override;
    [[nodiscard]] QWidget *generateWidget(RocketChatAccount *account, QWidget *parent) override;

    [[nodiscard]] AutoGenerateInteractionUiViewImage image() const;
    void setImage(const AutoGenerateInteractionUiViewImage &newImage);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewImageBlock &other) const;

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;

private:
    AutoGenerateInteractionUiViewImage mImage;
    // TODO title?: PlainText;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewImageBlock)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageBlock &t);
