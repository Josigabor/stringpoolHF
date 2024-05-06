/**
 * \file rstring.h
 *
 * RString osztály
 * Valósítsd meg az osztályhoz tartozó leírás alapján a szükséges funkciókat!
 * A függvények implementációját a forrásfájlban add meg!
 *
 * Ezt a fájlt be kell adni (fel kell tölteni) a megoldással.
 */

#ifndef RSTRING_H
#define RSTRING_H

#include <cstdio>
#include "memtrace.h"

class RString {
        char* tomb;
        size_t cap;

  public:
    // Létrehoz a megadott kapacitással egy karaktertömböt
    // A tömb első helyére egy \0-t rak
    RString(size_t capacity);
    // Létrehoz egy karaktertömböt strlen(str) + 1 mérettel
    // majd odamásolja str tartalmát
    RString(const char* str);
    // Létrehoz egy karaktertömböt capacity mérettel,
    // majd odamásolja az str string-et
    // Ha a string nem fér bele, eldobja a neptun-kódodat
    RString(const char* str, size_t capacity);
    // Megadja a string méretét (strlen)
    size_t size() const;
    // Megadja a karaktertömb kapacitását
    size_t capacity() const;
    // Visszaadja a string memóriaterületére mutató pointert
    operator char*() const;
    // Bemásolja a karaktertömbbe a paraméterül kapott
    // karakterláncot, ha nem fér bele, eldobja a neptun kódot
    RString& operator=(const char* str);
    ~RString();
};

#endif
