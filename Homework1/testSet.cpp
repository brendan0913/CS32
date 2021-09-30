#include "Set.h"
#include <iostream>
#include <cassert>

// To test a Set of unsigned long, leave the #define line commented out;
// to test a Set of string, remove the "//".
/*
#define TEST_WITH_STRING

#ifdef TEST_WITH_STRING

const ItemType DUMMY_VALUE = "hello";
const ItemType V1 = "abc";

void test()
{
    Set sss;
    assert(sss.empty());
    ItemType d = DUMMY_VALUE;
    assert(!sss.get(0, d) && d == DUMMY_VALUE); // v unchanged by get failure
    sss.insert(V1);
    assert(sss.size() == 1);
    assert(sss.get(0, d) && d == V1);

    Set ss;
    assert(ss.empty());
    assert(ss.size() == 0);
    ss.insert("lavash");
    ss.insert("roti");
    ss.insert("chapati");
    ss.insert("injera");
    ss.insert("roti");
    ss.insert("matzo");
    ss.insert("injera");
    ss.insert("");
    ss.insert("");
    assert(ss.size() == 6);
    assert(!ss.empty());
    ss.erase("injera");
    assert(ss.size() == 5);
    ss.erase("");
    assert(ss.size() == 4);

    Set s;
    s.insert("lavash");
    s.insert("roti");
    s.insert("chapati");
    s.insert("injera");
    s.insert("roti");
    s.insert("matzo");
    s.insert("injera");
    assert(s.size() == 5);  // duplicate "roti" and "injera" were not added
    std::string x;
    s.get(0, x);
    assert(x == "chapati");  // "chapati" is greater than exactly 0 items in ss
    s.get(4, x);
    assert(x == "roti");  // "roti" is greater than exactly 4 items in ss
    s.get(2, x);
    assert(x == "lavash");  // "lavash" is greater than exactly 2 items in ss

    Set s2;
    s2.insert("dosa");
    assert(!s2.contains(""));
    s2.insert("tortilla");
    s2.insert("");
    s2.insert("focaccia");
    assert(s2.contains(""));
    s2.erase("dosa");
    assert(s2.size() == 3 && s2.contains("focaccia") && s2.contains("tortilla") && s2.contains("") && !s2.contains("dosa"));
    std::string v;
    assert(s2.get(1, v) && v == "focaccia");
    assert(s2.get(0, v) && v == "");

    Set n;
    n.insert("");
    n.insert("1");
    n.insert("2");
    n.insert("3");
    n.insert("4");
    std::string num;
    n.get(4, num);
    assert(num == "4");
    n.get(3, num);
    assert(num == "3");
    n.get(2, num);
    assert(num == "2");
    n.get(1, num);
    assert(num == "1");
    n.get(0, num);
    assert(num == "");

    Set ss1;
    ss1.insert("laobing");
    Set ss2;
    ss2.insert("matzo");
    ss2.insert("pita");
    ss1.swap(ss2);

    assert(ss1.size() == 2 && ss1.contains("matzo") && ss1.contains("pita") &&
        ss2.size() == 1 && ss2.contains("laobing"));

    Set set;
    set.insert("dingding");
    set.insert("chingchong");
    set.insert("linglong");
    set.insert("wingwong");
    set.insert("bingbong");
    set.erase("bingbong");

    s2.swap(set);
    assert(set.contains("") && set.contains("focaccia") && set.contains("tortilla") && s2.contains("dingding") && s2.contains("chingchong") && !s2.contains("bingbong"));

    Set ss3;
    assert(ss3.insert("roti"));
    assert(ss3.insert("pita"));
    assert(ss3.size() == 2);
    assert(ss3.contains("pita"));
    ItemType x2 = "laobing";
    assert(ss3.get(0, x2) && x2 == "pita");
    assert(ss3.get(1, x2) && x2 == "roti");

}

#else // assume unsigned long

const ItemType DUMMY_VALUE = 9876543;
const ItemType V1 = 123456789;

void test()
{
    Set ssss;
    assert(ssss.empty());
    ItemType y = 9876543;
    assert(!ssss.get(42, y) && y == 9876543); // x unchanged by get failure
    ssss.insert(123456789);
    assert(ssss.size() == 1);
    assert(ssss.get(0, y) && y == 123456789);

    Set uls;
    assert(uls.insert(20));
    assert(uls.insert(10));
    assert(uls.size() == 2);
    assert(uls.contains(10));
    ItemType x = 30;
    assert(uls.get(0, x) && x == 10);
    assert(uls.get(1, x) && x == 20);

    Set b;      // b can hold at most 5 distinct items
    Set c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
    ItemType v[6] = { 5500,69,99,13,85,50 };

    // No failures inserting 5 distinct items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(v[k]));


    //copy constructor working as intended
    Set d = b;
    ItemType j;
    assert(d.get(0, j) && j == 13);
    assert(d.get(1, j) && j == 69);
    assert(d.get(2, j) && j == 85);
    assert(d.get(3, j) && j == 99);
    assert(d.get(4, j) && j == 5500);
    assert(!d.get(6, j) && j == 5500);
    //assignment operator works as intended
    c = d;
    assert(c.get(0, j) && j == 13);
    assert(c.get(1, j) && j == 69);
    assert(c.get(2, j) && j == 85);
    assert(c.get(3, j) && j == 99);
    assert(c.get(4, j) && j == 5500);
    assert(!c.get(6, j) && j == 5500);
    assert(!c.erase(12));
    c.erase(13);
    assert(c.size() == 4);
}

#endif*/

int main()
{
    //test();
    return 0;
}