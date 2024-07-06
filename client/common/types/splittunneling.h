#pragma once

#include <filesystem>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include <QVector>

#include "types/enums.h"
#include "utils/ipvalidation.h"
#include "utils/logger.h"

namespace types {


struct SplitTunnelingSettings
{
    SplitTunnelingSettings() = default;

    SplitTunnelingSettings(const QJsonObject &json)
    {
        if (json.contains(kJsonActiveProp) && json[kJsonActiveProp].isBool())
            active = json[kJsonActiveProp].toBool();

        if (json.contains(kJsonModeProp) && json[kJsonModeProp].isDouble())
            mode = static_cast<SPLIT_TUNNELING_MODE>(json[kJsonModeProp].toInt());
    }

    bool active = false;
    SPLIT_TUNNELING_MODE mode = SPLIT_TUNNELING_MODE_EXCLUDE;

    bool operator==(const SplitTunnelingSettings &other) const
    {
        return other.active == active &&
               other.mode == mode;
    }

    bool operator!=(const SplitTunnelingSettings &other) const
    {
        return !(*this == other);
    }

    QJsonObject toJson() const
    {
        QJsonObject json;
        json[kJsonActiveProp] = active;
        json[kJsonModeProp] = static_cast<int>(mode);
        return json;
    }

    friend QDataStream& operator <<(QDataStream &stream, const SplitTunnelingSettings &o)
    {
        stream << versionForSerialization_;
        stream << o.active << o.mode;
        return stream;
    }

    friend QDataStream& operator >>(QDataStream &stream, SplitTunnelingSettings &o)
    {
        quint32 version;
        stream >> version;
        if (version > o.versionForSerialization_) {
            stream.setStatus(QDataStream::ReadCorruptData);
            return stream;
        }
        stream >> o.active >> o.mode;
        return stream;
    }

    friend QDebug operator<<(QDebug dbg, const SplitTunnelingSettings &s)
    {
        QDebugStateSaver saver(dbg);
        dbg.nospace();
        dbg << "{active:" << s.active << "; ";
        dbg << "mode:" << SPLIT_TUNNELING_MODE_toString(s.mode) << "}";
        return dbg;
    }

private:
    static const inline QString kJsonActiveProp = "active";
    static const inline QString kJsonModeProp = "mode";

    static constexpr quint32 versionForSerialization_ = 1;  // should increment the version if the data format is changed
};

struct SplitTunnelingNetworkRoute
{
    SPLIT_TUNNELING_NETWORK_ROUTE_TYPE type = SPLIT_TUNNELING_NETWORK_ROUTE_TYPE_IP;
    QString name;

    SplitTunnelingNetworkRoute() : type(SPLIT_TUNNELING_NETWORK_ROUTE_TYPE_IP) {}

    SplitTunnelingNetworkRoute(const QJsonObject &json)
    {
        if (json.contains(kJsonTypeProp) && json[kJsonTypeProp].isDouble())
            type = static_cast<SPLIT_TUNNELING_NETWORK_ROUTE_TYPE>(json[kJsonTypeProp].toInt());

        if (json.contains(kJsonNameProp) && json[kJsonNameProp].isString())
            name = json[kJsonNameProp].toString();
    }

    bool operator==(const SplitTunnelingNetworkRoute &other) const
    {
        return other.type == type &&
               other.name == name;
    }

    bool operator!=(const SplitTunnelingNetworkRoute &other) const
    {
        return !(*this == other);
    }

    QJsonObject toJson() const
    {
        QJsonObject json;
        json[kJsonTypeProp] = static_cast<int>(type);
        json[kJsonNameProp] = name;
        return json;
    }

    friend QDataStream& operator <<(QDataStream &stream, const SplitTunnelingNetworkRoute &o)
    {
        stream << versionForSerialization_;
        stream << o.type << o.name;
        return stream;
    }

    friend QDataStream& operator >>(QDataStream &stream, SplitTunnelingNetworkRoute &o)
    {
        quint32 version;
        stream >> version;
        if (version > o.versionForSerialization_) {
            stream.setStatus(QDataStream::ReadCorruptData);
            return stream;
        }
        stream >> o.type >> o.name;
        return stream;
    }

    friend QDebug operator<<(QDebug dbg, const SplitTunnelingNetworkRoute &s)
    {
        QDebugStateSaver saver(dbg);
        dbg.nospace();
        dbg << "{type:" << (int)s.type << "; ";
        dbg << "name:" << s.name << "}";
        return dbg;
    }

private:
    static const inline QString kJsonTypeProp = "type";
    static const inline QString kJsonNameProp = "name";

    static constexpr quint32 versionForSerialization_ = 1;  // should increment the version if the data format is changed
};

struct SplitTunnelingApp
{
    SplitTunnelingApp() = default;
    SplitTunnelingApp& operator=(const SplitTunnelingApp&) = default;

    SplitTunnelingApp(const QJsonObject &json)
    {
        if (json.contains(kJsonTypeProp) && json[kJsonTypeProp].isDouble())
            type = static_cast<SPLIT_TUNNELING_APP_TYPE>(json[kJsonTypeProp].toInt());

        if (json.contains(kJsonNameProp) && json[kJsonNameProp].isString())
            name = json[kJsonNameProp].toString();

        if (json.contains(kJsonFullNameProp) && json[kJsonFullNameProp].isString())
            fullName = json[kJsonFullNameProp].toString();

        if (json.contains(kJsonActiveProp) && json[kJsonActiveProp].isBool())
            active = json[kJsonActiveProp].toBool();

        if (json.contains(kJsonIconProp) && json[kJsonIconProp].isString())
            icon = json[kJsonIconProp].toString();
    }

    bool active = false;
    QString fullName;   // path + name
    QString name;
    QString icon;
    SPLIT_TUNNELING_APP_TYPE type = SPLIT_TUNNELING_APP_TYPE_USER;

    bool operator==(const SplitTunnelingApp &other) const
    {
        return other.type == type &&
               other.name == name &&
               other.fullName == fullName &&
               other.active == active &&
               other.icon == icon;
    }

    bool operator!=(const SplitTunnelingApp &other) const
    {
        return !(*this == other);
    }

    QJsonObject toJson() const
    {
        QJsonObject json;
        json[kJsonActiveProp] = active;
        json[kJsonFullNameProp] = fullName;
        json[kJsonNameProp] = name;
        json[kJsonTypeProp] = static_cast<int>(type);
        json[kJsonIconProp] = icon;
        return json;
    }

    friend QDataStream& operator <<(QDataStream &stream, const SplitTunnelingApp &o)
    {
        stream << versionForSerialization_;
        stream << o.type << o.name << o.fullName << o.active << o.icon;
        return stream;
    }

    friend QDataStream& operator >>(QDataStream &stream, SplitTunnelingApp &o)
    {
        quint32 version;
        stream >> version;
        if (version > o.versionForSerialization_) {
            stream.setStatus(QDataStream::ReadCorruptData);
            return stream;
        }
        stream >> o.type >> o.name >> o.fullName >> o.active;

        if (version >= 2) {
            stream >> o.icon;
        }
        return stream;
    }

    friend QDebug operator<<(QDebug dbg, const SplitTunnelingApp &s)
    {
        QDebugStateSaver saver(dbg);
        dbg.nospace();
        dbg << "{type:" << (int)s.type << "; ";
        dbg << "name:" << s.name << "; ";
        dbg << "fullName:" << (s.fullName.isEmpty() ? "empty" : "settled") << "; ";
        dbg << "active:" << s.active << "; ";
        dbg << "icon:" << (s.icon.isEmpty() ? "empty" : "settled") << "}";
        return dbg;
    }

private:
    static const inline QString kJsonActiveProp = "active";
    static const inline QString kJsonFullNameProp = "fullName";
    static const inline QString kJsonNameProp = "name";
    static const inline QString kJsonIconProp = "icon";
    static const inline QString kJsonTypeProp = "type";

    static constexpr quint32 versionForSerialization_ = 2;  // should increment the version if the data format is changed
};


struct SplitTunneling
{
    SplitTunneling() = default;

    SplitTunneling(const QJsonObject &json)
    {
        if (json.contains(kJsonSettingsProp) && json[kJsonSettingsProp].isObject())
            settings = SplitTunnelingSettings(json[kJsonSettingsProp].toObject());

        if (json.contains(kJsonAppsProp) && json[kJsonAppsProp].isArray())
        {
            QJsonArray appsArray = json[kJsonAppsProp].toArray();
            apps.clear();
            apps.reserve(appsArray.size());
            for (const QJsonValue &appValue : appsArray)
            {
                if (appValue.isObject())
                {
                    apps.append(SplitTunnelingApp(appValue.toObject()));
                }
            }
        }

        if (json.contains(kJsonNetworkRoutesProp) && json[kJsonNetworkRoutesProp].isArray())
        {
            QJsonArray networkRoutesArray = json[kJsonNetworkRoutesProp].toArray();
            networkRoutes.clear();
            networkRoutes.reserve(networkRoutesArray.size());
            for (const QJsonValue &routeValue : networkRoutesArray)
            {
                if (routeValue.isObject())
                {
                    networkRoutes.append(SplitTunnelingNetworkRoute(routeValue.toObject()));
                }
            }
        }
    }

    SplitTunnelingSettings settings;
    QVector<SplitTunnelingApp> apps;
    QVector<SplitTunnelingNetworkRoute> networkRoutes;

    bool operator==(const SplitTunneling &other) const
    {
        return other.settings == settings &&
               other.apps == apps &&
               other.networkRoutes == networkRoutes;
    }

    bool operator!=(const SplitTunneling &other) const
    {
        return !(*this == other);
    }

    void fromIni(const QSettings &s)
    {
        settings.active = s.value(kIniSplitTunnelingEnabledProp, settings.active).toBool();
        settings.mode = SPLIT_TUNNELING_MODE_fromString(s.value(kIniSplitTunnelingModeProp, SPLIT_TUNNELING_MODE_toString(settings.mode)).toString());

        if (s.contains(kIniSplitTunnelingAppsProp)) {
            apps.clear();
            QStringList appsList;
            appsList = s.value(kIniSplitTunnelingAppsProp).toStringList();
            for (auto appPath : appsList) {
                std::error_code ec;
                std::filesystem::path path(appPath.toStdString());
                bool exists = std::filesystem::exists(path, ec);
                if (ec || !exists) {
                    qCDebug(LOG_BASIC) << "Skipping non-existent split tunneling app '" << appPath << "'";
                    continue;
                }

                SplitTunnelingApp a;
                a.active = true;
                a.fullName = appPath;
                a.name = appPath;
                a.icon = "";
                a.type = SPLIT_TUNNELING_APP_TYPE_USER;
                apps << a;
            }
        }

        if (s.contains(kIniSplitTunnelingRoutesProp)) {
            networkRoutes.clear();
            QStringList networkRoutesList;
            networkRoutesList = s.value(kIniSplitTunnelingRoutesProp).toStringList();
            for (auto route : networkRoutesList) {
                SplitTunnelingNetworkRoute r;
                r.name = route;
                if (IpValidation::isIp(route)) {
                    r.type = SPLIT_TUNNELING_NETWORK_ROUTE_TYPE_IP;
                } else if (IpValidation::isDomain(route)) {
                    r.type = SPLIT_TUNNELING_NETWORK_ROUTE_TYPE_HOSTNAME;
                } else {
                    qCDebug(LOG_BASIC) << "Skipping unrecognized split tunneling route type";
                    continue;
                }
                networkRoutes << r;
            }
        }
    }

    void toIni(QSettings &s) const
    {
        s.setValue(kIniSplitTunnelingEnabledProp, settings.active);
        s.setValue(kIniSplitTunnelingModeProp, SPLIT_TUNNELING_MODE_toString(settings.mode));

        QStringList appsList;
        for (auto app : apps) {
            appsList << app.fullName;
        }
        if (appsList.isEmpty()) {
            s.remove(kIniSplitTunnelingAppsProp);
        } else {
            s.setValue(kIniSplitTunnelingAppsProp, appsList);
        }

        QStringList routesList;
        for (auto route : networkRoutes) {
            routesList << route.name;
        }
        if (routesList.isEmpty()) {
            s.remove(kIniSplitTunnelingRoutesProp);
        } else {
            s.setValue(kIniSplitTunnelingRoutesProp, routesList);
        }
    }

    QJsonObject toJson() const
    {
        QJsonObject json;
        json[kJsonSettingsProp] = settings.toJson();

        QJsonArray appsArray;
        for (const SplitTunnelingApp& app : apps)
        {
            appsArray.append(app.toJson());
        }
        json[kJsonAppsProp] = appsArray;

        QJsonArray networkRoutesArray;
        for (const SplitTunnelingNetworkRoute& route : networkRoutes)
        {
            networkRoutesArray.append(route.toJson());
        }
        json[kJsonNetworkRoutesProp] = networkRoutesArray;

        json[kJsonVersionProp] = static_cast<int>(versionForSerialization_);

        return json;
    }

    friend QDataStream& operator <<(QDataStream &stream, const SplitTunneling &o)
    {
        stream << versionForSerialization_;
        stream << o.settings << o.apps << o.networkRoutes;
        return stream;
    }

    friend QDataStream& operator >>(QDataStream &stream, SplitTunneling &o)
    {
        quint32 version;
        stream >> version;
        if (version > o.versionForSerialization_) {
            stream.setStatus(QDataStream::ReadCorruptData);
            return stream;
        }
        stream >> o.settings >> o.apps >> o.networkRoutes;
        return stream;
    }

    friend QDebug operator<<(QDebug dbg, const SplitTunneling &s)
    {
        QDebugStateSaver saver(dbg);
        dbg.nospace();
        dbg << "{settings:" << s.settings << "; ";
        dbg << "apps:" << s.apps << "; ";
        dbg << "networkRoutes:" << s.networkRoutes << "}";
        return dbg;
    }

private:
    static const inline QString kIniSplitTunnelingAppsProp = "SplitTunnelingApps";
    static const inline QString kIniSplitTunnelingRoutesProp = "SplitTunnelingRoutes";
    static const inline QString kIniSplitTunnelingEnabledProp = "SplitTunnelingEnabled";
    static const inline QString kIniSplitTunnelingModeProp = "SplitTunnelingMode";

    static const inline QString kJsonAppsProp = "apps";
    static const inline QString kJsonNetworkRoutesProp = "networkRoutes";
    static const inline QString kJsonSettingsProp = "settings";
    static const inline QString kJsonVersionProp = "version";

    static constexpr quint32 versionForSerialization_ = 1;  // should increment the version if the data format is changed
};


} // types namespace
