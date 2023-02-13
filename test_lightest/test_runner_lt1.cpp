#include <iomanip>  // std::setprecision
#include <iostream> // std::cout, std::fixed, std::scientific
#include <limits>
#include <sstream>
#include <string>

#include "lightest/lightest.h"
#include "no_printf.hpp"

#if defined( NOPF_USE_ARGCONFIG ) && ( NOPF_USE_ARGCONFIG > 0 )
#  include "lightest/arg_config_ext.h"
#endif

#if defined( NOPF_USE_DATA_ANALYSIS ) && ( NOPF_USE_DATA_ANALYSIS > 0 )
#  include "lightest/data_analysis_ext.h"
#endif


#ifndef __FILE_NAME__ // g++ warning: "__FILE_NAME__" redefined [-Wbuiltin-macro-redefined] / can't undefine either: warning: undefining "__FILE_NAME__" [-Wbuiltin-macro-redefined]
#  define __FILE_NAME__ "core_test.cpp"
#endif

#ifndef DIM
#  define DIM( a ) ( sizeof( a ) / sizeof( a[ 0 ] ) )
#endif

template<typename T>
class TypeLimits
{
public:
  TypeLimits<T>() {}
  ~TypeLimits() {}
  static const T min, max;
};

template<typename T>
const T TypeLimits<T>::min( std::numeric_limits<T>::min() );
template<typename T>
const T TypeLimits<T>::max( std::numeric_limits<T>::max() );


class Limits
{
public:
  Limits() {}
  ~Limits() {}

public: // members
  // fixed integer types
  TypeLimits<uint8_t>  uint8;
  TypeLimits<int8_t>   sint8;
  TypeLimits<uint16_t> uint16;
  TypeLimits<int16_t>  sint16;
  TypeLimits<uint32_t> uint32;
  TypeLimits<int32_t>  sint32;
#if defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )
  TypeLimits<uint64_t> uint64;
  TypeLimits<int64_t>  sint64;
#endif //defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )
  // fundamental integer types
  TypeLimits<unsigned char> uchar;
  TypeLimits<signed char>   schar;
  TypeLimits<unsigned short>
                           ushort; // short, usually 2 byte / 16 bit. Defined as "at least 16 bit", commonly implemented mostly as 16 bit (https://en.cppreference.com/w/cpp/language/types)
  TypeLimits<signed short> sshort;
  TypeLimits<unsigned int>
                         uint; // unspecified int, usually 2..4 byte / 16..32 bit. Defined as "at least 16 bit", often implemented as 32 bit (https://en.cppreference.com/w/cpp/language/types)
  TypeLimits<signed int> sint;
  TypeLimits<unsigned long>
                          ulong; // long, usually 4..8 byte / 32..64 bit. Defined as "at least 32 bit", often implemented as 32 bit, except LP64 (https://en.cppreference.com/w/cpp/language/types)
  TypeLimits<signed long> slong;
#if defined( NOPF_USE_LONGLONG ) && ( NOPF_USE_LONGLONG )
  TypeLimits<unsigned long long>
                               ulonglong; // long long, usually 8 byte / 64 bit. Defined as "at least 64 bit", commonly implemented mostly as 64 bit (https://en.cppreference.com/w/cpp/language/types)
  TypeLimits<signed long long> slonglong;
#endif //defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG)
};

#if defined( NOPF_USE_ARGCONFIG ) && ( NOPF_USE_ARGCONFIG > 0 )
ARG_CONFIG();
#else
CONFIG( Configurations )
{
  for( ; argn > 0; argn--, argc++ )
  {
    if( std::string( *argc ) == "--no-color" ) NO_COLOR();
    if( std::string( *argc ) == "--no-output" ) NO_OUTPUT();
  }
}
#endif

/*
TEST( TestTimerMacros )
{
  int i = 0;
  REQ( TIMER( i++ ), >=, 0 );          // Run once and measure the time
  REQ( AVG_TIMER( i++, 100 ), >=, 0 ); // Run several times and return the average time
}
*/

TEST( Handling_Check )
{
  Limits Lim;
  int    a = Lim.sint.min;
  int    b = Lim.sint.max;

  SUB( Align_default_no_filling )
  {
    char Required[ 128 ];
    snprintf( Required, DIM( Required ), "A) Sml_a=%d, Big_g=%d.", a, b );
    Required[ DIM( Required ) - 1 ] = '\0';
    REQ( NoPrintf( "A) Sml_a=$1, Big_g=$2." ).arg( a ).arg( b ).get(), ==, std::string( Required ) );
  };
}

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

} // end-of TEST(Can_create_Empty_Strings)


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
}


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
    if( Lim.schar.max >= Lim.uint8.max
     && Lim.schar.min <= Lim.uint8.min )
    {
      Required.str( std::string() );
      a = Lim.uint8.min;
      b = Lim.uint8.max;
      REQ( a, < , b );
      Required << Topic << ") signed ShortShort A=" << static_cast<short>( a ) << ", B=" << static_cast<short>( b ) << ".";
      REQ( NoPrintf( "b) signed ShortShort A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.sint8.min;
    b = Lim.sint8.max;
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
    a = Lim.uint8.min;
    b = Lim.uint8.max;
    Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.sint8.min;
    b = Lim.sint8.max;
    REQ( a, < , b );
    Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // 16 bit unsigned values could perhaps fit
    if( Lim.sshort.max >= Lim.uint16.max
     && Lim.sshort.min <= Lim.uint16.min )
    {
      Required.str( std::string() );
      a = Lim.uint16.min;
      b = Lim.uint16.max;
      REQ( a, < , b );
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "d) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = Lim.sint16.min;
    b = Lim.sint16.max;
    REQ( a, < , b );
    Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // 32 bit unsigned values will likely not fit
    if( Lim.sshort.max >= Lim.uint32.max
     && Lim.sshort.min <= Lim.uint32.min )
    {
#     if defined( SHRT_MAX ) && defined( UINT32_MAX ) && (SHRT_MAX>=UINT32_MAX)
      Required.str( std::string() );
      a = Lim.uint32.min;
      b = Lim.uint32.max;
      REQ( a, < , b );
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "f) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#     else // perhaps some overflow expectations testing?
#     endif //defined( SHRT_MAX ) && defined( UINT32_MAX ) && (SHRT_MAX>=UINT32_MAX)
    }
    Topic++;

    // 32 bit signed values will likely not fit
    if( Lim.sshort.max >= Lim.sint32.max
     && Lim.sshort.min <= Lim.sint32.min )
    {
#     if defined( SHRT_MAX ) && defined( INT32_MAX ) && (SHRT_MAX>=INT32_MAX)
      Required.str( std::string() );
      a = Lim.sint32.min;
      b = Lim.sint32.max;
      REQ( a, < , b );
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "g) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#     else // perhaps some overflow expectations testing?
#     endif //defined( SHRT_MAX ) && defined( INT32_MAX ) && (SHRT_MAX>=INT32_MAX)
    }
    Topic++;

    // unsigned short values likely not fit in short, because short == signed short
    if( Lim.ushort.max >= Lim.sint32.max &&
        Lim.ushort.min <= Lim.sint32.min )
    {
      Required.str( std::string() );
      a = Lim.ushort.min;
      b = Lim.ushort.max;
      REQ( a, < , b );
      Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "h) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // signed short values must fit
    Required.str( std::string() );
    a = Lim.sshort.min;
    b = Lim.sshort.max;
    REQ( a, < , b );
    Required << Topic << ") signed Short A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) signed Short A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    Topic++;

    // a (singned) short value can not hold an unsigned int
    if( Lim.sshort.max >= Lim.uint.max &&
        Lim.sshort.min <= Lim.uint.min )
    {
#     if defined( SHRT_MAX ) && defined( UINT_MAX ) && (SHRT_MAX>=UINT_MAX)
      Required.str( std::string() );
      a = Lim.uint.min;
      b = Lim.uint.max;
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
    if( Lim.sshort.max >= Lim.sint.max &&
        Lim.sshort.min <= Lim.sint.min )
    {
#     if defined( SHRT_MAX ) && defined( INT_MAX ) && (SHRT_MAX>=INT_MAX)
      Required.str( std::string() );
      a = Lim.sint.min;
      b = Lim.sint.max;
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
    a = Lim.uint8.min;
    b = Lim.uint8.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.sint8.min;
    b = Lim.sint8.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit unsigned values must fit
    Required.str( std::string() );
    a = Lim.uint16.min;
    b = Lim.uint16.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "d) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = Lim.sint16.min;
    b = Lim.sint16.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // bit problematic test here, must be made CPU dependent
    // 32 bit unsigned values could fit
    Required.str( std::string() );
    a = Lim.uint32.min;
#   if defined( __INT_MAX__ ) && defined( __UINT32_MAX__ ) && (__INT_MAX__ >= __UINT32_MAX__)
    b = Lim.uint32.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "f) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#   else // if uint32 is bigger than signed int, we can not do this test on int. on very most platforms INT_MAX is (uint32/2-1)
    Required << ++Topic << ") signed INT is " << (__INT_WIDTH__ -1) << " bit, skipping because of overflow on " << __INT_LEAST32_WIDTH__ << ".";
    REQ( NoPrintf( "f) signed INT is $1 bit, skipping because of overflow on $2." ).arg(__INT_WIDTH__ -1).arg(__INT_LEAST32_WIDTH__).get(), ==, Required.str() );
#   endif

    // 32 bit signed values could fit
    Required.str( std::string() );
    a = Lim.sint32.min;
    b = Lim.sint32.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "g) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unsigned short values must fit
    Required.str( std::string() );
    a = Lim.ushort.min;
    b = Lim.ushort.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "h) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // signed short values must fit
    Required.str( std::string() );
    a = Lim.sshort.min;
    b = Lim.sshort.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Integer A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    if( Lim.sint.max >= Lim.uint.max &&
        Lim.sint.min <= Lim.uint.min )
    {
      Required.str( std::string() );
      a = Lim.uint.min;
      b = Lim.uint.max;
      REQ( a, <, b );
      Required << Topic << ") signed Integer A=" << a << ", B=" << b << ".";
      // if int is unsigned (unlikely) unsigned int values must fit
      REQ( NoPrintf( "j) signed Integer A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    // signed int values must fit
    Required.str( std::string() );
    a = Lim.sint.min;
    b = Lim.sint.max;
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
    a = Lim.uint8.min;
    b = Lim.uint8.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.sint8.min;
    b = Lim.sint8.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit unsigned values must fit
    Required.str( std::string() );
    a = Lim.uint16.min;
    b = Lim.uint16.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "d) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = Lim.sint16.min;
    b = Lim.sint16.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // bit problematic test here, must be made CPU dependent
    // 32 bit unsigned values could fit
    Required.str( std::string() );
    a = Lim.uint32.min;
#   if defined( __LONG_MAX__ ) && defined( __UINT32_MAX__ ) && (__LONG_MAX__ >= __UINT32_MAX__)
    b = Lim.uint32.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "f) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#   else // if uint32 is bigger than signed long, we can not do this test on int. On some 64 bit platforms LONG_MAX could be (uint32/2-1)???
    Required << ++Topic << ") signed LONG is " << (__LONG_WIDTH__ -1) << " bit, skipping because of overflow on " << __INT_LEAST32_WIDTH__ << ".";
    REQ( NoPrintf( "f) signed LONG is $1 bit, skipping because of overflow on $2." ).arg(__LONG_WIDTH__ -1).arg(__INT_LEAST32_WIDTH__).get(), ==, Required.str() );
#   endif

    // 32 bit signed values could fit
    Required.str( std::string() );
    a = Lim.sint32.min;
    b = Lim.sint32.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "g) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unsigned short values must fit
    Required.str( std::string() );
    a = Lim.ushort.min;
    b = Lim.ushort.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "h) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // signed short values must fit
    Required.str( std::string() );
    a = Lim.sshort.min;
    b = Lim.sshort.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    Required.str( std::string() );
    a = Lim.uint.min;
#   if defined( __LONG_MAX__ ) && defined( __UINT64_MAX__ ) && (__LONG_MAX__ >= __UINT64_MAX__)
    b = Lim.uint.max;
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
    a = Lim.sint.min;
    b = Lim.sint.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "k) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unspecified (compiler preset) int values must fit
    Required.str( std::string() );
    a = Lim.sint.min;
    b = Lim.sint.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "l) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    if( Lim.slong.max >= Lim.ulong.max &&
        Lim.slong.min <= Lim.ulong.min )
    {
      Required.str( std::string() );
      a = Lim.ulong.min;
      b = Lim.ulong.max;
      REQ( a, <, b );
      Required << Topic << ") signed Long A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "m) signed Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    Required.str( std::string() );
    a = Lim.slong.min;
    b = Lim.slong.max;
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
    a = Lim.uint8.min;
    b = Lim.uint8.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "b) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 8 bit signed values must fit
    Required.str( std::string() );
    a = Lim.sint8.min;
    b = Lim.sint8.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "c) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit unsigned values must fit
    Required.str( std::string() );
    a = Lim.uint16.min;
    b = Lim.uint16.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "d) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 16 bit signed values must fit
    Required.str( std::string() );
    a = Lim.sint16.min;
    b = Lim.sint16.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "e) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 32 bit unsigned values must fit to long long
    Required.str( std::string() );
    a = Lim.uint32.min;
    b = Lim.uint32.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "f) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // 32 bit signed values must fit to long long
    Required.str( std::string() );
    a = Lim.sint32.min;
    b = Lim.sint32.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "g) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

#   if defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )
    // 64 bit unsigned values could fit to long long
    Required.str( std::string() );
    a = Lim.uint64.min;
#   if defined( __LONG_LONG_MAX__ ) && defined( __UINT64_MAX__ ) && (__LONG_LONG_MAX__ >= __UINT64_MAX__)
    b = Lim.uint64.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "h) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#   else // if uint64 is bigger than signed long long, we can not do this test on int. On most platforms LONGLONG_MAX is(uint64/2-1)
    Required << ++Topic << ") signed LONG_LONG is " << (__LONG_LONG_WIDTH__ -1) << " bit, skipping because of overflow on " << __INT_FAST64_WIDTH__ << ".";
    REQ( NoPrintf( "h) signed LONG_LONG is $1 bit, skipping because of overflow on $2." ).arg(__LONG_LONG_WIDTH__ -1).arg(__INT_FAST64_WIDTH__).get(), ==, Required.str() );
#   endif

    // 64 bit signed should must fit to long long
    Required.str( std::string() );
    a = Lim.sint64.min;
    b = Lim.sint64.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "i) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
#   else
    Topic += 2;
#   endif // defined( NOPF_USE_INT64 ) && ( NOPF_USE_INT64 )

    // unsigned short values must fit
    Required.str( std::string() );
    a = Lim.ushort.min;
    b = Lim.ushort.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "j) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // signed short values must fit
    Required.str( std::string() );
    a = Lim.sshort.min;
    b = Lim.sshort.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "k) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // uint values must fit to long long
    Required.str( std::string() );
    a = Lim.uint.min;
    b = Lim.uint.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    // if int is unsigned (unlikely) unsigned int values must fit
    REQ( NoPrintf( "l) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    Required.str( std::string() );
    a = Lim.sint.min;
    b = Lim.sint.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "m) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    Required.str( std::string() );
    a = Lim.sint.min;
    b = Lim.sint.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "n) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    Required.str( std::string() );
    a = Lim.slong.min;
    b = Lim.slong.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "o) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unspecified long is for sure signed long
    Required.str( std::string() );
    a = Lim.slong.min;
    b = Lim.slong.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "p) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // no knowledge, if PlatformIO ESP8266 supports uint64_t / long long and what exactly long is, compared to int (short < int <= long)
    if( Lim.slonglong.max >= Lim.ulonglong.max &&
        Lim.slonglong.min <= Lim.ulonglong.min )
    {
      Required.str( std::string() );
      a = Lim.ulonglong.min;
      b = Lim.ulonglong.max;
      REQ( a, <, b );
      Required << Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
      REQ( NoPrintf( "q) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
    }
    Topic++;

    Required.str( std::string() );
    a = Lim.slonglong.min;
    b = Lim.slonglong.max;
    REQ( a, < , b );
    Required << ++Topic << ") signed Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "r) signed Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );

    // unspecified long is for sure signed long
    Required.str( std::string() );
    a = Lim.slonglong.min;
    b = Lim.slonglong.max;
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

    a = Lim.ulong.min;
    b = Lim.ulong.max;
    REQ( a, < , b );
    Required.str( std::string() );
    Required << ++Topic << ") Unsigned Long Long A=" << a << ", B=" << b << ".";
    REQ( NoPrintf( "B) Unsigned Long Long A=$1, B=$2." ).arg( a ).arg( b ).get(), ==, Required.str() );
  };


#endif //  defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG)
} // end-of TEST(Handling_Numbers_in_DotArg)


TEST( Handling_aligned_numbers )
{
  static Limits Lim;

  int a = Lim.sint.min;
  int b = -654321;
  int c = -1;
  int d = 0;
  int e = 1;
  int f = 1234567;
  int g = Lim.sint.max;
  /*
  std::cout << "int32_t min() : " << std::numeric_limits<int32_t>::min() << std::endl;
  std::cout << "int32_t max() : " << std::numeric_limits<int32_t>::max() << std::endl;
  std::cout << "int     min() : " << Lim.sint.min << std::endl;
  std::cout << "int     max() : " << Lim.sint.max << std::endl;
  std::cout << "signed  min() : " << std::numeric_limits<signed int>::min() << std::endl;
  std::cout << "signed  max() : " << std::numeric_limits<signed int>::max() << std::endl;
  std::cout << "long    min() : " << Lim.slong.min << std::endl;
  std::cout << "long    max() : " << Lim.slong.max << std::endl;
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

  char old_fill = NoPF_Set::FillCharAlignRight;
  SUB( Align_plus07_rightalign_prepend_upto7_zeroes ) // does prepend '0000' up to 7 times
  {
    NoPF_Set::FillCharAlignRight = '0';
    char Required[128];

    snprintf( Required, DIM(Required), "F) Minus1_c=%07d, Zero_d=%07d", c, d );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "F) Minus1_c=$1, Zero_d=$2" ).arg( c,7 ).arg( d,7 ).get(), ==, std::string( Required ) );

    snprintf( Required, DIM(Required), "G) Sml_a=%07d, Minus_b=%07d, Minus1_c=%07d, Zero_d=%07d, One_e=%07d, F_f=%07d, Big_g=%07d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "G) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a,7 ).arg( b,7 ).arg(c,7).arg(d,7).arg(e,7).arg(f,7).arg(g,7).get(), ==, std::string( Required ) );
  };

  SUB( Align_plus07_rightalign_prepend_upto7_spaces ) // does prepend ' ' up to 7 times
  {
    NoPF_Set::FillCharAlignRight = ' ';
    char Required[128];

    snprintf( Required, DIM(Required), "H) Minus1_c=%7d, Zero_d=%7d", c, d );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "H) Minus1_c=$1, Zero_d=$2" ).arg( c,7 ).arg( d,7 ).get(), ==, std::string( Required ) );

    snprintf( Required, DIM(Required), "I) Sml_a=%7d, Minus_b=%7d, Minus1_c=%7d, Zero_d=%7d, One_e=%7d, F_f=%7d, Big_g=%7d.", a, b, c, d ,e ,f, g );
    Required[ DIM(Required)-1 ] = '\0';
    REQ( NoPrintf( "I) Sml_a=$1, Minus_b=$2, Minus1_c=$3, Zero_d=$4, One_e=$5, F_f=$6, Big_g=$7." ).arg( a,7 ).arg( b,7 ).arg(c,7).arg(d,7).arg(e,7).arg(f,7).arg(g,7).get(), ==, std::string( Required ) );
  };
  NoPF_Set::FillCharAlignRight = old_fill;
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

  SUB( Can_Init_all_in_one_Line )
  {
    NoPrintf ElectricalDetails;
    int Volt = 10, Ampere = 20, Power = Volt * Ampere;
    ElectricalDetails( "Volt:$1 * Current:$2 => Power:$3" ).val( Volt, "V" ).val( Ampere, "A" ).val( Power, "W" );
    REQ( ElectricalDetails.get(), ==, std::string( "Volt:10 V * Current:20 A => Power:200 W" ) );
  };
} // end - TEST( NoPrintf_SimpleString_Creations )


TEST( raw_is_like_arg )
{
  SUB( SimpleString )
  {
    std::string Required("We replace " "this" " with " "that" " and " "other" " is " "also" " raw.");
    NoPrintf Untranslated("We replace $1 with $2 and $3 is $4 raw.");
    REQ( Untranslated.raw("this").raw("that").raw("other").raw("also").get(), ==, Required );
  };
} // end - TEST( raw_is_like_arg )


TEST( val_is_for_engineering_values )
{
  SUB( Just_some_statics_positive )
  {
    std::string Required("Voltage:111 V * Current: 9 A => Power: 999 W, * 1 h today's Work is 999 Wh.");
    NoPrintf ElectricalSimple("Voltage:$1 * Current: $2 => Power: $3, * $4 today's Work is $5.");
    int Volt=111;
    short Ampere=9;
    long Power = Volt * Ampere;
    uint8_t Time=1;
    long long Work = Power * Time;
    REQ( ElectricalSimple.val(Volt,"V").val(Ampere,"A").val(Power, "W").val(Time,"h").val(Work, "Wh").get(), ==, Required );
  };

  SUB( Just_some_statics_negative )
  {
    std::string Required("Voltage:111 V * Current: -9 A => Power: -999 W, * 1 h today's Work is -999 Wh.");
    NoPrintf ElectricalSimple("Voltage:$1 * Current: $2 => Power: $3, * $4 today's Work is $5.");
    int Volt=111;
    short Ampere=-9;
    signed long Power = Volt * Ampere;
    uint8_t Time=1;
    signed long long Work = Power * Time;
    REQ( ElectricalSimple.val(Volt,"V").val(Ampere,"A").val(Power, "W").val(Time,"h").val(Work, "Wh").get(), ==, Required );
  };

  SUB( NoPF_UnitVal_construct )
  {
    int EngineeringDecimals_old = NoPF_Set::EngineeringDecimals;
    NoPF_Set::EngineeringDecimals = 2;

    short Shortval = 12345;
    UnitVal UV( Shortval, "P" );

    REQ( UV.get_abs(), == , 12345 );
    REQ( UV.is_minus(), == , false );
    REQ( UV.get_sign(), == , '\0' );
    REQ( UV.get_sign(true), == , '+' );
    REQ( UV.get_mult(), == , 1 );
    REQ( UV.get_base(), == , std::string("P") );
    REQ( UV.get_prefix(), == , '\0' );

    NoPF_Set::EngineeringDecimals = EngineeringDecimals_old;
  };

  SUB( NoPF_UnitVal_get )
  {
    int EngineeringDecimals_old = NoPF_Set::EngineeringDecimals;
    NoPF_Set::EngineeringDecimals = 2;
    try
    {
      short Shortval = 12345;
      static UnitVal UV( Shortval, "P" );

      REQ( UV.get_abs(), == , 12345 );
      REQ( UV.is_minus(), == , false );
      REQ( UV.get_sign(), == , '\0' );
      REQ( UV.get_sign(true), == , '+' );
      REQ( UV.get_engineer(), == , 12 );
      REQ( UV.get_reminder(), == , 35 );
      REQ( UV.get_mult(), ==, 1000 );
      REQ( UV.get_base(), ==, std::string("P") );
      REQ( UV.get_gap(), ==, std::string(" ") );
      REQ( UV.get_prefix(), ==, 'k' );

      static UnitVal Temp1( 37, "°C" );
      REQ( Temp1.get_abs(), == , 37 );
      REQ( Temp1.is_minus(), == , false );
      REQ( Temp1.get_sign(), == , '\0' );
      REQ( Temp1.get_engineer(), == , 37 );
      REQ( Temp1.get_reminder(), == , 0 );
      REQ( Temp1.get_mult(), ==, 1 );
      REQ( Temp1.get_base(), ==, std::string("°C") );
      REQ( Temp1.get_gap(), == , std::string("") );
      REQ( Temp1.get_prefix(), == , '\0' );

      static UnitVal Temp2( -274, "°C" );
      REQ( Temp2.get_abs(), == , 274 );
      REQ( Temp2.is_minus(), == , true );
      REQ( Temp2.get_sign(), == , '-' );
      REQ( Temp2.get_base(), ==, std::string("°C") );
      REQ( Temp2.get_gap(), == , std::string("") );
      REQ( Temp2.get_prefix(), == , '\0' );
    } catch( const std::exception& e )
    {
      puts( e.what() );
      REQ( 1, == , 2 );
    }

    NoPF_Set::EngineeringDecimals = EngineeringDecimals_old;
  };

  SUB( Value_in_a_Loop_with_no_prefixing )
  {
// NoPF_Set::EngineeringDecimals // 2 -> "230.00 V" *5 = "1.15 kV" /100 = "11.50 V" /100 = "0.11 mV"
// NoPF_Set::DecimalsDelimitter // '.'
    const char Prefix[] = "qryzafpnµm\0kMGTPEZYRQ";
    char const *pPrefix = Prefix + sizeof(Prefix)/2; // point to middle/nothing
    std::stringstream Required;
    long long Power(9), Work(0);
    unsigned short Day(0), Hours(12);

    for( Day = 1; Day <= 365 ; Day++ )
    {
      if( Day%28 ) Power*=10;
      MUST( Power <= static_cast<long long>(__FLT_MAX__) );
      float fPower = float(Power);
      Hours += ((Day%5)-2);
      Work = Power * Hours;
      MUST( Work <= static_cast<long long>(__DBL_MAX__) );
      double fWork = double(Work);

      Required.str( std::string() );
      NoPrintf ElectricalSimple;

      const std::string WattHours("Wh");
      const std::string Watt("W");
      std::string WattHours_Req(WattHours), WattHours_Act(WattHours);
      std::string Watt_Req(Watt), Watt_Act(Watt);

      pPrefix = Prefix + sizeof(Prefix)/2;
      MUST( (*pPrefix == '\0') && ( pPrefix >= Prefix ) && ( pPrefix < Prefix + sizeof(Prefix) ) );
      while(fWork>=1000.0000)
      {
        fWork/=1000.0;
        pPrefix++;
        MUST( pPrefix >= Prefix );
        MUST( pPrefix < Prefix + sizeof(Prefix) );
        WattHours_Req = std::string( 1, *pPrefix ) + WattHours;
      }
      while(fWork < 1.0)
      {
        fWork*=1000.0;
        pPrefix--;
        MUST( pPrefix >= Prefix );
        MUST( pPrefix < Prefix + sizeof(Prefix) );
        WattHours_Req = std::string( 1, *pPrefix ) + WattHours;
      }

      pPrefix = Prefix + sizeof(Prefix)/2;
      MUST( (*pPrefix == '\0') && ( pPrefix >= Prefix ) && ( pPrefix < Prefix + sizeof(Prefix) ) );
      while(fPower>=1000.0000)
      {
        fPower/=1000.0;
        pPrefix++;
        MUST( pPrefix >= Prefix );
        MUST( pPrefix < Prefix + sizeof(Prefix) );
        Watt_Req = std::string( 1, *pPrefix ) + Watt;
      }
      while(fPower < 1.0)
      {
        fPower*=1000.0;
        pPrefix--;
        MUST( pPrefix >= Prefix );
        MUST( pPrefix < Prefix + sizeof(Prefix) );
        Watt_Req = std::string( 1, *pPrefix ) + Watt;
      }

      Required << std::setprecision(NoPF_Set::EngineeringDecimals) << std::fixed;
      Required << "At day " << unsigned(Day) << " the " << unsigned(Hours) << " h work=" << fWork << " " << WattHours_Req << " (@ " << fPower << " " << Watt_Req << ").";

      ElectricalSimple("At day $1 the $2 h work=$3 (@ $4).").arg(Day).arg(Hours).val(Work,WattHours_Act).val(Power,Watt_Act);
      REQ( ElectricalSimple.get(), ==, Required.str() );
      //MUST( ElectricalSimple.get() == Required.str() );
    }
  };
} // end - TEST( raw_is_like_arg )



#if defined(NOPF_USE_DATA_ANALYSIS) && (NOPF_USE_DATA_ANALYSIS)
REPORT()
{
  REPORT_FAILED_TESTS();
  REPORT_PASS_RATE();
  REPORT_AVG_TIME();
}

#else // defined(NOPF_USE_DATA_ANALYSIS) && (NOPF_USE_DATA_ANALYSIS)

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
#endif // defined(NOPF_USE_DATA_ANALYSIS) && (NOPF_USE_DATA_ANALYSIS)
