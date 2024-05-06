/**
 * \file test.cpp
 *
 * StrinPool nevû szorgalmi feladat tesztjei gtest_lite eszközeivel megvalósítva
 * A szorgalmi feladat megoldásához ezt az állományt nem kell beadni (feltölteni).
 *
 * A ELKESZULT makró vezérli az egyes tesztesetek fordítását, ezzel lehetõvé válik
 * hogy kisebb lépésekben teszteljünk és inkrementálisan készüljön el a program.
 *
 */

#include <iostream>

#include "gtest_lite.h"
#include "memtrace.h"
#include "rstring.h"
#include "pool.h"

#define ELKESZULT 6

/* ELKESZULT makró:
    <= 4: RString tesztjei
    >= 14: Összes teszt
*/

int main() {
    GTINIT(std::cin);
    #if ELKESZULT > 0
        TEST(RString, konstruktorok) {
            RString a(100);
            EXPECT_STREQ("", a);
            EXPECT_EQ(100u, a.capacity());
            EXPECT_EQ(0u, a.size());

            RString b("alma");
            EXPECT_STREQ("alma", b);
            EXPECT_EQ(5u, b.capacity());
            EXPECT_EQ(4u, b.size());

            RString c("alma", 10);
            EXPECT_STREQ("alma", b);
            EXPECT_EQ(10u, c.capacity());
            EXPECT_EQ(4u, c.size());
        } END
    #endif

    #if ELKESZULT > 1
        TEST(RString, konstruktor kivetel) {
            RString a("alma", 5);
            EXPECT_STREQ("alma", a);
            EXPECT_EQ(5u, a.capacity());
            EXPECT_EQ(4u, a.size());

            try {
                EXPECT_THROW_THROW(RString b("alma", 4), const char* p);
            } catch (const char *p) {
            #ifdef CPORTA
                EXPECT_ENVCASEEQ("ORG_ID", p);
            #endif
            }
        } END
    #endif

    #if ELKESZULT > 2
        TEST(RString, ertekado_str) {
            RString a("alma", 20);
            a = "korte";
            EXPECT_STREQ("korte", a);
            EXPECT_EQ(20u, a.capacity());
            EXPECT_EQ(5u, a.size());
            a = "fa";
            EXPECT_STREQ("fa", a);
            EXPECT_EQ(20u, a.capacity());
            EXPECT_EQ(2u, a.size());
        } END
    #endif

    #if ELKESZULT > 3
        TEST(RString, cast) {
            RString a("alma", 20);
            char* str = a;
            strcpy(&str[4], "fa");
            EXPECT_STREQ("almafa", a);
            EXPECT_EQ(20u, a.capacity());
            EXPECT_EQ(6u, a.size());
        } END
    #endif


    #if ELKESZULT > 4
        TEST(StringPool, init) {
            StringPool pool(10, 20);
            EXPECT_EQ(10u, pool.size());
            EXPECT_EQ(10u, pool.free_size());
            RString& ref = pool.acquire(15);
            EXPECT_EQ(20u, ref.capacity());
            EXPECT_STREQ("", ref);
            RString& ref2 = pool.acquire("almafa");
            EXPECT_STREQ("almafa", ref2);
        } END
    #endif


    #if ELKESZULT > 5
        TEST(StringPool, free_size) {
            StringPool pool(2, 20);
            EXPECT_EQ(2u, pool.free_size());
            RString& ref1 = pool.acquire(15);
            EXPECT_EQ(1u, pool.free_size());
            RString& ref2 = pool.acquire(20);
            EXPECT_EQ(0u, pool.free_size());
            EXPECT_EQ(20u, ref1.capacity());
            EXPECT_EQ(20u, ref2.capacity());
        } END
    #endif

    #if ELKESZULT > 6
        TEST(StringPool, create_new_obj) {
            StringPool pool(1, 20);
            RString& ref = pool.acquire(15);
            EXPECT_EQ(20u, ref.capacity());
            EXPECT_EQ(0u, pool.free_size());
            RString& newobj = pool.acquire(10);
            EXPECT_EQ(0u, pool.free_size());
            EXPECT_EQ(2u, pool.size());
            EXPECT_EQ(10u, newobj.capacity());
        } END
    #endif

    #if ELKESZULT > 7
        TEST(StringPool, release) {
            StringPool pool(1, 20);
            RString& ref = pool.acquire(15);
            EXPECT_EQ(1u, pool.size());
            EXPECT_EQ(0u, pool.free_size());
            EXPECT_FALSE(pool.acquireable(ref));
            ref = "alma";
            EXPECT_STREQ("alma", ref);
            pool.release(ref);
            EXPECT_TRUE(pool.acquireable(ref));
            EXPECT_EQ(1u, pool.free_size());
            EXPECT_EQ(1u, pool.size());
            ref = pool.acquire(15);
            EXPECT_STREQ("", ref);
        } END
    #endif

    #if ELKESZULT > 8
        TEST(StringPool, release_new) {
            StringPool pool(2, 15);
            RString& ref = pool.acquire(30);
            EXPECT_EQ(3u, pool.size());
            EXPECT_EQ(30u, ref.capacity());
            EXPECT_EQ(2u, pool.free_size());
            ref = "alma";
            RString& ref2 = pool.acquire(15);
            EXPECT_EQ(3u, pool.size());
            EXPECT_EQ(15u, ref2.capacity());
            EXPECT_EQ(1u, pool.free_size());
            pool.release(ref);
            pool.release(ref2);
            EXPECT_EQ(3u, pool.free_size());
            EXPECT_EQ(3u, pool.size());
            pool.release(ref2);
            EXPECT_EQ(3u, pool.free_size());
            EXPECT_EQ(3u, pool.size());
        } END
    #endif


    #if ELKESZULT > 9
        TEST(StringPool, release_complex) {
            StringPool pool(3, 15);
            RString& ref1 = pool.acquire(15);
            EXPECT_EQ(3u, pool.size());
            EXPECT_EQ(15u, ref1.capacity());
            EXPECT_EQ(2u, pool.free_size());
            RString& ref2 = pool.acquire(30);
            EXPECT_EQ(4u, pool.size());
            EXPECT_EQ(30u, ref2.capacity());
            EXPECT_EQ(2u, pool.free_size());
            pool.release(ref2);
            EXPECT_EQ(3u, pool.free_size());
            EXPECT_EQ(4u, pool.size());
            RString& ref3 = pool.acquire(25);
            EXPECT_EQ(4u, pool.size());
            EXPECT_EQ(30u, ref2.capacity());
            EXPECT_EQ(2u, pool.free_size());
            pool.release(ref3);
            EXPECT_EQ(3u, pool.free_size());
            EXPECT_EQ(4u, pool.size());
        } END
    #endif

    #if ELKESZULT > 10
        TEST(StringPool, max_capacity_check) {
            StringPool pool(1, 10);
            pool.release(pool.acquire(5));
            EXPECT_EQ(1u, pool.free_size());
            EXPECT_EQ(1u, pool.size());
            RString& tmp = pool.acquire(15);
            pool.release(pool.acquire(20));
            pool.release(pool.acquire(25));
            pool.release(tmp);
            EXPECT_EQ(4u, pool.free_size());
            EXPECT_EQ(4u, pool.size());
            RString& ref = pool.acquire(15);
            EXPECT_EQ(3u, pool.free_size());
            EXPECT_EQ(4u, pool.size());
            EXPECT_EQ(15u, ref.capacity());
            RString& ref1 = pool.acquire(15);
            EXPECT_EQ(2u, pool.free_size());
            EXPECT_EQ(4u, pool.size());
            EXPECT_EQ(20u, ref1.capacity());
            RString& ref2 = pool.acquire(15);
            EXPECT_EQ(1u, pool.free_size());
            EXPECT_EQ(4u, pool.size());
            EXPECT_EQ(25u, ref2.capacity());
            RString& ref3 = pool.acquire(15);
            EXPECT_EQ(1u, pool.free_size());
            EXPECT_EQ(5u, pool.size());
            EXPECT_EQ(15u, ref3.capacity());
            pool.release(ref1);
            pool.release(ref2);
            RString& ref4 = pool.acquire(15);
            EXPECT_EQ(2u, pool.free_size());
            EXPECT_EQ(5u, pool.size());
            EXPECT_EQ(20u, ref4.capacity());
        } END
    #endif

    #if ELKESZULT > 11
        TEST(StringPool, append) {
            StringPool pool(3, 15);
            RString& alma = pool.acquire("alma");
            const RString& fa = pool.acquire("fa");
            RString& almafa = pool.append(alma, fa);
            EXPECT_EQ(3u, pool.size());
            EXPECT_EQ(1u, pool.free_size());
            EXPECT_EQ(&almafa, &alma);
            EXPECT_STREQ("almafa", almafa);
        } END
    #endif

    #if ELKESZULT > 12
        TEST(StringPool, append_complex) {
            StringPool pool(3, 15);
            RString& arvizturo = pool.acquire("arvizturo");
            RString& tukorfurogep = pool.acquire("tukorfurogep");
            RString& egyben = pool.append(arvizturo, tukorfurogep);
            EXPECT_EQ(4u, pool.size());
            EXPECT_EQ(1u, pool.free_size());
            EXPECT_NE(&arvizturo, &egyben);
            EXPECT_STREQ("arvizturotukorfurogep", egyben);
        } END
    #endif



    #if ELKESZULT > 13
        TEST(StringPool, append_kivetel) {
            try {
                StringPool pool(3, 15);
                RString& arvizturo = pool.acquire("arvizturo");
                RString& tukorfurogep = pool.acquire("tukorfurogep");
                pool.release(arvizturo);
                EXPECT_THROW_THROW(pool.append(arvizturo, tukorfurogep), const char* p);
            } catch (const char *p) {
            #ifdef CPORTA
                EXPECT_ENVCASEEQ("ORG_ID", p);
            #endif
            }


            try {
                StringPool pool(3, 15);
                RString& arvizturo = pool.acquire("arvizturo");
                RString& tukorfurogep = pool.acquire("tukorfurogep");
                pool.release(tukorfurogep);
                EXPECT_THROW_THROW(pool.append(arvizturo, tukorfurogep), const char* p);
            } catch (const char *p) {
            #ifdef CPORTA
                EXPECT_ENVCASEEQ("ORG_ID", p);
            #endif
            }
        } END
    #endif

    if (ELKESZULT < 14)
      ADD_FAILURE() << "\nNem futott minden teszteset!" << std::endl;

    GTEND(std::cerr);
    return 0;
}
