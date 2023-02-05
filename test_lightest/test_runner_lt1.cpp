#include <limits>
#include <sstream>
#include <string>

#include "lightest/lightest.h"
#include "no_printf.hpp"
//#define __FILE__ "test.cpp"

#ifndef DIM
#  define DIM( a ) ( sizeof( a ) / sizeof( a[ 0 ] ) )
#endif

// ToDo: could be made of a template
class Limits
{
public:
  Limits()
    // init fixed integer types
    // 8 bit
    : uint8_min( std::numeric_limits<uint8_t>::min() ), uint8_max( std::numeric_limits<uint8_t>::max() )
    , int8_min( std::numeric_limits<int8_t>::min() ), int8_max( std::numeric_limits<int8_t>::max() )
    // 16 bit
    , uint16_min( std::numeric_limits<uint16_t>::min() ), uint16_max( std::numeric_limits<uint16_t>::max() )
    , int16_min( std::numeric_limits<int16_t>::min() ), int16_max( std::numeric_limits<int16_t>::max() )
    // 32 bit
    , uint32_min( std::numeric_limits<uint32_t>::min() ), uint32_max( std::numeric_limits<uint32_t>::max() )
    , int32_min( std::numeric_limits<int32_t>::min() ), int32_max( std::numeric_limits<int32_t>::max() )
    // 64 bit
#   if defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )
    , uint64_min( std::numeric_limits<uint64_t>::min() ), uint64_max( std::numeric_limits<uint64_t>::max() )
    , int64_min( std::numeric_limits<int64_t>::min() ), int64_max( std::numeric_limits<int64_t>::max() )
#   endif //defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )
    // init fundamental integer types
    // char, usually 1 byte / 8 bit. Defined as "at least 8 bit", commonly implemented mostly as 8 bit (https://en.cppreference.com/w/cpp/language/types)
    , uchar_min( std::numeric_limits<unsigned char>::min() ), uchar_max( std::numeric_limits<unsigned char>::max() )
    , char_min( std::numeric_limits<signed char>::min() ), char_max( std::numeric_limits<signed char>::max() )
    // short, usually 2 byte / 16 bit. Defined as "at least 16 bit", commonly implemented mostly as 16 bit (https://en.cppreference.com/w/cpp/language/types)
    , ushort_min( std::numeric_limits<unsigned short>::min() ), ushort_max( std::numeric_limits<unsigned short>::max() )
    , short_min( std::numeric_limits<signed short>::min() ), short_max( std::numeric_limits<signed short>::max() )
    // unspecified int, usually 2..4 byte / 16..32 bit. Defined as "at least 16 bit", often implemented as 32 bit (https://en.cppreference.com/w/cpp/language/types)
    , uint_min( std::numeric_limits<unsigned int>::min() ), uint_max( std::numeric_limits<unsigned int>::max() )
    , int_min( std::numeric_limits<signed int>::min() ), int_max( std::numeric_limits<signed int>::max() )
    // long, usually 4..8 byte / 32..64 bit. Defined as "at least 32 bit", often implemented as 32 bit, except LP64 (https://en.cppreference.com/w/cpp/language/types)
    , ulong_min( std::numeric_limits<unsigned long>::min() ), ulong_max( std::numeric_limits<unsigned long>::max() )
    , long_min( std::numeric_limits<signed long>::min() ), long_max( std::numeric_limits<signed long>::max() )
    // long long, usually 8 byte / 64 bit. Defined as "at least 64 bit", commonly implemented mostly as 64 bit (https://en.cppreference.com/w/cpp/language/types)
#if defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG)
    , ulonglong_min( std::numeric_limits<unsigned long long>::min() ), ulonglong_max( std::numeric_limits<unsigned long long>::max() )
    , longlong_min( std::numeric_limits<signed long long>::min() ), longlong_max( std::numeric_limits<signed long long>::max() )
#endif //defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG)
  {}

  ~Limits() {}

public: // members
    // fixed integer types
    size_t uint8_min, uint8_max;
    size_t int8_min, int8_max;
    size_t uint16_min, uint16_max;
    size_t int16_min, int16_max;
    size_t uint32_min, uint32_max;
    size_t int32_min, int32_max;
#   if defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )
    size_t uint64_min, uint64_max;
    size_t int64_min, int64_max;
#   endif //defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )

    // fundamental integer types
    size_t uchar_min, uchar_max;
    size_t char_min, char_max;
    size_t ushort_min, ushort_max;
    size_t short_min, short_max;
    size_t uint_min, uint_max;
    size_t int_min, int_max;
    size_t ulong_min, ulong_max;
    size_t long_min, long_max;
#if defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG)
    size_t ulonglong_min, ulonglong_max;
    size_t longlong_min, longlong_max;
#endif //defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG)
};


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
  Limits Lim;
  int a = Lim.int_min;
  int b = Lim.int_max;

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
  static Limits Lim;

  SUB( Numerical_8bit_int )
  {
    signed char       a = 0, b = 1, c = -1;
    char              Topic = 'a';
    std::stringstream Required;

    /* 0/1/-1 test, no extreme limits */
    REQ( a, < , b );
    REQ( c, < , a );
    Required << Topic << ") signed ShortShort A=" << static_cast<short>( a ) << ", B=" << static_cast<short>( b ) << ", C=" << static_cast<short>( c ) << ".";
    REQ( NoPrintf( "a) signed ShortShort A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );
    Topic++;

    // 8 bit unsigned values are likely not fit to char
    if( Lim.char_max >= Lim.uint8_max
     && Lim.char_min <= Lim.uint8_min )
    {
      Required.str( std::string() );
      a = Lim.uint8_min;
      b = Lim.uint8_max;
      REQ( a, < , b );
      Required << Topic << ") signed ShortShort A=" << static_cast<short>( a ) << ", B=" << static_cast<short>( b ) << ".";
      REQ( NoPrintf( "b) signed ShortShort A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.int8_min;
    b = Lim.int8_max;
    REQ( a, < , b );
    Required << Topic << ") signed ShortShort A=" << static_cast<short>( a ) << ", B=" << static_cast<short>( b ) << ".";
    REQ( NoPrintf( "c) signed ShortShort A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    //Topic++;
  };

  SUB( Numerical_short )
  {
    short             a = 0, b = 1, c = -1;
    char              Topic = 'a';
    std::stringstream Required;

    /* 0/1/-1 test, no extreme limits */
    REQ( a, < , b );
    REQ( c, < , a );
    Required << Topic << ") signed Short A=" << a << ", B=" << b << ", C=" << c << ".";
    REQ( NoPrintf( "a) signed Short A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );
    Topic++;

    // 8 bit unsigned values must fit
    Required.str( std::string() );
    REQ( a, < , b );
    a = Lim.uint8_min;
    b = Lim.uint8_max;
    Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.int8_min;
    b = Lim.int8_max;
    REQ( a, < , b );
    Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // 16 bit unsigned values could perhaps fit
    if( Lim.short_max >= Lim.uint16_max
     && Lim.short_min <= Lim.uint16_min )
    {
      Required.str( std::string() );
      a = Lim.uint16_min;
      b = Lim.uint16_max;
      REQ( a, < , b );
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "d) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = Lim.int16_min;
    b = Lim.int16_max;
    REQ( a, < , b );
    Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // 32 bit unsigned values will likely not fit
    if( Lim.short_max >= Lim.uint32_max
     && Lim.short_min <= Lim.uint32_min )
    {
#     if defined( SHRT_MAX ) && defined( UINT32_MAX ) && (SHRT_MAX>=UINT32_MAX)
      Required.str( std::string() );
      a = Lim.uint32_min;
      b = Lim.uint32_max;
      REQ( a, < , b );
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "f) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#     else // perhaps some overflow expectations testing?
#     endif //defined( SHRT_MAX ) && defined( UINT32_MAX ) && (SHRT_MAX>=UINT32_MAX)
    }
    Topic++;

    // 32 bit signed values will likely not fit
    if( Lim.short_max >= Lim.int32_max
     && Lim.short_min <= Lim.int32_min )
    {
#     if defined( SHRT_MAX ) && defined( INT32_MAX ) && (SHRT_MAX>=INT32_MAX)
      Required.str( std::string() );
      a = Lim.int32_min;
      b = Lim.int32_max;
      REQ( a, < , b );
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "g) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#     else // perhaps some overflow expectations testing?
#     endif //defined( SHRT_MAX ) && defined( INT32_MAX ) && (SHRT_MAX>=INT32_MAX)
    }
    Topic++;

    // unsigned short values likely not fit in short, because short == signed short
    if( Lim.ushort_max >= Lim.int32_max &&
        Lim.ushort_min <= Lim.int32_min )
    {
      Required.str( std::string() );
      a = Lim.ushort_min;
      b = Lim.ushort_max;
      REQ( a, < , b );
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "h) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // signed short values must fit
    Required.str( std::string() );
    a = Lim.short_min;
    b = Lim.short_max;
    REQ( a, < , b );
    Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // a (singned) short value can not hold an unsigned int
    if( Lim.short_max >= Lim.uint_max &&
        Lim.short_min <= Lim.uint_min )
    {
#     if defined( SHRT_MAX ) && defined( UINT_MAX ) && (SHRT_MAX>=UINT_MAX)
      Required.str( std::string() );
      a = Lim.uint_min;
      b = Lim.uint_max;
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( a, < , b );
      REQ( NoPrintf( "j) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#     else // perhaps some overflow expectations testing?
      Required << ++Topic << ") signed short is " << (__SHRT_WIDTH__ -1) << " bit, skipping because of overflow on " << __INT_WIDTH__ << ".";
      REQ( NoPrintf( "k) signed short is $1 bit, skipping because of overflow on $2." ).arg(__SHRT_WIDTH__ -1).arg(__INT_WIDTH__).get(), ==, Required.str() );
#     endif //defined( SHRT_MAX ) && defined( UINT_MAX ) && (SHRT_MAX>=UINT_MAX)
    }
    Topic++;

    // a (singned) short value can likely not hold an signed int
    if( Lim.short_max >= Lim.int_max &&
        Lim.short_min <= Lim.int_min )
    {
#     if defined( SHRT_MAX ) && defined( INT_MAX ) && (SHRT_MAX>=INT_MAX)
      Required.str( std::string() );
      a = Lim.int_min;
      b = Lim.int_max;
      REQ( a, < , b );
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "k) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#     else // perhaps some overflow expectations testing?
      Required << ++Topic << ") signed short is " << (__SHRT_WIDTH__ -1) << " bit, skipping because of overflow on " << __INT_WIDTH__ << ".";
      REQ( NoPrintf( "k) signed short is $1 bit, skipping because of overflow on $2." ).arg(__SHRT_WIDTH__ -1).arg(__INT_WIDTH__).get(), ==, Required.str() );
#     endif //defined( SHRT_MAX ) && defined( INT_MAX ) && (SHRT_MAX>=INT_MAX)
    }
    Topic++;

    // no knowledge, if PlatformIO ESP8266 supports uint64_t / long long and what exactly long is, compared to int (short < int <= long)
  };


  SUB( Numerical_integer )
  {
    int               a = 0, b = 1, c = -1;
    char              Topic = 'a';
    std::stringstream Required;

    /* 0/1/-1 test, no extreme limits */
    REQ( a, < , b );
    REQ( c, < , a );
    Required << Topic << ") signed Integer A=" << a << ", B=" << b << ", C=" << c << ".";
    REQ( NoPrintf( "a) signed Integer A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );

    // 8 bit unsigned values must fit
    Required.str( std::string() );
    a = Lim.uint8_min;
    b = Lim.uint8_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.int8_min;
    b = Lim.int8_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit unsigned values must fit
    Required.str( std::string() );
    a = Lim.uint16_min;
    b = Lim.uint16_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "d) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = Lim.int16_min;
    b = Lim.int16_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // bit problematic test here, must be made CPU dependent
    // 32 bit unsigned values could fit
    Required.str( std::string() );
    a = Lim.uint32_min;
#   if defined( __INT_MAX__ ) && defined( __UINT32_MAX__ ) && (__INT_MAX__ >= __UINT32_MAX__)
    b = Lim.uint32_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "f) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#   else // if uint32 is bigger than signed int, we can not do this test on int. on very most platforms INT_MAX is (uint32/2-1)
    Required << ++Topic << ") signed INT is " << (__INT_WIDTH__ -1) << " bit, skipping because of overflow on " << __INT_LEAST32_WIDTH__ << ".";
    REQ( NoPrintf( "f) signed INT is $1 bit, skipping because of overflow on $2." ).arg(__INT_WIDTH__ -1).arg(__INT_LEAST32_WIDTH__).get(), ==, Required.str() );
#   endif

    // 32 bit signed values could fit
    Required.str( std::string() );
    a = Lim.int32_min;
    b = Lim.int32_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "g) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unsigned short values must fit
    Required.str( std::string() );
    a = Lim.ushort_min;
    b = Lim.ushort_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "h) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // signed short values must fit
    Required.str( std::string() );
    a = Lim.short_min;
    b = Lim.short_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    if( Lim.int_max >= Lim.uint_max &&
        Lim.int_min <= Lim.uint_min )
    {
      Required.str( std::string() );
      a = Lim.uint_min;
      b = Lim.uint_max;
      REQ( a, <, b );
      Required << Topic << ") signed Integer A=" << a << ", B=" << b << ".";
      // if int is unsigned (unlikely) unsigned int values must fit
      REQ( NoPrintf( "j) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // signed int values must fit
    Required.str( std::string() );
    a = Lim.int_min;
    b = Lim.int_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "k) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
  };


  SUB( Numerical_long )
  {
    long              a = 0L, b = 1L, c = -1L;
    char              Topic = 'a';
    std::stringstream Required;

    /* 0/1/-1 test, no extreme limits */
    REQ( a, < , b );
    REQ( c, < , a );
    Required << Topic << ") signed Long A=" << a << ", B=" << b << ", C=" << c << ".";
    REQ( NoPrintf( "a) signed Long A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );

    // 8 bit unsigned values must fit
    Required.str( std::string() );
    a = Lim.uint8_min;
    b = Lim.uint8_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.int8_min;
    b = Lim.int8_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit unsigned values must fit
    Required.str( std::string() );
    a = Lim.uint16_min;
    b = Lim.uint16_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "d) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = Lim.int16_min;
    b = Lim.int16_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // bit problematic test here, must be made CPU dependent
    // 32 bit unsigned values could fit
    Required.str( std::string() );
    a = Lim.uint32_min;
#   if defined( __LONG_MAX__ ) && defined( __UINT32_MAX__ ) && (__LONG_MAX__ >= __UINT32_MAX__)
    b = Lim.uint32_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "f) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#   else // if uint32 is bigger than signed long, we can not do this test on int. On some 64 bit platforms LONG_MAX could be (uint32/2-1)???
    Required << ++Topic << ") signed LONG is " << (__LONG_WIDTH__ -1) << " bit, skipping because of overflow on " << __INT_LEAST32_WIDTH__ << ".";
    REQ( NoPrintf( "f) signed LONG is $1 bit, skipping because of overflow on $2." ).arg(__LONG_WIDTH__ -1).arg(__INT_LEAST32_WIDTH__).get(), ==, Required.str() );
#   endif

    // 32 bit signed values could fit
    Required.str( std::string() );
    a = Lim.int32_min;
    b = Lim.int32_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "g) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unsigned short values must fit
    Required.str( std::string() );
    a = Lim.ushort_min;
    b = Lim.ushort_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "h) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // signed short values must fit
    Required.str( std::string() );
    a = Lim.short_min;
    b = Lim.short_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    Required.str( std::string() );
    a = Lim.uint_min;
#   if defined( __LONG_MAX__ ) && defined( __UINT64_MAX__ ) && (__LONG_MAX__ >= __UINT64_MAX__)
    b = Lim.uint_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    // if int is unsigned (unlikely) unsigned int values must fit
    REQ( NoPrintf( "j) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#   else // if uint is bigger than signed long, we can not do this test on int. On some 64 bit platforms LONG_MAX could be (uint/2-1)???
    Required << ++Topic << ") signed LONG is " << (__LONG_WIDTH__ -1) << " bit, skipping because of overflow on " << __INT_FAST64_WIDTH__ << ".";
    REQ( NoPrintf( "j) signed LONG is $1 bit, skipping because of overflow on $2." ).arg(__LONG_WIDTH__ -1).arg(__INT_FAST64_WIDTH__).get(), ==, Required.str() );
#   endif

    // signed int values must fit
    Required.str( std::string() );
    a = Lim.int_min;
    b = Lim.int_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "k) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unspecified (compiler preset) int values must fit
    Required.str( std::string() );
    a = Lim.int_min;
    b = Lim.int_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "l) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    if( Lim.long_max >= Lim.ulong_max &&
        Lim.long_min <= Lim.ulong_min )
    {
      Required.str( std::string() );
      a = Lim.ulong_min;
      b = Lim.ulong_max;
      REQ( a, <, b );
      Required << Topic << ") signed Long A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "m) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    Required.str( std::string() );
    a = Lim.long_min;
    b = Lim.long_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "n) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
  };


  SUB( Numerical_unsigned_long )
  {
    unsigned long     a = 0, b = 1;
    char              Topic = 'a';
    std::stringstream Required;

    /* 0/1/-1 test, no extreme limits */
    REQ( a, < , b );
    Required << Topic << ") unsigned Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "a) unsigned Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
  };


#if defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG)
  // no knowledge, if PlatformIO ESP8266 supports uint64_t / long long and what exactly long is, compared to int (short < int <= long)
  SUB( Numerical_long_long )
  {
    long long         a = 0LL, b = 1LL, c = -1LL;
    char              Topic = 'a';
    std::stringstream Required;

    /* 0/1/-1 test, no extreme limits */
    REQ( a, < , b );
    REQ( c, < , a );
    Required << Topic << ") signed Long Long A=" << a << ", B=" << b << ", C=" << c << ".";
    REQ( NoPrintf( "a) signed Long Long A=$1, B=$2, C=$3." ).arg( a ).arg( b ).arg( c ).get(), ==, Required.str() );

    // 8 bit unsigned values must fit
    Required.str( std::string() );
    a = Lim.uint8_min;
    b = Lim.uint8_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.int8_min;
    b = Lim.int8_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit unsigned values must fit
    Required.str( std::string() );
    a = Lim.uint16_min;
    b = Lim.uint16_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "d) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = Lim.int16_min;
    b = Lim.int16_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 32 bit unsigned values must fit to long long
    Required.str( std::string() );
    a = Lim.uint32_min;
    b = Lim.uint32_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "f) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 32 bit signed values must fit to long long
    Required.str( std::string() );
    a = Lim.int32_min;
    b = Lim.int32_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "g) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

#   if defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )
    // 64 bit unsigned values could fit to long long
    Required.str( std::string() );
    a = Lim.uint64_min;
#   if defined( __LONG_LONG_MAX__ ) && defined( __UINT64_MAX__ ) && (__LONG_LONG_MAX__ >= __UINT64_MAX__)
    b = Lim.uint64_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "h) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#   else // if uint64 is bigger than signed long long, we can not do this test on int. On most platforms LONGLONG_MAX is(uint64/2-1)
    Required << ++Topic << ") signed LONG_LONG is " << (__LONG_LONG_WIDTH__ -1) << " bit, skipping because of overflow on " << __INT_FAST64_WIDTH__ << ".";
    REQ( NoPrintf( "h) signed LONG_LONG is $1 bit, skipping because of overflow on $2." ).arg(__LONG_LONG_WIDTH__ -1).arg(__INT_FAST64_WIDTH__).get(), ==, Required.str() );
#   endif

    // 64 bit signed should must fit to long long
    Required.str( std::string() );
    a = Lim.int64_min;
    b = Lim.int64_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#   else
    Topic += 2;
#   endif // defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )

    // unsigned short values must fit
    Required.str( std::string() );
    a = Lim.ushort_min;
    b = Lim.ushort_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "j) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // signed short values must fit
    Required.str( std::string() );
    a = Lim.short_min;
    b = Lim.short_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "k) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // uint values must fit to long long
    Required.str( std::string() );
    a = Lim.uint_min;
    b = Lim.uint_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    // if int is unsigned (unlikely) unsigned int values must fit
    REQ( NoPrintf( "l) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    Required.str( std::string() );
    a = Lim.int_min;
    b = Lim.int_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "m) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    Required.str( std::string() );
    a = Lim.int_min;
    b = Lim.int_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "n) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    Required.str( std::string() );
    a = Lim.long_min;
    b = Lim.long_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "o) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unspecified long is for sure signed long
    Required.str( std::string() );
    a = Lim.long_min;
    b = Lim.long_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "p) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // no knowledge, if PlatformIO ESP8266 supports uint64_t / long long and what exactly long is, compared to int (short < int <= long)
    if( Lim.longlong_max >= Lim.ulonglong_max &&
        Lim.longlong_min <= Lim.ulonglong_min )
    {
      Required.str( std::string() );
      a = Lim.ulonglong_min;
      b = Lim.ulonglong_max;
      REQ( a, <, b );
      Required << Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "q) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    Required.str( std::string() );
    a = Lim.longlong_min;
    b = Lim.longlong_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "r) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unspecified long is for sure signed long
    Required.str( std::string() );
    a = Lim.longlong_min;
    b = Lim.longlong_max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "s) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
  };


  SUB( Numerical_unsigned_longlong )
  {
    unsigned long long     a = 0ull, b = 1ull;
    char              Topic = 'A';
    std::stringstream Required;

    /* 0/1/-1 test, no extreme limits */
    REQ( a, < , b );
    Required << Topic << ") Unsigned Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "A) Unsigned Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    a = Lim.ulong_min;
    b = Lim.ulong_max;
    REQ( a, < , b );
    Required.str( std::string() );
    Required << ++Topic << ") Unsigned Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "B) Unsigned Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
  };


#endif //  defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG)
}; // end-of TEST(Handling_Numbers_in_DotArg)


TEST( Handling_aligned_numbers )
{
  static Limits Lim;

  int a = Lim.int_min;
  int b = -654321;
  int c = -1;
  int d = 0;
  int e = 1;
  int f = 1234567;
  int g = Lim.int_max;
  /*
  std::cout << "int32_t min() : " << std::numeric_limits<int32_t>::min() << std::endl;
  std::cout << "int32_t max() : " << std::numeric_limits<int32_t>::max() << std::endl;
  std::cout << "int     min() : " << Lim.int_min << std::endl;
  std::cout << "int     max() : " << Lim.int_max << std::endl;
  std::cout << "signed  min() : " << std::numeric_limits<signed int>::min() << std::endl;
  std::cout << "signed  max() : " << std::numeric_limits<signed int>::max() << std::endl;
  std::cout << "long    min() : " << Lim.long_min << std::endl;
  std::cout << "long    max() : " << Lim.long_max << std::endl;
  */
  REQ( a, < , b );
  REQ( b, < , c );
  REQ( c, < , d );
  REQ( d, < , e );
  REQ( e, < , f );
  REQ( f, < , g );

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
