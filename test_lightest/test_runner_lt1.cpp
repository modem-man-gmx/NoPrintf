#include <limits>
#include <sstream>
#include <string>

#include "lightest/lightest.h"
#include "no_printf.hpp"
//#define __FILE__ "test.cpp"

#ifndef DIM
#  define DIM( a ) ( sizeof( a ) / sizeof( a[ 0 ] ) )
#endif


CONFIG( Configurations )
{
  for( ; argn > 0; argn--, argc++ )
  {
    if( std::string( *argc ) == "--no-color" ) NO_COLOR();
    if( std::string( *argc ) == "--no-output" ) NO_OUTPUT();
  }
  // NO_COLOR();
  // NO_OUTPUT();
}

/*
TEST(TestTimerMacros)
{
  int i = 0;
  REQ(TIMER(i++), >=, 0);  // Run once and measure the time
  REQ(AVG_TIMER(i++, 100), >=, 0);  // Run several times and return the average time
}
*/

TEST( Handling_Check )
{
  int a = std::numeric_limits<int>::min();
  int b = std::numeric_limits<int>::max();

  SUB( Align_default_no_filling )
  {
    char Required[ 128 ];
    snprintf( Required, DIM( Required ), "A) Sml_a=%d, Big_g=%d.", a, b );
    Required[ DIM( Required ) - 1 ] = '\0';
    REQ( NoPrintf( "A) Sml_a=$1, Big_g=$2." ).arg( a ).arg( b ).get(), ==, std::string( Required ) );
  };
};

TEST( Default_NoPrintf_handling )
{
  NoPrintf Default;
  MUST( REQ( Default.get(), ==, std::string() ) ); // clang-format off
  REQ( Default.get(), ==, std::string( "" ) );
  REQ( *Default.get().c_str(), ==, '\0' );
} // end-of TEST(Default_NoPrintf_handling)

TEST( Can_create_Empty_Strings )
{
  SUB( EmptyString )
  {
    NoPrintf EmptyString( "" );
    REQ( EmptyString.get(), ==, std::string() );
    REQ( EmptyString.get(), ==, std::string( "" ) );
    REQ( *EmptyString.get().c_str(), ==, '\0' );
    REQ( EmptyString.get().c_str(), !=, static_cast<const char*>( nullptr ) );
  };

  SUB( EmptyString_ignores_args )
  {
    NoPrintf EmptyString( "" );
    EmptyString.arg( "invisible" ).arg( 0L );
    REQ( EmptyString.get(), ==, std::string() );
    REQ( EmptyString.get(), ==, std::string( "" ) );
    REQ( *EmptyString.get().c_str(), ==, '\0' );
    REQ( EmptyString.get().c_str(), !=, static_cast<const char*>( nullptr ) );
  };

  SUB( Anonymous_default_is_empty )
  {
    REQ( NoPrintf().get(), ==, std::string() );
    REQ( NoPrintf().get(), ==, std::string( "" ) );
    REQ( *NoPrintf().get().c_str(), ==, '\0' );
    REQ( NoPrintf().get().c_str(), !=, static_cast<const char*>( nullptr ) );
  };

  SUB( Anonymous_empty_is_empty )
  {
    const char* nix = "\0";
    std::string leer( "" );
    std::string deflt;
    REQ( NoPrintf( "" ).get(), ==, std::string() );
    REQ( NoPrintf( nix ).get(), ==, std::string( "" ) );
    REQ( *NoPrintf( leer ).get().c_str(), ==, '\0' );
    REQ( NoPrintf( deflt ).get().c_str(), !=, static_cast<const char*>( nullptr ) );
  };

  SUB( Anonymous_empty_ignores_args )
  {
    REQ( NoPrintf().arg( "invisible" ).arg( 0L ).get(), ==, std::string() );
    REQ( NoPrintf().arg( "invisible" ).arg( 0L ).get(), ==, std::string( "" ) );
    REQ( *NoPrintf().arg( "invisible" ).arg( 0L ).get().c_str(), ==, '\0' );
    REQ( NoPrintf().arg( "invisible" ).arg( 0L ).get().c_str(), !=, static_cast<const char*>( nullptr ) );
  };

}; // end-of TEST(Can_create_Empty_Strings)


TEST( Handling_bad_placeholders )
{
  SUB( detect_missing_holders1 )
  {
    const char Required[] = "Bad_1=<invalid>?";
    NoPrintf Actual( "Bad_1=$1?" ); // ignore needed Actual.arg(123);
    REQ( Actual.get(), ==, std::string( Required ) );
  };

  SUB( detect_missing_holders1_2_3 )
  {
    const char Required[] = "Bad_1=<invalid>, bad2=<invalid>, more bad=<invalid>?";
    NoPrintf Actual( "Bad_1=$1, bad2=$2, more bad=$3?" ); // ignore needed Actual.arg(123);
    REQ( Actual.get(), ==, std::string( Required ) );
  };

  SUB( detect_missing_holders2_to_3 )
  {
    const char Required[] = "Bad_1=123, bad2=<invalid>, more bad=<invalid>?";
    NoPrintf Actual( "Bad_1=$1, bad2=$2, more bad=$3?" );
    Actual.arg(123);
    REQ( Actual.get(), ==, std::string( Required ) );
  };

  SUB( detect_jumped_holder9 )
  {
    const char Required[] = "Bad_1=123, bad2=<invalid>, good_again=456!";
    const char Assumed[] = "Bad_1=123, bad2=456, good_again=789!";
    NoPrintf Actual( "Bad_1=$1, bad2=$9, good_again=$2!" );
    Actual.arg(123).arg(456).arg(789);
    REQ( Actual.get(), !=, std::string( Assumed ) );
    REQ( Actual.get(), ==, std::string( Required ) );
  };
};


TEST( Handling_Numbers_in_DotArg )
{
  SUB( Numerical_8bit_int )
  {
    char              a = 0, b = 1, c = -1;
    char              Topic = 'a';
    std::stringstream Required;

    Required << Topic << ") ShortShort A=" << static_cast<short>( a ) << ", B=" << static_cast<short>( b ) << ", C=" << static_cast<short>( c ) << ".";
    REQ( NoPrintf( "a) ShortShort A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );
    Topic++;

    // 8 bit unsigned values are likely not fit
    if( std::numeric_limits<char>::max() == std::numeric_limits<uint8_t>::max()
     && std::numeric_limits<char>::min() == std::numeric_limits<uint8_t>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<uint8_t>::min();
      b = std::numeric_limits<uint8_t>::max();
      Required << Topic << ") ShortShort A=" << static_cast<short>( a ) << ", B=" << static_cast<short>( b ) << ".";
      REQ( NoPrintf( "b) ShortShort A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = std::numeric_limits<int8_t>::min();
    b = std::numeric_limits<int8_t>::max();
    Required << Topic << ") ShortShort A=" << static_cast<short>( a ) << ", B=" << static_cast<short>( b ) << ".";
    REQ( NoPrintf( "c) ShortShort A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    //Topic++;
  };

  SUB( Numerical_short )
  {
    short             a = 0, b = 1, c = -1;
    char              Topic = 'a';
    std::stringstream Required;

    Required << Topic << ") Short A=" << a << ", B=" << b << ", C=" << c << ".";
    REQ( NoPrintf( "a) Short A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );
    Topic++;

    // 8 bit unsigned values must fit
    Required.str( std::string() );
    a = std::numeric_limits<uint8_t>::min();
    b = std::numeric_limits<uint8_t>::max();
    Required << Topic << ") Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = std::numeric_limits<int8_t>::min();
    b = std::numeric_limits<int8_t>::max();
    Required << Topic << ") Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // 16 bit unsigned values could perhaps fit
    if( std::numeric_limits<short>::max() >= std::numeric_limits<uint16_t>::max()
     && std::numeric_limits<short>::min() <= std::numeric_limits<uint16_t>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<uint16_t>::min();
      b = std::numeric_limits<uint16_t>::max();
      Required << Topic << ") Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "d) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = std::numeric_limits<int16_t>::min();
    b = std::numeric_limits<int16_t>::max();
    Required << Topic << ") Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // 32 bit unsigned values will likely not fit
    if( std::numeric_limits<short>::max() >= std::numeric_limits<uint32_t>::max()
     && std::numeric_limits<short>::min() <= std::numeric_limits<uint32_t>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<uint32_t>::min();
      b = std::numeric_limits<uint32_t>::max();
      Required << Topic << ") Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "f) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // 32 bit signed values will likely not fit
    if( std::numeric_limits<short>::max() >= std::numeric_limits<int32_t>::max()
     && std::numeric_limits<short>::min() <= std::numeric_limits<int32_t>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<int32_t>::min();
      b = std::numeric_limits<int32_t>::max();
      Required << Topic << ") Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "g) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // unsigned short values likely not fit in short, because short == signed short
    if( std::numeric_limits<unsigned short>::max() >= std::numeric_limits<int32_t>::max() &&
        std::numeric_limits<unsigned short>::min() <= std::numeric_limits<int32_t>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<unsigned short>::min();
      b = std::numeric_limits<unsigned short>::max();
      Required << Topic << ") Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "h) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // signed short values must fit
    Required.str( std::string() );
    a = std::numeric_limits<signed short>::min();
    b = std::numeric_limits<signed short>::max();
    Required << Topic << ") Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // a (singned) short value can not hold an unsigned int
    if( std::numeric_limits<short>::max() >= std::numeric_limits<unsigned int>::max() &&
        std::numeric_limits<short>::min() <= std::numeric_limits<unsigned int>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<unsigned int>::min();
      b = std::numeric_limits<unsigned int>::max();
      Required << Topic << ") Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "j) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // a (singned) short value can likely not hold an signed int
    if( std::numeric_limits<short>::max() >= std::numeric_limits<signed int>::max() &&
        std::numeric_limits<short>::min() <= std::numeric_limits<signed int>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<signed int>::min();
      b = std::numeric_limits<signed int>::max();
      Required << Topic << ") Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "k) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // unspecified (compiler preset) int values can fit it short == int
    if( std::numeric_limits<short>::max() >= std::numeric_limits<int>::max()
     && std::numeric_limits<short>::min() <= std::numeric_limits<int>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<int>::min();
      b = std::numeric_limits<int>::max();
      Required << Topic << ") Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "l) Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // no knowledge, if PlatformIO ESP8266 supports uint64_t / long long and what exactly long is, compared to int (short < int <= long)
  };


  SUB( Numerical_integer )
  {
    long              a = 0, b = 1, c = -1;
    char              Topic = 'a';
    std::stringstream Required;

    Required << Topic << ") Integer A=" << a << ", B=" << b << ", C=" << c << ".";
    REQ( NoPrintf( "a) Integer A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );

    // 8 bit unsigned values must fit
    Required.str( std::string() );
    a = std::numeric_limits<uint8_t>::min();
    b = std::numeric_limits<uint8_t>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = std::numeric_limits<int8_t>::min();
    b = std::numeric_limits<int8_t>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit unsigned values must fit
    Required.str( std::string() );
    a = std::numeric_limits<uint16_t>::min();
    b = std::numeric_limits<uint16_t>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "d) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = std::numeric_limits<int16_t>::min();
    b = std::numeric_limits<int16_t>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // bit problematic test here, must be made CPU dependent
    // 32 bit unsigned values could fit
    Required.str( std::string() );
    a = std::numeric_limits<uint32_t>::min();
    b = std::numeric_limits<uint32_t>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "f) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 32 bit signed values could fit
    Required.str( std::string() );
    a = std::numeric_limits<int32_t>::min();
    b = std::numeric_limits<int32_t>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "g) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unsigned short values must fit
    Required.str( std::string() );
    a = std::numeric_limits<unsigned short>::min();
    b = std::numeric_limits<unsigned short>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "h) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // signed short values must fit
    Required.str( std::string() );
    a = std::numeric_limits<signed short>::min();
    b = std::numeric_limits<signed short>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    if( std::numeric_limits<int>::max() == std::numeric_limits<unsigned int>::max() &&
        std::numeric_limits<int>::min() == std::numeric_limits<unsigned int>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<unsigned int>::min();
      b = std::numeric_limits<unsigned int>::max();
      REQ( a, <, b );
      Required << Topic << ") Integer A=" << a << ", B=" << b << ".";
      // if int is unsigned (unlikely) unsigned int values must fit
      REQ( NoPrintf( "j) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // signed int values must fit
    Required.str( std::string() );
    a = std::numeric_limits<signed int>::min();
    b = std::numeric_limits<signed int>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "k) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unspecified (compiler preset) int values must fit
    Required.str( std::string() );
    a = std::numeric_limits<int>::min();
    b = std::numeric_limits<int>::max();
    Required << ++Topic << ") Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "l) Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // no knowledge, if PlatformIO ESP8266 supports uint64_t / long long and what exactly long is, compared to int (short < int <= long)
  };


  SUB( Numerical_long )
  {
    long              a = 0L, b = 1L, c = -1L;
    char              Topic = 'a';
    std::stringstream Required;

    Required << Topic << ") Long A=" << a << ", B=" << b << ", C=" << c << ".";
    REQ( NoPrintf( "a) Long A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );

    // 8 bit unsigned values must fit
    Required.str( std::string() );
    a = std::numeric_limits<uint8_t>::min();
    b = std::numeric_limits<uint8_t>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = std::numeric_limits<int8_t>::min();
    b = std::numeric_limits<int8_t>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit unsigned values must fit
    Required.str( std::string() );
    a = std::numeric_limits<uint16_t>::min();
    b = std::numeric_limits<uint16_t>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "d) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = std::numeric_limits<int16_t>::min();
    b = std::numeric_limits<int16_t>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // bit problematic test here, must be made CPU dependent
    // 32 bit unsigned values could fit
    Required.str( std::string() );
    a = std::numeric_limits<uint32_t>::min();
    b = std::numeric_limits<uint32_t>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "f) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 32 bit signed values could fit
    Required.str( std::string() );
    a = std::numeric_limits<int32_t>::min();
    b = std::numeric_limits<int32_t>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "g) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unsigned short values must fit
    Required.str( std::string() );
    a = std::numeric_limits<unsigned short>::min();
    b = std::numeric_limits<unsigned short>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "h) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // signed short values must fit
    Required.str( std::string() );
    a = std::numeric_limits<signed short>::min();
    b = std::numeric_limits<signed short>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    Required.str( std::string() );
    a = std::numeric_limits<unsigned int>::min();
    b = std::numeric_limits<unsigned int>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    // if int is unsigned (unlikely) unsigned int values must fit
    REQ( NoPrintf( "j) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // signed int values must fit
    Required.str( std::string() );
    a = std::numeric_limits<signed int>::min();
    b = std::numeric_limits<signed int>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "k) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unspecified (compiler preset) int values must fit
    Required.str( std::string() );
    a = std::numeric_limits<int>::min();
    b = std::numeric_limits<int>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "l) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    if( std::numeric_limits<long>::max() == std::numeric_limits<unsigned long>::max() &&
        std::numeric_limits<long>::min() == std::numeric_limits<unsigned long>::min() )
    {
      Required.str( std::string() );
      a = std::numeric_limits<unsigned long>::min();
      b = std::numeric_limits<unsigned long>::max();
      REQ( a, <, b );
      Required << Topic << ") Long A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "m) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    Required.str( std::string() );
    a = std::numeric_limits<signed long>::min();
    b = std::numeric_limits<signed long>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "n) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unspecified long is for sure signed long
    Required.str( std::string() );
    a = std::numeric_limits<long>::min();
    b = std::numeric_limits<long>::max();
    Required << ++Topic << ") Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "o) Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // no knowledge, if PlatformIO ESP8266 supports uint64_t / long long and what exactly long is, compared to int (short < int <= long)
  };

  SUB( Numerical_unsigned_long )
  {
    unsigned long     a = 0, b = 1, c = std::numeric_limits<signed long>::max();
    char              Topic = 'a';
    std::stringstream Required;

    Required << Topic << ") Long A=" << a << ", B=" << b << ", C=" << c << ".";
    REQ( NoPrintf( "a) Long A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );
  };

}; // end-of TEST(Handling_Numbers_in_DotArg)

/* a test does not work like a SUB
TEST( Check_Some_printf )
{
  int c = -1;
  int d = 0;
  char Required[128];

  SUB( Align_default )
  {
    snprintf( Required, DIM(Required), "A) Minus1_c=%d, Zero_d=%d.", c, d );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( std::string( Required ), ==, std::string( "A) Minus1_c=-1, Zero_d=0." ) );
  };
}
*/


TEST( Handling_aligned_numbers )
{
  int a = std::numeric_limits<int>::min();
  int b = -654321;
  int c = -1;
  int d = 0;
  int e = 1;
  int f = 1234567;
  int g = std::numeric_limits<int>::max();
  /*
  std::cout << "int32_t min() : " << std::numeric_limits<int32_t>::min() << std::endl;
  std::cout << "int32_t max() : " << std::numeric_limits<int32_t>::max() << std::endl;
  std::cout << "int     min() : " << std::numeric_limits<int>::min() << std::endl;
  std::cout << "int     max() : " << std::numeric_limits<int>::max() << std::endl;
  std::cout << "signed  min() : " << std::numeric_limits<signed int>::min() << std::endl;
  std::cout << "signed  max() : " << std::numeric_limits<signed int>::max() << std::endl;
  std::cout << "long    min() : " << std::numeric_limits<long>::min() << std::endl;
  std::cout << "long    max() : " << std::numeric_limits<long>::max() << std::endl;
  */
  SUB( Align_default_no_filling )
  {
    char Required[128];
    snprintf( Required, DIM(Required), "A) Sml_a=%d, Minus_b=%d, Minus1_c=%d, Zero_d=%d, One_e=%d, F_f=%d, Big_g=%d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "A) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a ).arg( b ).arg(c).arg(d).arg(e).arg(f).arg(g).get(), ==, std::string( Required ) );
  };

  SUB( Align_0_no_filling_zero_dropped )
  {
    char Required[128];
    snprintf( Required, DIM(Required), "B) Sml_a=%.0d, Minus_b=%.0d, Minus1_c=%.0d, Zero_d=%.0d, One_e=%.0d, F_f=%.0d, Big_g=%.0d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "B) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a,0 ).arg( b,0 ).arg(c,0).arg(d,0).arg(e,0).arg(f,0).arg(g,0).get(), ==, std::string( Required ) );
  };

  SUB( Align_minus1_has_no_effect )
  {
    char Required[128];
    snprintf( Required, DIM(Required), "C) Sml_a=%-1d, Minus_b=%-1d, Minus1_c=%-1d, Zero_d=%-1d, One_e=%-1d, F_f=%-1d, Big_g=%-1d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "C) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a,-1 ).arg( b,-1 ).arg(c,-1).arg(d,-1).arg(e,-1).arg(f,-1).arg(g,-1).get(), ==, std::string( Required ) );
  };

  SUB( Align_minus7_leftalign_postpend_upto7_spaces )
  {
    char Required[128];
    snprintf( Required, DIM(Required), "D) Sml_a=%-7d, Minus_b=%-7d, Minus1_c=%-7d, Zero_d=%-7d, One_e=%-7d, F_f=%-7d, Big_g=%-7d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "D) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a,-7 ).arg( b,-7 ).arg(c,-7).arg(d,-7).arg(e,-7).arg(f,-7).arg(g,-7).get(), ==, std::string( Required ) );
  };

  SUB( Align_plus1_has_no_effect )
  {
    char Required[128];
    snprintf( Required, DIM(Required), "E) Sml_a=%1d, Minus_b=%1d, Minus1_c=%1d, Zero_d=%1d, One_e=%1d, F_f=%1d, Big_g=%1d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "E) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a,1 ).arg( b,1 ).arg(c,1).arg(d,1).arg(e,1).arg(f,1).arg(g,1).get(), ==, std::string( Required ) );
  };

#if defined( NOPF_NUMERICAL_RIGHTALIGN_FILLCHAR ) &&  ( NOPF_NUMERICAL_RIGHTALIGN_FILLCHAR=='0' )
  SUB( Align_plus07_rightalign_prepend_upto7_zeroes ) // does prepend '0000' up to 7 times
  {
    char Required[128];
    snprintf( Required, DIM(Required), "F) Sml_a=%07d, Minus_b=%07d, Minus1_c=%07d, Zero_d=%07d, One_e=%07d, F_f=%07d, Big_g=%07d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "F) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a,7 ).arg( b,7 ).arg(c,7).arg(d,7).arg(e,7).arg(f,7).arg(g,7).get(), ==, std::string( Required ) );
  };
#elif defined( NOPF_NUMERICAL_RIGHTALIGN_FILLCHAR ) &&  ( NOPF_NUMERICAL_RIGHTALIGN_FILLCHAR==' ' )
  SUB( Align_plus07_rightalign_prepend_upto7_zeroes ) // does prepend ' ' up to 7 times
  {
    char Required[128];
    snprintf( Required, DIM(Required), "F) Sml_a=%7d, Minus_b=%7d, Minus1_c=%7d, Zero_d=%7d, One_e=%7d, F_f=%7d, Big_g=%7d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "F) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a,7 ).arg( b,7 ).arg(c,7).arg(d,7).arg(e,7).arg(f,7).arg(g,7).get(), ==, std::string( Required ) );
  };
#endif

/* currently no syntax for right-align and space padding
  SUB( Align_plus_space7 )
  {
    char Required[128];
    snprintf( Required, DIM(Required), "G) Sml_a=%7d, Minus_b=%7d, Minus1_c=%7d, Zero_d=%7d, One_e=%7d, F_f=%7d, Big_g=%7d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "G) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a,7 ).arg( b,7 ).arg(c,7).arg(d,7).arg(e,7).arg(f,7).arg(g,7).get(), ==, std::string( Required ) );
  };
*/

} // End-of TEST( Handling_aligned_numbers )


#define JOHANNES_1_1a "Im Anfang war das Wort, und das Wort war bei Gott, und das Wort war Gott."
#define JOHANNES_1_1b "Im Anfang war es bei Gott."
#define JOHANNES_1_1c "Alles ist durch das Wort geworden und ohne das Wort wurde nichts, was geworden ist."
#define JOHANNES_1_1d "In ihm war das Leben und das Leben war das Licht der Menschen."

#define First_Thing_A "das Wort"
#define First_Thing_B "der Urknall"
#define JWH "Gott"
#define ATHEIST "Singular"

#define BIBLE_OPENING_LINES JOHANNES_1_1a "\r\n" JOHANNES_1_1b "\r" JOHANNES_1_1c "\n" JOHANNES_1_1d "\t."

#define BIBLE_OPENING_TMPL                                                  \
  "Im Anfang war $1, und $1 war bei $2, und $1 war $2."                     \
  "\r\n"                                                                    \
  "Im Anfang war es bei $2."                                                \
  "\r"                                                                      \
  "Alles ist durch $1 geworden und ohne $1 wurde nichts, was geworden ist." \
  "\n"                                                                      \
  "In ihm war das Leben und das Leben war das Licht der Menschen."          \
  "\t."


TEST( NoPrintf_SimpleString_Creations )
{
  SUB( SimpleString )
  {
    NoPrintf OneWord( "Wort" );
    REQ( OneWord.get(), ==, std::string( "Wort" ) );
    REQ( OneWord.get(), !=, std::string( "wort" ) );
    REQ( OneWord.get(), !=, std::string( "WORT" ) );
    REQ( OneWord.get(), !=, std::string( "Wort\n" ) );
  };

  SUB( Simple_Sentence )
  {
    NoPrintf OneSentence( BIBLE_OPENING_LINES );
    REQ( OneSentence.get(), ==, std::string( BIBLE_OPENING_LINES ) );
    REQ( OneSentence.get(), ==, std::string( JOHANNES_1_1a "\r\n" JOHANNES_1_1b "\r" JOHANNES_1_1c "\n" JOHANNES_1_1d "\t." ) );
  };

  SUB( Replace_Dollars )
  {
    NoPrintf Dollars_1( "3$ 4ct, aber 7$$ 6ct." );
    Dollars_1.arg( 3ul ).arg( 4ul ).arg( 7ul ).arg( 6ul );
    REQ( Dollars_1.get(), ==, std::string( "3$ 4ct, aber 7$ 6ct." ) );

    NoPrintf Dollars_2( "$1$ $2ct, aber $3$$ $4ct." );
    Dollars_2.arg( 3ul ).arg( 4ul ).arg( 7ul ).arg( 6ul );
    REQ( Dollars_2.get(), ==, std::string( "3$ 4ct, aber 7$ 6ct." ) );
  };

  SUB( Creation_of_all )
  {
    NoPrintf Christian( BIBLE_OPENING_TMPL );
    Christian.arg( First_Thing_A ).arg( JWH );
    NoPrintf Heretics;
    Heretics.set( BIBLE_OPENING_TMPL ).arg( First_Thing_B ).arg( ATHEIST );
    REQ( Christian.get(), ==, std::string( BIBLE_OPENING_LINES ) );
    REQ( Heretics.get(), !=, std::string( BIBLE_OPENING_LINES ) );
  };
}


// To test DATA
DATA( GetFailedTests )
{
  unsigned int failedTestCount = 0;
  std::cout << "-----------------------------" << std::endl;
  std::cout << "Failed tests:" << std::endl;
  data->IterSons(
      [ &failedTestCount ]( const lightest::Data* item )
      {
        const lightest::DataSet* test = static_cast<const lightest::DataSet*>( item );
        if( test->GetFailed() )
        {
          std::cout << " * " << test->GetName() << std::endl;
          failedTestCount++;
        }
      } ); // lambda-end
  std::cout << failedTestCount << " test" << ( failedTestCount > 1 ? "s" : "" ) << " failed." << std::endl;
  std::cout << "-----------------------------" << std::endl;
  if( lightest::toOutput )
  {
    std::cout << "Now the overall report:" << std::endl;
  }
  return; // just for easy breakpoint setting while debugging failed tests.
}
