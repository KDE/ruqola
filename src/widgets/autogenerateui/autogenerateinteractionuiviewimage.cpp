/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewimage.h"
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewImage::AutoGenerateInteractionUiViewImage()

{
}

AutoGenerateInteractionUiViewImage::~AutoGenerateInteractionUiViewImage() = default;

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImage &t)
{
    d.space() << "altText:" << t.altText();
    d.space() << "imageUrl:" << t.imageUrl();
    return d;
}
void AutoGenerateInteractionUiViewImage::parse(const QJsonObject &json)
{
    mAltText = json["altText"_L1].toString();
    mImageUrl = json["imageUrl"_L1].toString();
}

QString AutoGenerateInteractionUiViewImage::altText() const
{
    return mAltText;
}

void AutoGenerateInteractionUiViewImage::setAltText(const QString &newAltText)
{
    mAltText = newAltText;
}

QString AutoGenerateInteractionUiViewImage::imageUrl() const
{
    return mImageUrl;
}

void AutoGenerateInteractionUiViewImage::setImageUrl(const QString &newImageUrl)
{
    mImageUrl = newImageUrl;
}

bool AutoGenerateInteractionUiViewImage::operator==(const AutoGenerateInteractionUiViewImage &other) const
{
    return other.altText() == altText() && other.imageUrl() == imageUrl();
}

QJsonObject AutoGenerateInteractionUiViewImage::serialize() const
{
    QJsonObject o;
    o["altText"_L1] = mAltText;
    o["imageUrl"_L1] = mImageUrl;
    return o;
}
