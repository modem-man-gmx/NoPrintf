#include <string>

#include "no_printf.hpp"

using namespace std;

int main()
{
    NoPrintf S1("String 1");
    S1.put(); // just say "String 1"
    S1.clean();

    S1="Stronk";
    S1.set("String"); // replace old value by "String"
    S1.append(" $$"); // append " $$" to "String"
    S1.arg("2"); // ignored, because no $1 in S1 so far.
    S1.put();

    string std_str("std_str");
    NoPrintf S2(std_str);
    S2 += " + " + std_str;
    S2.append( ";" );
    S2.put();

    S1 = S2;
    S1.put();

    NoPrintf Arguments1;
    Arguments1.set("1.) A String:$1 and an integer=$2, $3 are values.").arg("string").arg(-123456789L).arg(123456789ul).put();

    NoPrintf Arguments2("2.) A String:$1 and an integer=$2, $3 are values.");
    Arguments2.arg("string").arg(-123L).arg(456ul).put();

    NoPrintf Arguments3("3.) another ");
    Arguments3.append("String:'$1' and integer=$2 are values too.").arg("STRING").arg(42L).put();

    return 0;
}
