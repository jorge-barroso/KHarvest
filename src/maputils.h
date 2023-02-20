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

    template<typename T, typename U>
    static bool map_insert_or_create_set(QMap<T, QSet<U>> &map, T key, U set_value);
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

template<typename T, typename U>
bool MapUtils::map_insert_or_create_set(QMap<T, QSet<U>> &map, T key, U set_value) {
    typename QMap<T, QSet<U>>::iterator lb{map.find(key)};

    if (lb != map.end()) {
        // key already exists
        if(lb.value().contains(set_value)) {
            return false;
        }

        lb.value().insert(set_value);
        return true;
    }

    // the key does not exist in the map
    // Use lb as a hint to insert,
    // so it can avoid another lookup
    return map.insert(lb, key, {set_value}) != map.end();
}


#endif //HARVESTTIMER_QT_CMAKE_MAPUTILS_H
