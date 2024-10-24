/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinfo.h"
#include "utils.h"
#include <KLocalizedString>

#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;
AppsMarketPlaceInfo::AppsMarketPlaceInfo() = default;

AppsMarketPlaceInfo::~AppsMarketPlaceInfo() = default;

QDebug operator<<(QDebug d, const AppsMarketPlaceInfo &t)
{
    d.space() << "appId " << t.appId();
    d.space() << "isEnterpriseOnly " << t.isEnterpriseOnly();
    d.space() << "appName " << t.appName();
    d.space() << "categories " << t.categories();
    d.space() << "documentationUrl " << t.documentationUrl();
    d.space() << "purchaseType " << t.purchaseType();
    d.space() << "description " << t.description();
    d.space() << "price " << t.price();
    d.space() << "version " << t.version();
    d.space() << "shortDescription " << t.shortDescription();
    d.space() << "pixmap is valid " << !t.pixmap().isNull();
    d.space() << "modifiedDate " << t.modifiedDate();
    d.space() << "isPaid " << t.isPaid();
    d.space() << "pricePlan " << t.pricePlan();
    d.space() << "isPrivate " << t.isPrivate();
    d.space() << "support " << t.support();
    d.space() << "homePage " << t.homePage();
    d.space() << "author " << t.authorName();
    d.space() << "privacyPolicySummary " << t.privacyPolicySummary();
    d.space() << "requested " << t.requested();
    d.space() << "installed " << t.installed();
    d.space() << "migrated " << t.migrated();
    return d;
}

QDebug operator<<(QDebug d, const AppsMarketPlaceInfo::PricePlan &t)
{
    d.space() << "price " << t.price;
    d.space() << "trialDays " << t.trialDays;
    d.space() << "enabled " << t.enabled;
    d.space() << "strategy " << t.strategy;
    d.space() << "isPerSeat " << t.isPerSeat;
    return d;
}

void AppsMarketPlaceInfo::parsePrincingPlan(const QJsonArray &array)
{
    mPricePlan.clear();
    for (const QJsonValue &current : array) {
        PricePlan price;
        price.price = current["price"_L1].toInt();
        price.trialDays = current["trialDays"_L1].toInt();
        price.enabled = current["enabled"_L1].toBool();
        price.isPerSeat = current["isPerSeat"_L1].toBool();
        price.strategy = price.convertStringToStrategy(current["strategy"_L1].toString());
        mPricePlan.append(std::move(price));
    }
}

QString AppsMarketPlaceInfo::generatePriceInfo() const
{
    QString pricingStr;
    for (const auto &p : mPricePlan) {
        if (!pricingStr.isEmpty()) {
            pricingStr += QStringLiteral("<br/>");
        }
        QString value;
        const QString strategy = p.strategyToI18n();
        value = QStringLiteral("$") + QString::number(p.price);
        if (!strategy.isEmpty()) {
            value += QLatin1Char(' ') + i18n("per %1", strategy);
        }
        pricingStr += value;
    }
    return pricingStr;
}

QString AppsMarketPlaceInfo::homePage() const
{
    return mHomePage;
}

void AppsMarketPlaceInfo::setHomePage(const QString &newHomePage)
{
    mHomePage = newHomePage;
}

QString AppsMarketPlaceInfo::support() const
{
    return mSupport;
}

void AppsMarketPlaceInfo::setSupport(const QString &newSupport)
{
    mSupport = newSupport;
}

bool AppsMarketPlaceInfo::isPrivate() const
{
    return mIsPrivate;
}

void AppsMarketPlaceInfo::setIsPrivate(bool newIsPrivate)
{
    mIsPrivate = newIsPrivate;
}

bool AppsMarketPlaceInfo::PricePlan::operator==(const AppsMarketPlaceInfo::PricePlan &other) const
{
    return price == other.price && trialDays == other.trialDays && strategy == other.strategy && enabled == other.enabled && isPerSeat == other.isPerSeat;
}

AppsMarketPlaceInfo::PricePlan::Strategy AppsMarketPlaceInfo::PricePlan::convertStringToStrategy(const QString &str) const
{
    if (str == "monthly"_L1) {
        return AppsMarketPlaceInfo::PricePlan::Strategy::Monthly;
    } else if (str == "yearly"_L1) {
        return AppsMarketPlaceInfo::PricePlan::Strategy::Yearly;
    }
    return AppsMarketPlaceInfo::PricePlan::Strategy::Unknown;
}

QString AppsMarketPlaceInfo::PricePlan::strategyToI18n() const
{
    switch (strategy) {
    case Unknown:
        return {};
    case Monthly:
        return i18n("month");
    case Yearly:
        return i18n("year");
    }
    return {};
}

void AppsMarketPlaceInfo::parseInstalledApps(const QJsonObject &replyObject)
{
}

void AppsMarketPlaceInfo::parseAppRequestStats(const QJsonObject &replyObject)
{
    // "appRequestStats":{"appId":"ebb7f05b-ea65-4565-880b-8c2360f14500","totalSeen":1}
    mRequested = replyObject["totalSeen"_L1].toInt();
}

QString AppsMarketPlaceInfo::authorName() const
{
    return mAuthorName;
}

void AppsMarketPlaceInfo::setAuthorName(const QString &newAuthorName)
{
    mAuthorName = newAuthorName;
}

bool AppsMarketPlaceInfo::migrated() const
{
    return mMigrated;
}

void AppsMarketPlaceInfo::setMigrated(bool newMigrated)
{
    mMigrated = newMigrated;
}

bool AppsMarketPlaceInfo::installed() const
{
    return mInstalled;
}

void AppsMarketPlaceInfo::setInstalled(bool newInstalled)
{
    mInstalled = newInstalled;
}

int AppsMarketPlaceInfo::requested() const
{
    return mRequested;
}

void AppsMarketPlaceInfo::setRequested(int newRequested)
{
    mRequested = newRequested;
}

void AppsMarketPlaceInfo::parseAuthor(const QJsonObject &authorObject)
{
    mHomePage = authorObject["homepage"_L1].toString();
    mSupport = authorObject["support"_L1].toString();
    mAuthorName = authorObject["name"_L1].toString();
}

QString AppsMarketPlaceInfo::privacyPolicySummary() const
{
    return mPrivacyPolicySummary;
}

void AppsMarketPlaceInfo::setPrivacyPolicySummary(const QString &newPrivacyPolicySummary)
{
    mPrivacyPolicySummary = newPrivacyPolicySummary;
}

void AppsMarketPlaceInfo::parseAppsMarketPlaceInfo(const QJsonObject &replyObject)
{
    // qDebug() << " replyObject " << replyObject;
    mAppId = replyObject["appId"_L1].toString().toLatin1();
    mIsEnterpriseOnly = replyObject["isEnterpriseOnly"_L1].toBool();
    mPurchaseType = replyObject["purchaseType"_L1].toString();

    mModifiedDate = Utils::parseIsoDate("modifiedAt"_L1, replyObject);

    parsePrincingPlan(replyObject["pricingPlans"_L1].toArray());
    // TODO implement plans support
    mPrice = replyObject["price"_L1].toInt();

    const QJsonObject latestObj = replyObject["latest"_L1].toObject();
    const QJsonArray categoriesArray = latestObj["categories"_L1].toArray();

    QStringList lst;
    lst.reserve(categoriesArray.count());
    for (const QJsonValue &current : categoriesArray) {
        lst.append(current.toString());
    }
    mCategories = lst;

    parseAuthor(latestObj["author"_L1].toObject());

    const QJsonObject detailedDescription = latestObj["detailedDescription"_L1].toObject();
    if (detailedDescription.contains("rendered"_L1)) {
        mDescription = detailedDescription["rendered"_L1].toString();
    } else {
        mDescription = latestObj["description"_L1].toString();
    }
    mShortDescription = latestObj["shortDescription"_L1].toString();
    mVersion = latestObj["version"_L1].toString();
    mAppName = latestObj["name"_L1].toString();
    mDocumentationUrl = latestObj["documentationUrl"_L1].toString();
    const QByteArray baImageBase64 = latestObj["iconFileData"_L1].toString().toLatin1();
    mPixmap.loadFromData(QByteArray::fromBase64(baImageBase64), "PNG");

    mPrivacyPolicySummary = latestObj["privacyPolicySummary"_L1].toString();
    parseAppRequestStats(replyObject["appRequestStats"_L1].toObject());

    // TODO qDebug() << " XXXXXXXXXX " << replyObject["status"_L1].toString();
}

QByteArray AppsMarketPlaceInfo::appId() const
{
    return mAppId;
}

void AppsMarketPlaceInfo::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

bool AppsMarketPlaceInfo::isEnterpriseOnly() const
{
    return mIsEnterpriseOnly;
}

void AppsMarketPlaceInfo::setIsEnterpriseOnly(bool newIsEnterpriseOnly)
{
    mIsEnterpriseOnly = newIsEnterpriseOnly;
}

QString AppsMarketPlaceInfo::appName() const
{
    return mAppName;
}

void AppsMarketPlaceInfo::setAppName(const QString &newAppName)
{
    mAppName = newAppName;
}

QStringList AppsMarketPlaceInfo::categories() const
{
    return mCategories;
}

void AppsMarketPlaceInfo::setCategories(const QStringList &newCategories)
{
    mCategories = newCategories;
}

bool AppsMarketPlaceInfo::isValid() const
{
    // TODO
    return !mAppId.isEmpty();
}

QString AppsMarketPlaceInfo::documentationUrl() const
{
    return mDocumentationUrl;
}

void AppsMarketPlaceInfo::setDocumentationUrl(const QString &newDocumentationUrl)
{
    mDocumentationUrl = newDocumentationUrl;
}

QString AppsMarketPlaceInfo::purchaseType() const
{
    return mPurchaseType;
}

void AppsMarketPlaceInfo::setPurchaseType(const QString &newPurchaseType)
{
    mPurchaseType = newPurchaseType;
}

int AppsMarketPlaceInfo::price() const
{
    return mPrice;
}

void AppsMarketPlaceInfo::setPrice(int newPrice)
{
    mPrice = newPrice;
}

QString AppsMarketPlaceInfo::description() const
{
    return mDescription;
}

void AppsMarketPlaceInfo::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QPixmap AppsMarketPlaceInfo::pixmap() const
{
    return mPixmap;
}

void AppsMarketPlaceInfo::setPixmap(const QPixmap &newIcon)
{
    mPixmap = newIcon;
}

QString AppsMarketPlaceInfo::version() const
{
    return mVersion;
}

void AppsMarketPlaceInfo::setVersion(const QString &newVersion)
{
    mVersion = newVersion;
}

QString AppsMarketPlaceInfo::shortDescription() const
{
    return mShortDescription;
}

void AppsMarketPlaceInfo::setShortDescription(const QString &newShortDescription)
{
    mShortDescription = newShortDescription;
}

bool AppsMarketPlaceInfo::operator==(const AppsMarketPlaceInfo &other) const
{
    return mCategories == other.mCategories && mAppId == other.mAppId && mAppName == other.mAppName && mDescription == other.mDescription
        && mDocumentationUrl == other.mDocumentationUrl && mPurchaseType == other.mPurchaseType && mVersion == other.mVersion
        && mShortDescription == other.mShortDescription /*&& mPixmap.isNull() == other.mPixmap.isNull()*/ && mPrice == other.mPrice
        && mIsEnterpriseOnly == other.mIsEnterpriseOnly && mModifiedDate == other.mModifiedDate && mPricePlan == other.mPricePlan
        && mHomePage == other.mHomePage && mSupport == other.mSupport && mPrivacyPolicySummary == other.mPrivacyPolicySummary && mRequested == other.mRequested
        && mInstalled == other.mInstalled && mMigrated == other.mMigrated && mAuthorName == other.mAuthorName;
}

qint64 AppsMarketPlaceInfo::modifiedDate() const
{
    return mModifiedDate;
}

void AppsMarketPlaceInfo::setModifiedDate(qint64 newModifiedDate)
{
    mModifiedDate = newModifiedDate;
}

QString AppsMarketPlaceInfo::applicationInformations() const
{
    QString str = QStringLiteral("<b>%1</b><br/>").arg(mAppName);
    str += mShortDescription + QStringLiteral("<br/><br/>");

    const QString pricingInfo = generatePriceInfo();
    if (!pricingInfo.isEmpty()) {
        str += pricingInfo + QStringLiteral("<br/><br/>");
    }

    QString newDescription = mDescription;

    str += QStringLiteral("<b>%1</b><br/>").arg(i18n("Description")) + newDescription + QStringLiteral("<br/><br/>");

    str += QStringLiteral("<b>%1</b><br/>").arg(i18n("Version")) + mVersion + QStringLiteral("<br/><br/>");

    if (!mCategories.isEmpty()) {
        str += QStringLiteral("<b>%1</b><br/>").arg(i18n("Categories")) + mCategories.join(QStringLiteral(", ")) + QStringLiteral("<br/><br/>");
    }
    if (!mDocumentationUrl.isEmpty()) {
        const QString url = mDocumentationUrl.startsWith(QStringLiteral("http")) ? mDocumentationUrl : QStringLiteral("https://%1").arg(mDocumentationUrl);
        str += QStringLiteral("<b>%1</b><br/>").arg(i18n("Documentation")) + QStringLiteral("<a href=\"%2\">%1</a>").arg(mDocumentationUrl, url)
            + QStringLiteral("<br/><br/>");
    }

    if (!mAuthorName.isEmpty()) {
        str += QStringLiteral("<b>%1</b><br/>").arg(i18n("Author")) + mAuthorName + QStringLiteral("<br/><br/>");
    }

    if (!mHomePage.isEmpty()) {
        const QString url = mHomePage.startsWith(QStringLiteral("http")) ? mHomePage : QStringLiteral("https://%1").arg(mHomePage);
        str +=
            QStringLiteral("<b>%1</b><br/>").arg(i18n("Homepage")) + QStringLiteral("<a href=\"%2\">%1</a>").arg(mHomePage, url) + QStringLiteral("<br/><br/>");
    }

    if (!mSupport.isEmpty()) {
        const QString url = mSupport.startsWith(QStringLiteral("http")) ? mSupport : QStringLiteral("mailto://%1").arg(mSupport);
        str +=
            QStringLiteral("<b>%1</b><br/>").arg(i18n("Support")) + QStringLiteral("<a href=\"%2\">%1</a>").arg(mSupport, url) + QStringLiteral("<br/><br/>");
    }

    if (!mPrivacyPolicySummary.isEmpty()) {
        str += QStringLiteral("<b>%1</b><br/>").arg(i18n("Privacy Summary")) + mPrivacyPolicySummary + QStringLiteral("<br/><br/>");
    }

    return str;
}

bool AppsMarketPlaceInfo::isPaid() const
{
    return !mPricePlan.isEmpty();
}

QList<AppsMarketPlaceInfo::PricePlan> AppsMarketPlaceInfo::pricePlan() const
{
    return mPricePlan;
}

void AppsMarketPlaceInfo::setPricePlan(const QList<PricePlan> &newPricePlan)
{
    mPricePlan = newPricePlan;
}

bool AppsMarketPlaceInfo::Permission::operator==(const Permission &other) const
{
    // TODO
    return true;
}

QString AppsMarketPlaceInfo::Permission::convertTypeToI18n() const
{
    // TODO
    return {};
}
