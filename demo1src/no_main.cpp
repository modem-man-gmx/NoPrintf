#include <string>

#include "no_printf.hpp"

using namespace std; // only for easy demo writing

int main()
{
  NoPrintf S1( "String 1" ); // construct from char*
  S1.put();                  // just say "String 1"
  // can put twice
  S1.put();
  S1.clean();
  // can put again, but empty
  S1.put();

  // can get reassigned
  S1 = "Stronk";      // assign from char*
  S1.set( "String" ); // replace old value by "String"
  S1.append( " $$" ); // append " $$" to "String"
  S1.arg( "1" );      // ignored, because no "$1" placeholder in S1 so far.
  S1.put();
  S1.append( "=$1" ); // append usage of arg 2, but that will gone reset in between
  S1.put();
  // now the arg 2 is alive
  S1.arg( "1" ); // set again as arg 1 ($1) and we have "$1" placeholder in the meantime
  S1.put();

  string   std_str( "std_str" );
  NoPrintf S2( std_str ); // construct from std::string
  S2 += " + " + std_str;
  S2.append( ";" );
  S2.put();

  NoPrintf S3( S1 );       // copy construct
  S3.put();                // must have inherited the argument, too
  S3.append( ", $$2=$2" ); // append usage of arg 2, but that will gone reset in between,
  // so we need to assign all arguments again.
  // this is good, because the other way would make it impossible to manage which number of arg is what
  S3.arg( 1L ).arg( 3L ).put(); // can replace the argument, too

  S1 = S2; // copy assign
  S1.put();

  NoPrintf( "Static $1, $2, $3" ).arg( "erstes" ).arg( "zweites" ).arg( "drittes" ).put();

  NoPrintf Arguments1;
  Arguments1.set( "1.) A String:$1 and an integer=$2, $3 are values." ).arg( "string" ).arg( -123456789L ).arg( 123456789ul ).put();

  NoPrintf Arguments2( "2.) A String:$1 and an integer=$2, $3 are values." );
  Arguments2.arg( "string" ).arg( -123L ).arg( 456ul ).put();

  NoPrintf Arguments3( "3.) another " );
  Arguments3.append( "String:'$1' and integer=$2 are values too." ).arg( "STRING" ).arg( 42L ).put();

  return 0;
}
