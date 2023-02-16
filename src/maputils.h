// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#ifndef HARVESTTIMER_QT_CMAKE_MAPUTILS_H
#define HARVESTTIMER_QT_CMAKE_MAPUTILS_H

#include <QMap>
#include <QVector>
#include <QSet>

class MapUtils {
public:
    template<typename T, typename U>
    static void map_insert_or_create_vector(QMap<T, QVector<U>> &map, T key, U vector_value);
};

template<typename T, typename U>
void MapUtils::map_insert_or_create_vector(QMap<T, QVector<U>> &map, T key, U vector_value) {
    typename QMap<T, QVector<U>>::iterator lb{map.find(key)};
    if (lb != map.end()) {
        // key already exists
        lb.value().append(vector_value);
    } else {
        // the key does not exist in the map
        // Use lb as a hint to insert,
        // so it can avoid another lookup
        map.insert(lb, key, {vector_value});
    }
}


#endif //HARVESTTIMER_QT_CMAKE_MAPUTILS_H
