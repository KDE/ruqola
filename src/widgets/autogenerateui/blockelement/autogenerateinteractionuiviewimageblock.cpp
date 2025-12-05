/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewimageblock.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonObject>

// Look at /apps/uikit-playground/src/Payload/actionBlock/image

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewImageBlock::AutoGenerateInteractionUiViewImageBlock(QObject *parent)
    : AutoGenerateInteractionUiViewBlockBase(parent)
{
}
AutoGenerateInteractionUiViewImageBlock::~AutoGenerateInteractionUiViewImageBlock()
{
    delete mTitle;
}

QWidget *AutoGenerateInteractionUiViewImageBlock::generateWidget([[maybe_unused]] QWidget *parent)
{
    qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewImageBlock::generateWidget not implemented yet";
    return nullptr;
}

void AutoGenerateInteractionUiViewImageBlock::parseBlock(const QJsonObject &json)
{
    mImage.parse(json);
    if (json.contains("title"_L1)) {
        mTitle = new AutoGenerateInteractionUiViewText;
        mTitle->parse(json["title"_L1].toObject());
    }
}

QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> AutoGenerateInteractionUiViewImageBlock::serializeBlockState() const
{
    return {};
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewImageBlock::title() const
{
    return mTitle;
}

void AutoGenerateInteractionUiViewImageBlock::setTitle(AutoGenerateInteractionUiViewText *newText)
{
    mTitle = newText;
}

void AutoGenerateInteractionUiViewImageBlock::setErrorMessages(const QMap<QString, QString> &map)
{
    Q_UNUSED(map);
}

void AutoGenerateInteractionUiViewImageBlock::assignState(const QList<StateInfo> &info)
{
    Q_UNUSED(info);
}

AutoGenerateInteractionUiViewImage AutoGenerateInteractionUiViewImageBlock::image() const
{
    return mImage;
}

void AutoGenerateInteractionUiViewImageBlock::setImage(const AutoGenerateInteractionUiViewImage &newImage)
{
    mImage = newImage;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageBlock &t)
{
    d.space() << "image:" << t.image();
    if (t.title()) {
        d.space() << "text:" << *t.title();
    }
    return d;
}

bool AutoGenerateInteractionUiViewImageBlock::operator==(const AutoGenerateInteractionUiViewImageBlock &other) const
{
    // TODO title
    return AutoGenerateInteractionUiViewBlockBase::operator==(other) && other.image() == image();
}

void AutoGenerateInteractionUiViewImageBlock::serializeBlock(QJsonObject &o) const
{
    o["image"_L1] = mImage.serialize();
    if (mTitle) {
        o["title"_L1] = mTitle->serialize();
    }
}

#include "moc_autogenerateinteractionuiviewimageblock.cpp"
