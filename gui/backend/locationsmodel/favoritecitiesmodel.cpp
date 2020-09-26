#include "favoritecitiesmodel.h"

FavoriteCitiesModel::FavoriteCitiesModel(QObject *parent) : BasicCitiesModel(parent)
{

}

void FavoriteCitiesModel::update(QVector<LocationModelItem *> locations)
{
    clearCities();

    for (const LocationModelItem *lmi : locations)
    {
        if (!lmi->id.isCustomConfigsLocation() && !lmi->id.isStaticIpsLocation() && !lmi->id.isBestLocation())
        {
            for (int i = 0; i < lmi->cities.count(); ++i)
            {
                CityModelItem *cmi = new CityModelItem();
                *cmi = lmi->cities[i];
                cities_ << cmi;
            }
        }
    }

    selectOnlyFavorite();
    sort(favoriteCities_);
    emit itemsUpdated(favoriteCities_);
}

void FavoriteCitiesModel::setOrderLocationsType(ProtoTypes::OrderLocationType orderLocationsType)
{
    orderLocationsType_ = orderLocationsType;
    sort(favoriteCities_);
    emit itemsUpdated(favoriteCities_);
}

void FavoriteCitiesModel::setIsFavorite(const LocationID &id, bool isFavorite)
{
    Q_FOREACH(CityModelItem *cmi, cities_)
    {
        if (cmi->id == id)
        {
            cmi->isFavorite = isFavorite;
            if (isFavorite)
            {
                favoriteCities_ << cmi;
                sort(favoriteCities_);
                emit itemsUpdated(favoriteCities_);
            }
            else
            {
                favoriteCities_.removeOne(cmi);
                emit itemsUpdated(favoriteCities_);
            }
            break;
        }
    }
}

void FavoriteCitiesModel::setFreeSessionStatus(bool isFreeSessionStatus)
{
    if (isFreeSessionStatus != isFreeSessionStatus_)
    {
        BasicCitiesModel::setFreeSessionStatus(isFreeSessionStatus);
        selectOnlyFavorite();
        sort(favoriteCities_);
        emit itemsUpdated(favoriteCities_);
    }
}

void FavoriteCitiesModel::selectOnlyFavorite()
{
    favoriteCities_.clear();

    Q_FOREACH(CityModelItem *cmi, cities_)
    {
        if (cmi->isFavorite && !(cmi->bShowPremiumStarOnly && isFreeSessionStatus_))
        {
            favoriteCities_ << cmi;
        }
    }

}
