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
        used_l = 0;
        not_used_l = obj_num;
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
                            RString* res= *iter;
                            std::list<RString*>::iterator used_iter = used.begin();
                            for(;used_iter != used.end() && (*used_iter)->capacity() >= res->capacity(); used_iter++);
                            used.insert(used_iter, res);
                            not_used.erase(iter);
                            used_l++;
                            not_used_l--;
                            return *res;
                    }
            }
            RString* rstr = new RString(capacity);
            used_l++;
            used.push_back(rstr);
            return *rstr;
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
                            RString* res= *iter;
                            strcpy(*res, str);
                            std::list<RString*>::iterator used_iter = used.begin();
                            for(;used_iter != used.end() && (*used_iter)->capacity() >= res->capacity(); used_iter++);
                            used.insert(used_iter, res);
                            not_used.erase(iter);
                            used_l++;
                            not_used_l--;
                            return *res;
                    }
            }
            RString* rstr = new RString(strlen(str)+1);
            strcpy(*rstr, str);
            used_l++;
            used.push_back(rstr);
            return *rstr;
    }
    // visszaadja a pool méretét
    size_t StringPool::size() const{return used_l+not_used_l;}

    // visszaadja a szabad objektumok számát
    size_t StringPool::free_size() const{return not_used_l;}

    // Visszaadja, hogy az str objektum szabad, azaz
    // kiosztható/megszerezhetõ-e
    bool StringPool::acquireable(const RString& str) const{
            size_t neccesry_cap = str.size();
            std::list<RString*>::const_iterator iter = not_used.begin();
            for(; iter != not_used.end(); iter++){
                if((*iter)->capacity() >= neccesry_cap)
                        return true;
            }
            return false;
    }

    // Ezután a visszaadott str objektum újra kiosztható másnak
    // A függvény "reseteli" a stringet is: a karaktertömb elsõ
    // helyére '\0'-t rak.
    // Ha az objektum már nem foglalt, nem dob hibát!
    void StringPool::release(RString& str){
            std::list<RString*>::iterator used_iter = used.begin();
            if(used.empty()){
                return;
            }
            for(;used_iter != used.end() && strcmp(*(*used_iter) , str) !=0; used_iter++);
            if(strcmp(*(*used_iter) , str) ==0){
                    std::cout<<"fuck"<<std::endl;
                    std::list<RString*>::iterator iter = not_used.begin();
                    std::cout<<"fuck"<<std::endl;
                    for(;iter != not_used.end(); iter++){
                        str[0] = '\0';
                        if((*iter)->capacity() >= (*used_iter)->capacity()){
                            not_used.insert(iter, &str);
                            used.erase(used_iter);
                            used_l--;
                            not_used_l++;
                            return;
                        }
                    }
            }
//                if(!acquireable(str)){
//                    str[0]='\0';
//                    used.remove(&str);
//                    used_l--;
//                    std::list<RString*>::iterator i;
//                    for(i=not_used.begin();i!=not_used.end();i++){
//                            if((*i)->capacity()>=str.capacity()){
//                                not_used.insert(i, &str);
//                                not_used_l++;
//                                return;
//                            }
//                    }
//                    not_used_l++;
//                    not_used.push_back(&str);
//                }
//                return;

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
            if(used.size()<2)
                throw "BQESH6";
            bool str1_found = false;
            bool str2_found = false;
            std::list<RString*>::iterator iter1 = used.begin();
            std::list<RString*>::iterator iter2 = used.begin();
            for(;iter1 != used.end() && strcmp(*(*iter1) , str1) !=0; iter1++);
            for(;iter2 != used.end() && strcmp(*(*iter2) , str2) !=0; iter2++);
            if(strcmp(*(*iter1) , str1) ==0)
                str1_found = true;
            if(strcmp(*(*iter2) , str2) ==0)
                str2_found = true;
            if(str1_found && str2_found){
                if(((int)(*iter1)->capacity()-(int)(*iter1)->size()-(int)(*iter2)->size()) >= 0){
                    *(*iter1) = strcat(*(*iter1), *(*iter2));
                    return *(*iter1);
                }else{
                    if(this->free_size() == 0)
                        throw "BQESH6";
                    RString& res = this->acquire(strcat(*(*iter1), *(*iter2)));
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
            std::cout<<used_l<<std::endl;
            for(iter1 = not_used.begin();iter1 != not_used.end(); iter1++){
                delete *iter1;
                std::cout<<"dtor";
            }

    }
