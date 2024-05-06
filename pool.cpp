/**
 * \file pool.cpp
 *
 * StringPool oszt�ly implemnt�ci�ja
 * Val�s�tsd meg az oszt�ly met�dusait, a le�r�st a header f�jlban tal�lod.
 *
 * Ezt a f�jlt be kell adni (fel kell t�lteni) a megold�ssal.
 */
#include <list>
#include <algorithm>
#include "memtrace.h"
#include "pool.h"

    // L�trehoz obj_num db RString obejktumot,
    // melyek kapacit�sa init_cap m�ret�
    StringPool::StringPool(size_t obj_num, size_t init_cap){
        used_l = 0;
        not_used_l = obj_num;
        for(size_t i = 0; i<obj_num; i++){
            RString* rstr = new RString(init_cap);
            not_used.push_back(rstr);
        }
    }
    // Visszaad egy szabad RString objektumot ami
    // legal�bb capacity kapacit�ssal rendelkezik
    // azt az objektumot kell visszadni, ami maxim�lis kihaszn�lts�g�
    // Ha nincs ilyen a pool-ban, akkor l�trehoz egyet (capacity kapacit�ssal).
    // Visszavon�sig az a string objektum nem oszhat� ki.
    // "Foglaltnak" min�s�l
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
    // belef�r a param�te�l kapott string, azt bele is m�solja.
    // azt az objektumot kell visszadni, ami maxim�lis kihaszn�lts�g�
    // Ha nincs ilyen a pool-ban, akkor l�trehoz egyet (strlen(str)+1 kapacit�ssal).
    // Visszavon�sig az a string objektum nem oszhat� ki.
    // "Foglaltnak" min�s�l
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
    // visszaadja a pool m�ret�t
    size_t StringPool::size() const{return used_l+not_used_l;}

    // visszaadja a szabad objektumok sz�m�t
    size_t StringPool::free_size() const{return not_used_l;}

    // Visszaadja, hogy az str objektum szabad, azaz
    // kioszthat�/megszerezhet�-e
    bool StringPool::acquireable(const RString& str) const{
            size_t neccesry_cap = str.size();
            std::list<RString*>::const_iterator iter = not_used.begin();
            for(; iter != not_used.end(); iter++){
                if((*iter)->capacity() >= neccesry_cap)
                        return true;
            }
            return false;
    }

    // Ezut�n a visszaadott str objektum �jra kioszthat� m�snak
    // A f�ggv�ny "reseteli" a stringet is: a karaktert�mb els�
    // hely�re '\0'-t rak.
    // Ha az objektum m�r nem foglalt, nem dob hib�t!
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

    // Hozz�f�zi str1 v�g�re str2-t.
    // - Ha str1 kapacit�sa elegend�
    //   akkor str1-ben v�gzi ezt el, �s azt adja vissza
    // - Ha nem, akkor egy �j string-et k�r (acquire), �s abban
    //   f�zi �ssze �ket, majd visszaadja azt
    // Figyelj mit, hogyan kell ny�lv�ntartani! Az str1 �s
    // str2 csak kiosztott er�forr�sok lehetnek, a visszat�r�si
    // �rt�k is szint�n kiosztott.
    // Ha a param�terek nem lefoglalt er�forr�sok,
    // akkor eldobja a neptun k�dodat!
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
