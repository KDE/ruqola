/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinfo.h"
#include "ktexttohtmlfork/ruqolaktexttohtml.h"
#include "ruqola_debug.h"
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
    d.space() << "support " << t.support();
    d.space() << "homePage " << t.homePage();
    d.space() << "author " << t.authorName();
    d.space() << "privacyPolicySummary " << t.privacyPolicySummary();
    d.space() << "requested " << t.requested();
    d.space() << "installedInfo " << t.installedInfo();
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

void AppsMarketPlaceInfo::parsePermissions(const QJsonArray &array)
{
    mPermissions.clear();
    for (const QJsonValue &current : array) {
        Permission perm;
        perm.type = perm.convertStringToPermissionType(current["name"_L1].toString());
        mPermissions.append(std::move(perm));
    }
}

AppsMarketPlaceInstalledInfo AppsMarketPlaceInfo::installedInfo() const
{
    return mInstalledInfo;
}

void AppsMarketPlaceInfo::setInstalledInfo(const AppsMarketPlaceInstalledInfo &newInstalledInfo)
{
    mInstalledInfo = newInstalledInfo;
}

void AppsMarketPlaceInfo::clearInstalledInfo()
{
    mInstalledInfo = {};
}

void AppsMarketPlaceInfo::changeApplicationStatus(const QString &str)
{
    mInstalledInfo.changeApplicationStatus(str);
}

void AppsMarketPlaceInfo::parsePrincingPlan(const QJsonArray &array)
{
    mPricePlan.clear();
    for (const QJsonValue &current : array) {
        PricePlan priceElement;
        priceElement.price = current["price"_L1].toInt();
        priceElement.trialDays = current["trialDays"_L1].toInt();
        priceElement.enabled = current["enabled"_L1].toBool();
        priceElement.isPerSeat = current["isPerSeat"_L1].toBool();
        priceElement.strategy = priceElement.convertStringToStrategy(current["strategy"_L1].toString());
        mPricePlan.append(std::move(priceElement));
    }
}

QString AppsMarketPlaceInfo::generatePriceInfo() const
{
    QString pricingStr;
    for (const auto &p : mPricePlan) {
        if (!pricingStr.isEmpty()) {
            pricingStr += u"<br/>"_s;
        }
        QString value;
        const QString strategy = p.strategyToI18n();
        value = u"$"_s + QString::number(p.price);
        if (!strategy.isEmpty()) {
            value += u' ' + i18n("per %1", strategy);
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
    qCWarning(RUQOLA_LOG) << "Strategy Unknown type " << str;
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
    mPrice = replyObject["price"_L1].toInt();

    const QJsonObject latestObj = replyObject["latest"_L1].toObject();
    const QJsonArray categoriesArray = latestObj["categories"_L1].toArray();
    parsePermissions(latestObj["permissions"_L1].toArray());

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
    return !mAppId.isEmpty() || mInstalledInfo.isValid();
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
        && mAuthorName == other.mAuthorName;
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
    QString str = u"<b>%1</b><br/>"_s.arg(mAppName);
    if (!mShortDescription.isEmpty()) {
        str += mShortDescription + u"<br/><br/>"_s;
    } else {
        str += u"<br/>"_s;
    }

    const QString pricingInfo = generatePriceInfo();
    if (!pricingInfo.isEmpty()) {
        str += pricingInfo + u"<br/><br/>"_s;
    }

    QString newDescription;
    if (mInstalledInfo.isValid() && !mInstalledInfo.description().isEmpty()) {
        newDescription = mInstalledInfo.description();
    } else {
        newDescription = mAuthorName;
    }

    str += u"<b>%1</b><br/>"_s.arg(i18n("Description")) + newDescription + u"<br/><br/>"_s;

    QString versionnfo;
    if (mInstalledInfo.isValid() && !mInstalledInfo.version().isEmpty()) {
        versionnfo = mInstalledInfo.version();
    } else {
        versionnfo = mVersion;
    }

    str += u"<b>%1</b><br/>"_s.arg(i18n("Version")) + versionnfo + u"<br/><br/>"_s;

    if (!mCategories.isEmpty()) {
        str += u"<b>%1</b><br/>"_s.arg(i18n("Categories")) + mCategories.join(u", "_s) + u"<br/><br/>"_s;
    }
    if (!mDocumentationUrl.isEmpty()) {
        const QString url = mDocumentationUrl.startsWith(u"http"_s) ? mDocumentationUrl : u"https://%1"_s.arg(mDocumentationUrl);
        str += u"<b>%1</b><br/>"_s.arg(i18n("Documentation")) + u"<a href=\"%2\">%1</a>"_s.arg(mDocumentationUrl, url) + u"<br/><br/>"_s;
    }

    QString authorName;
    if (mInstalledInfo.isValid() && !mInstalledInfo.authorName().isEmpty()) {
        authorName = mInstalledInfo.authorName();
    } else {
        authorName = mAuthorName;
    }

    if (!authorName.isEmpty()) {
        str += u"<b>%1</b><br/>"_s.arg(i18n("Author")) + authorName + u"<br/><br/>"_s;
    }

    QString homePage;
    if (mInstalledInfo.isValid() && !mInstalledInfo.homePage().isEmpty()) {
        homePage = mInstalledInfo.homePage();
    } else {
        homePage = mHomePage;
    }

    if (!homePage.isEmpty()) {
        const QString url = homePage.startsWith(u"http"_s) ? homePage : u"https://%1"_s.arg(homePage);
        str += u"<b>%1</b><br/>"_s.arg(i18n("Homepage")) + u"<a href=\"%2\">%1</a>"_s.arg(homePage, url) + u"<br/><br/>"_s;
    }

    QString support;
    if (mInstalledInfo.isValid() && !mInstalledInfo.support().isEmpty()) {
        support = mInstalledInfo.support();
    } else {
        support = mSupport;
    }

    if (!support.isEmpty()) {
        const QString url = support.startsWith(u"http"_s) ? support : u"mailto://%1"_s.arg(support);
        str += u"<b>%1</b><br/>"_s.arg(i18n("Support")) + u"<a href=\"%2\">%1</a>"_s.arg(support, url) + u"<br/><br/>"_s;
    }

    if (!mPrivacyPolicySummary.isEmpty()) {
        const RuqolaKTextToHTML::Options convertFlags = RuqolaKTextToHTML::HighlightText | RuqolaKTextToHTML::ConvertPhoneNumbers;
        str += u"<b>%1</b><br/>"_s.arg(i18n("Privacy Summary")) + RuqolaKTextToHTML::convertToHtml(mPrivacyPolicySummary, convertFlags) + u"<br/><br/>"_s;
    }

    str += permissionsDescription();
    return str;
}

QString AppsMarketPlaceInfo::permissionsDescription() const
{
    QString str;
    if (!mPermissions.isEmpty()) {
        str = u"<b>%1</b><br/>"_s.arg(i18n("Permissions"));
        str += u"<ol>"_s;
        for (const Permission &p : mPermissions) {
            str += u"<li>%1</li>"_s.arg(p.convertTypeToI18n());
        }
        str += u"</ol>"_s;
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
    return type == other.type;
}

AppsMarketPlaceInfo::Permission::PermissionType AppsMarketPlaceInfo::Permission::convertStringToPermissionType(const QString &str)
{
    if (str == "message.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::MessageWrite;
    } else if (str == "message.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::MessageRead;
    } else if (str == "persistence"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::Persistence;
    } else if (str == "room.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::RoomRead;
    } else if (str == "room.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::RoomWrite;
    } else if (str == "networking"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::Networking;
    } else if (str == "user.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::UserWrite;
    } else if (str == "user.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::UserRead;
    } else if (str == "scheduler"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::Scheduler;
    } else if (str == "slashcommand"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::SlashCommand;
    } else if (str == "api"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::Api;
    } else if (str == "server-setting.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::ServerSettingWrite;
    } else if (str == "server-setting.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::ServerSettingRead;
    } else if (str == "upload.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::UploadWrite;
    } else if (str == "upload.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::UploadRead;
    } else if (str == "cloud.workspace-token"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::CloudWorkspaceToken;
    } else if (str == "env.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::EnvRead;
    } else if (str == "livechat-department.multiple"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatDepartmentMultiple;
    } else if (str == "livechat-department.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatDepartmentRead;
    } else if (str == "livechat-department.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatDepartmentWrite;
    } else if (str == "livechat-room.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatRoomWrite;
    } else if (str == "livechat-room.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatRoomRead;
    } else if (str == "livechat-message.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatMessageWrite;
    } else if (str == "livechat-message.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatMessageRead;
    } else if (str == "livechat-visitor.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatVisitorWrite;
    } else if (str == "livechat-visitor.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatVisitorRead;
    } else if (str == "livechat-status.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatStatusRead;
    } else if (str == "livechat-custom-fields.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatCustomFieldsWrite;
    } else if (str == "ui.interact"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::UiInteract;
    } else if (str == "livechat-message.multiple"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::LiveChatMessageMultiple;
    } else if (str == "threads.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::ThreadsRead;
    } else if (str == "moderation.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::ModerationWrite;
    } else if (str == "moderation.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::ModerationRead;
    } else if (str == "oauth-app.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::OauthAppWrite;
    } else if (str == "oauth-app.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::OauthAppRead;
    } else if (str == "video-conference.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::VideoConferenceWrite;
    } else if (str == "video-conference.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::VideoConferenceRead;
    } else if (str == "video-conference-provider"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::VideoConferenceProvider;
    } else if (str == "ui.registerButtons"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::UiRegistrerButtons;
    } else if (str == "contact.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::ContactRead;
    } else if (str == "contact.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::ContactWrite;
    } else if (str == "email.send"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::EmailSend;
    } else if (str == "role.read"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::RoleRead;
    } else if (str == "role.write"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::RoleWrite;
    } else if (str == "outbound-communication.provide"_L1) {
        return AppsMarketPlaceInfo::Permission::PermissionType::OutboundComms;
    }

    qCWarning(RUQOLA_LOG) << "PermissionType Unknown type " << str;
    return AppsMarketPlaceInfo::Permission::PermissionType::Unknown;
}

QString AppsMarketPlaceInfo::Permission::convertTypeToI18n() const
{
    switch (type) {
    case Permission::PermissionType::Unknown:
        return {};
    case Permission::PermissionType::MessageRead:
        return i18n("Access messages");
    case Permission::PermissionType::MessageWrite:
        return i18n("Send and modify messages");
    case Permission::PermissionType::RoomRead:
        return i18n("Access room information");
    case Permission::PermissionType::RoomWrite:
        return i18n("Create and modify rooms");
    case Permission::PermissionType::Networking:
        return i18n("Access to this server network");
    case Permission::PermissionType::UserRead:
        return i18n("Access user information");
    case Permission::PermissionType::UserWrite:
        return i18n("Modify user information");
    case Permission::PermissionType::SlashCommand:
        return i18n("Register new slash commands");
    case Permission::PermissionType::Scheduler:
        return i18n("Register and maintain scheduled jobs");
    case Permission::PermissionType::Api:
        return i18n("Register new HTTP endpoints");
    case Permission::PermissionType::ServerSettingRead:
        return i18n("Access settings in this server");
    case Permission::PermissionType::ServerSettingWrite:
        return i18n("Modify settings in this server");
    case Permission::PermissionType::UploadWrite:
        return i18n("Upload files to this server");
    case Permission::PermissionType::UploadRead:
        return i18n("Access files uploaded to this server");
    case Permission::PermissionType::CloudWorkspaceToken:
        return i18n("Interact with Cloud Services on behalf of this server");
    case Permission::PermissionType::EnvRead:
        return i18n("Access minimal information about this server environment");
    case Permission::PermissionType::LiveChatDepartmentMultiple:
        return i18n("Access to multiple Livechat departments information");
    case Permission::PermissionType::LiveChatDepartmentRead:
        return i18n("Access Livechat department information");
    case Permission::PermissionType::LiveChatDepartmentWrite:
        return i18n("Modify Livechat department information");
    case Permission::PermissionType::LiveChatRoomWrite:
        return i18n("Modify Livechat room information");
    case Permission::PermissionType::LiveChatRoomRead:
        return i18n("Access Livechat room information");
    case Permission::PermissionType::LiveChatMessageWrite:
        return i18n("Modify Livechat message information");
    case Permission::PermissionType::LiveChatMessageRead:
        return i18n("Access Livechat message information");
    case Permission::PermissionType::LiveChatVisitorWrite:
        return i18n("Modify Livechat visitor information");
    case Permission::PermissionType::LiveChatVisitorRead:
        return i18n("Access Livechat visitor information");
    case Permission::PermissionType::LiveChatStatusRead:
        return i18n("Access Livechat status information");
    case Permission::PermissionType::UiInteract:
        return i18n("Interact with the UI");
    case Permission::PermissionType::Persistence:
        return i18n("Store internal data in the database");
    case Permission::PermissionType::LiveChatCustomFieldsWrite:
        return i18n("Modify Livechat custom field configuration");
    case Permission::PermissionType::LiveChatMessageMultiple:
        break;
    case Permission::PermissionType::ThreadsRead:
        break;
    case Permission::PermissionType::ModerationWrite:
        break;
    case Permission::PermissionType::ModerationRead:
        break;
    case Permission::PermissionType::OauthAppWrite:
        return i18n("Allow to write oauth app settings");
    case Permission::PermissionType::OauthAppRead:
        return i18n("Allow to read oauth app settings");
    case Permission::PermissionType::VideoConferenceWrite:
        break;
    case Permission::PermissionType::VideoConferenceRead:
        break;
    case Permission::PermissionType::VideoConferenceProvider:
        break;
    case Permission::PermissionType::UiRegistrerButtons:
        break;
    case Permission::PermissionType::ContactRead:
        break;
    case Permission::PermissionType::ContactWrite:
        break;
    case Permission::PermissionType::EmailSend:
        return i18n("Allow to send email");
    case Permission::PermissionType::RoleWrite:
        return i18n("Allow to write role");
    case Permission::PermissionType::RoleRead:
        return i18n("Allow to read role");
    case Permission::PermissionType::OutboundComms:
        return i18n("Out Bound Communication");
    }
    qCWarning(RUQOLA_LOG) << " i18n not found for " << int(type);
    return {};
}
