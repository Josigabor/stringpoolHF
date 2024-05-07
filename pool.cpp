/**
 * \file pool.cpp
 *
 * StringPool osztály implemntációja
 * Valósítsd meg az osztály metódusait, a leírást a header fájlban találod.
 *
 * Ezt a fájlt be kell adni (fel kell tölteni) a megoldással.
 */
#include <list>
#include <algorithm>
#include "memtrace.h"
#include "pool.h"

    // Létrehoz obj_num db RString obejktumot,
    // melyek kapacitása init_cap méretû
    StringPool::StringPool(size_t obj_num, size_t init_cap){
        for(size_t i = 0; i<obj_num; i++){
            RString* rstr = new RString(init_cap);
            not_used.push_back(rstr);
        }
    }
    // Visszaad egy szabad RString objektumot ami
    // legalább capacity kapacitással rendelkezik
    // azt az objektumot kell visszadni, ami maximális kihasználtságú
    // Ha nincs ilyen a pool-ban, akkor létrehoz egyet (capacity kapacitással).
    // Visszavonásig az a string objektum nem oszható ki.
    // "Foglaltnak" minõsül
    RString& StringPool::acquire(size_t capacity){
            std::list<RString*>::iterator iter = not_used.begin();
            for(; iter != not_used.end(); iter++){
                    if((*iter)->capacity()>=capacity){
                            used.push_back(*iter);
                            not_used.erase(iter);
                            return *used.back();
                    }
            }
            RString* rstr = new RString(capacity);
            used.push_back(rstr);
            return *used.back();
    }
    // Visszaad egy szabad RString objektumot amibe
    // belefér a paraméteül kapott string, azt bele is másolja.
    // azt az objektumot kell visszadni, ami maximális kihasználtságú
    // Ha nincs ilyen a pool-ban, akkor létrehoz egyet (strlen(str)+1 kapacitással).
    // Visszavonásig az a string objektum nem oszható ki.
    // "Foglaltnak" minõsül
    RString& StringPool::acquire(const char* str){
            std::list<RString*>::iterator iter = not_used.begin();
            for(; iter != not_used.end(); iter++){
                    if((*iter)->capacity()>=strlen(str)+1){
                            strcpy(*(*iter), str);
                            used.push_back(*iter);
                            not_used.erase(iter);
                            return *used.back();
                    }
            }
            RString* rstr = new RString(str);
            used.push_back(rstr);
            return *used.back();
    }
    // visszaadja a pool méretét
    size_t StringPool::size() const{
            size_t i = this->free_size();
            std::list<RString*>::const_iterator iter = used.begin();
            for(; iter != used.end(); iter++){i++;};
            return i;
    }

    // visszaadja a szabad objektumok számát
    size_t StringPool::free_size() const{
            size_t i = 0;
            std::list<RString*>::const_iterator iter = not_used.begin();
            for(; iter != not_used.end(); iter++){i++;};
            return i;
    }

    // Visszaadja, hogy az str objektum szabad, azaz
    // kiosztható/megszerezhetõ-e
    bool StringPool::acquireable(const RString& str) const{
            std::list<RString*>::const_iterator iter = not_used.begin();
            for(; iter != not_used.end(); iter++){
                if(*iter == &str)
                        return true;
            }
            return false;
    }

    // Ezután a visszaadott str objektum újra kiosztható másnak
    // A függvény "reseteli" a stringet is: a karaktertömb elsõ
    // helyére '\0'-t rak.
    // Ha az objektum már nem foglalt, nem dob hibát!
    void StringPool::release(RString& str){
                std::list<RString*>::iterator iter = not_used.begin();
                if(!acquireable(str)){
                    used.remove(&str);
                    for(;iter != not_used.end();iter++){
                        if((*iter)->capacity()>=str.capacity()){
                                str[0] = '\0';
                                not_used.insert(iter, &str);
                                return;
                        }
                    }
                    str[0] = '\0';
                    not_used.push_back(&str);
                    return;
                }



    }

    // Hozzáfûzi str1 végére str2-t.
    // - Ha str1 kapacitása elegendõ
    //   akkor str1-ben végzi ezt el, és azt adja vissza
    // - Ha nem, akkor egy új string-et kér (acquire), és abban
    //   fûzi össze õket, majd visszaadja azt
    // Figyelj mit, hogyan kell nyílvántartani! Az str1 és
    // str2 csak kiosztott erõforrások lehetnek, a visszatérési
    // érték is szintén kiosztott.
    // Ha a paraméterek nem lefoglalt erõforrások,
    // akkor eldobja a neptun kódodat!
    RString& StringPool::append(RString& str1, const RString& str2){
            std::list<RString*>::iterator iter1 = used.begin();
            std::list<RString*>::iterator iter2 = used.begin();
            for(;iter1 != used.end() && &str1 != (*iter1); iter1++);
            for(;iter2 != used.end() && (*iter2) != &str2; iter2++);
            if(&str1 == (*iter1) && (*iter2) == &str2){
                if(((int)(*iter1)->capacity()-(int)(*iter1)->size()-(int)(*iter2)->size()) >= 0){
                    *(*iter1) = strcat(*(*iter1), *(*iter2));
                    return *(*iter1);
                }else{
                    RString& res = this->acquire(str1.size() + str2.size()+1);
                    strcat(res, str1);
                    strcat(res, str2);
                    return res;
                }
            }else{throw "BQESH6";}
    }

    StringPool::~StringPool(){
//            used.clear();
//            not_used.clear();
            std::list<RString*>::iterator iter1;
            for(iter1 = used.begin();iter1 != used.end(); iter1++){
                delete *iter1;
            }
            for(iter1 = not_used.begin();iter1 != not_used.end(); iter1++){
                delete *iter1;
            }

    }
