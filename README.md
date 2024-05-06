# StringPool 

A következő szorgalmi feladatban az erőforrásszűkében gyakran használt ObjectPool mintát fogjuk megvalósítani. Bár a minta általánosságban használható, illetve több nyelv alatt is elérhető valamilyen speciális változata (látsd: ThreadPool, ConnectionPool), mi ezek megvalósítására nem törekszünk, csak String-ek kezelésére fogjuk használni. 

A minta alkalmazására olyan esetben lehet szükség, ha viszonylag gyorsan, sok objektum keletkezik és szűnik meg. Ezek az objektumok létrehozása és felszabadítása pedig lassú is. Az objektumok tekinthetőek bizonyos erőforrásoknak is, amik szűk keresztmetszettel érhetők csak el. 

Általában ezek a következő alap fügvényekkel rendelkeznek:

 - Resource& acquire(): Azon erőforrásokból, amik szabadok a csoportban visszaad egy referenciát és innentől az adott erőforrás foglaltnak minősül. 
 - void release(Resource&): A kliens (hívó fél) visszaadja az objektumot, így az szabaddá válik.
 - Konstruktor, amivel az erőforráscsoport (pool) méretét lehet megadni 

Ugyanakkor a hagyományos "pooling" mintát kiterjesztjük további funkciókkal a String-ek kezelésére szabva: 
 - A konstruktorral nemcsak a pool-ban lévő string objektumok számát megadni, hanem azok kezdő méretét (mekkora legyen a karaktertömb) is. 
 - Egy string objektum elkérésekor megadható, hogy minimum mekkorának kell lennie a karaktertömb kapacitásának (ez nem a string hossza). 
 - Ha nincs megfelelő kapacitású string tömb, akkor az acquire függvény egy új stringet állít elő. 

## RString

Az első feladat egy `RString` nevű osztály elkészítése. Az osztály megvalósítandó metódusait az `rstring.h` fájl tartalmazza. AZ `RString` osztály szabályos `\0`-val lezárt karakterláncokat reprezentál, ugyanakkor a mögöttes karaktertömb kapacitása szabadon megadható.


A `RString` osztály a header fájlban megadott publikus metódusai tovább nem bővíthetőek, ugyanakkor tetszőleges privát adattaggal/metódussal bővíthető. Természetesen gondoskodni kell a megfelelő dinamikus memóriakazelésről. (itt csak elemi memóriakezelés használható)

## StringPool interfész

A `pool.h`-ban az osztályhoz készített kommentezést követve implementáld a StringPool-t! Az osztály megvalósítása során törekedj az egységbezárásra, a hatékony megvalósításra, az osztály publikus interfésze nem bővíthető és nem is módosítható. 

A két `acquire` függvénynél azt a szabad `RString` objektumot kell visszaadni, ami maximális kihasználtságú, azaz:
 - `acquire(size_t cap)` esetén a kért kapacitás (cap) és a visszaadott RString objektum kapacitásának (str_cap) hányadosa maximális: 0 < max(cap/str_cap) <= 1
 - `acquire(const char* str)` esetén a kapott string valós méretének (len+1) és a visszaadott RString objektum kapacitásának (str_cap) hányadosa maximális: 0 < max((len+1)/str_cap) <= 1

*Ha több, azonos kihasználtságú elem is megadható, akkor mindegy melyiket adja vissza. Tanács: érdemes kapacitás szerint rendezetten tárolni az `RString` objektumokat, emiatt hasznos listát használni. (akár STL is)*

## Tiltott elemek

Az `RString` implementálásakor semmilyen STL tároló nem használható. A megoldásnál elemi dinamikus memóriakezelést kell alkalmazni. Ugyanakkor a `StringPool`-nál már használható valamilyen STL vektor vagy lista. Ezt a JPorta egyszerű szövegvizsgálattal ellenőrzi!

Ha csak a header fájlokban dolgozol, akkor azt később visszaautasíthatjuk, a függvények megvalósítására a forrásfájlok valók! **Minden olyan megoldást visszautasíthatunk utólag is, ami nagyon hatékonytalan!** Figyelj ezekre implementáláskor!

## Beadás

Töltsd le a gitlab tárolóból a projektet, használd az `ELKESZULT` makrót a lépésekben való fejlesztéshez! A portálra feltöntendő a `pool.h`, `pool.cpp`, `rstring.h` és `rstring.cpp` fájlok. Bár nagyon sok ObjectPool megvalósítás található különböző weboldalakon, de ez a feladat egészen egyedi! A határidő után egy plágiumellenőrzőt futtatunk le! 
