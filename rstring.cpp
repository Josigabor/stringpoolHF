/**
 * \file rstring.cpp
 *
 * RString osztály implemntációja
 * Valósítsd meg az osztály metódusait, a leírást a header fájlban találod.
 *
 * Ezt a fájlt be kell adni (fel kell tölteni) a megoldással.
 */

#include "memtrace.h"
#include "rstring.h"

    // Létrehoz a megadott kapacitással egy karaktertömböt
    // A tömb első helyére egy \0-t rak
    RString::RString(size_t capacity){
            cap = capacity;
            tomb = new char[capacity];
            tomb[0] = '\0';
    }
    // Létrehoz egy karaktertömböt strlen(str) + 1 mérettel
    // majd odamásolja str tartalmát
    RString::RString(const char* str){
            cap = strlen(str)+1;
            tomb = new char[cap];
            strcpy(tomb, str);
    }
    // Létrehoz egy karaktertömböt capacity mérettel,
    // majd odamásolja az str string-et
    // Ha a string nem fér bele, eldobja a neptun-kódodat
    RString::RString(const char* str, size_t capacity){
            if(capacity<=strlen(str))
                throw "BQESH6";
            cap = capacity;
            tomb = new char[cap+1];
            strcpy(tomb,str);

    }
    // Megadja a string méretét (strlen)
    size_t RString::size() const{return strlen(tomb);}
    // Megadja a karaktertömb kapacitását
    size_t RString::capacity() const{return cap;}
    // Visszaadja a string memóriaterületére mutató pointert
    RString::operator char*() const{return tomb;}
    // Bemásolja a karaktertömbbe a paraméterül kapott
    // karakterláncot, ha nem fér bele, eldobja a neptun kódot
    RString& RString::operator=(const char* str){
            if(cap<strlen(str))
                throw "BQESH6";
            strcpy(tomb, str);

            return *this;
    }
    RString::~RString(){
            delete[] tomb;
    }
