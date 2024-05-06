/**
 * \file rstring.h
 *
 * StringPool osztály
  * Valósítsd meg az osztályhoz tartozó leírás alapján a szükséges funkciókat!
 * A függvények implementációját a forrásfájlban add meg!
 *
 * Ezt a fájlt be kell adni (fel kell tölteni) a megoldással.
 */

#ifndef POOL_H
#define POOL_H

#include "memtrace.h"
#include "rstring.h"

class StringPool {
    std::list<RString*> used;
    std::list<RString*> not_used;
    size_t used_l;
    size_t not_used_l;
  public:
    // Létrehoz obj_num db RString obejktumot,
    // melyek kapacitása init_cap méretű
    StringPool(size_t obj_num, size_t init_cap);

    // Visszaad egy szabad RString objektumot ami
    // legalább capacity kapacitással rendelkezik
    // azt az objektumot kell visszadni, ami maximális kihasználtságú
    // Ha nincs ilyen a pool-ban, akkor létrehoz egyet (capacity kapacitással).
    // Visszavonásig az a string objektum nem oszható ki.
    // "Foglaltnak" minősül
    RString& acquire(size_t capacity);

    // Visszaad egy szabad RString objektumot amibe
    // belefér a paraméteül kapott string, azt bele is másolja.
    // azt az objektumot kell visszadni, ami maximális kihasználtságú
    // Ha nincs ilyen a pool-ban, akkor létrehoz egyet (strlen(str)+1 kapacitással).
    // Visszavonásig az a string objektum nem oszható ki.
    // "Foglaltnak" minősül
    RString& acquire(const char* str);

    // Visszaadja, hogy az str objektum szabad, azaz
    // kiosztható/megszerezhető-e
    bool acquireable(const RString& str) const;

    // Ezután a visszaadott str objektum újra kiosztható másnak
    // A függvény "reseteli" a stringet is: a karaktertömb első
    // helyére '\0'-t rak.
    // Ha az objektum már nem foglalt, nem dob hibát!
    void release(RString& str);

    // Hozzáfűzi str1 végére str2-t.
    // - Ha str1 kapacitása elegendő
    //   akkor str1-ben végzi ezt el, és azt adja vissza
    // - Ha nem, akkor egy új string-et kér (acquire), és abban
    //   fűzi össze őket, majd visszaadja azt
    // Figyelj mit, hogyan kell nyílvántartani! Az str1 és
    // str2 csak kiosztott erőforrások lehetnek, a visszatérési
    // érték is szintén kiosztott.
    // Ha a paraméterek nem lefoglalt erőforrások,
    // akkor eldobja a neptun kódodat!
    RString& append(RString& str1, const RString& str2);

    // visszaadja a pool méretét
    size_t size() const;

    // visszaadja a szabad objektumok számát
    size_t free_size() const;
    ~StringPool();
};

#endif
