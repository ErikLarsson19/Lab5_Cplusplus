//DEL == 1 för första labben, == 2 för andra labben
#include "TestLevel.h"

#if DEL == 1 
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#include <crtdbg.h>
#endif DEL

#include "Vector.hpp"

#include <type_traits>
#include <iostream>
using std::cout;
#include <cassert>

void TestBasic();
void TestVector();
void TestIter();

int main() {
#if DEL == 1
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif DEL
    std::locale::global(std::locale("swedish"));

    std::cout <<
        "Ändra i TestLevel.h för att kontrollera vad som testas\n"
        "Gå stegvis upp med LEVEL från 1 så långt det behövs,  ändra den sen till 99\n"
        "Och kontrollera vilket betyg som testas genom att definiera VG_BETYG eller G_BETYG\n"
        "Change the include of Vector.hpp to your own file\n\n"
        ;
    std::cout << "Testing Vector del " << DEL << "\n";
    std::cout << "Selected is " <<
#ifdef VG_BETYG
        "VG"
#else
#ifdef G_BETYG
        "G"
#else
        "????"
#endif
#endif
        << ", Level is: " << LEVEL << "\n\n";

    TestBasic();
    TestVector();
    TestIter();

#if DEL == 1
    new int; cout << "det finns en minnesläcka i main, avsiktligt!\n så ni kan se att er minnesläckstest fungerar\n";
#else
    //Dalloc<double>().allocate(1); cout << "det finns en minnesläcka i main, avsiktligt!\n så ni kan se att er minnesläckstest fungerar\n press ENTER to continue ";
#endif DEL
    std::cin.get();
}
