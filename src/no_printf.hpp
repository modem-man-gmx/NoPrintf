#ifndef NO_PRINTF_CLASS
#define NO_PRINTF_CLASS

#include <no_printf_customizing.h>

#include <cassert>
#include <climits>
#include <cmath> // pow
#include <cstddef>
#include <string>
#include <vector>

//namespace No {


struct NoPF_Set // static Settings
{
  // NOPF_NUMERICAL_LEFTALIGN_FILLCHAR    -- do not change this to numericals, would get postpending '0000' so number could get wrong
  static char FillCharAlignLeft;
  // NOPF_NUMERICAL_RIGHTALIGN_FILLCHAR   -- printf would use a '0', but printf also supports other outdated 0001234+ 000789- formatting ...
  static char FillCharAlignRight;
  // NOPF_SCIENTIFICALLY_CORRECT_SPACING  -- true: "14 km/h, 230 V, 37°C", false: "14km/h, 230V, 37°C"
  static bool FillCharScience;
  // NOPF_ENGINNERING_DECIMALS_DEFAULT    -- 2 -> "230.00 V" *5 = "1.15 kV" /100 = "11.50 V" /100 = "0.11 mV"
  static int EngineeringDecimals;
  // NOPF_ENGINNERING_DECIMALS_DELIMITTER -- '.' for most English languages, ',' for Germany. ToDo: set locale-dependent?
  static char DecimalsDelimitter;
};

// international scientific 10^(3*n) prefixes with -10 <= n <= 10 (wich can't be used all on usual C++ integrated types)
#define NOPF_PREFIX_ARRAY "qryzafpnµm\0kMGTPEZYRQ"

/*
struct unsigned_PhysicalQuantity
{
  BiggestNumerical_t val;
  std::string unit;
}

struct signed_PhysicalQuantity
{
  BiggestSigned_t val;
  std::string unit;
}
*/

class UnitVal
{
public:
  UnitVal() = delete;
  UnitVal( BiggestNumerical_t RawValue, const std::string& BaseUnit = std::string( "" ) )
      : m_optimized( RawValue<1000 )
      , m_isBaseUnit( true )
      , m_isMinus( false )
      , m_value( RawValue )
      , m_multiply( 1 ) // not a denominator! it is 1000 for "k", 1000000 for "G", ...
      , m_baseunit( BaseUnit )
      , m_prefix( NOPF_PREFIX_ARRAY )
      , m_pPrefixSelector( m_prefix + sizeof( NOPF_PREFIX_ARRAY ) / 2 ) {};

  template<typename T>
  UnitVal( T RawValue, const std::string& BaseUnit = std::string( "" ) )
      : UnitVal( static_cast<BiggestNumerical_t>( ( RawValue < 0 ) ? ( ( ( RawValue + 1 ) * -1 ) + 1 ) : RawValue ),
                 BaseUnit ) // CTor delegation
  {
    m_isMinus = ( RawValue < 0 );
  };

  ~UnitVal() = default;

  const std::string& get_baseunit() const { return m_baseunit; };

  bool is_negative() const { return m_isMinus; };
  char get_sign( bool plus_on_positive = false ) const { return ( m_isMinus ) ? '-' : ( plus_on_positive ) ? '+' : '\0'; };
  const char* get_gap() const
  {
    return ( m_baseunit.length() && NoPF_Set::FillCharScience && ( 0 != m_baseunit.find( std::string( "\u00b0" ) ) ) ) ? " "
                                                                                                                       : "";
  };
  std::string get_gapstr() { const char c = *get_gap(); if('0'==c) return std::string(""); else return std::string(1,c); };
  BiggestNumerical_t get_abs() const { return m_value; };
  BiggestSigned_t    get_signed() const // can throw!
  {
    if( m_value > NoPF_MaxSignedVal )
      throw std::runtime_error( "BiggestSigned_t" );
    return static_cast<BiggestSigned_t>(m_value);
  };

  char get_prefix() { if(!m_optimized) optimize(); return *m_pPrefixSelector; };
  std::string get_prefixstr()
  {
    char c = get_prefix();
    if('\0'==c) return std::string("");
    else return std::string(1,c);
  };
  BiggestNumerical_t get_multiplier() { if(!m_optimized) optimize(); return m_multiply; };

  short get_engineers() { if(!m_optimized) optimize(); return static_cast<short>(m_value / (m_multiply * (m_isMinus?-1:1))); };

  std::string get_engineers_unit() { if(!m_optimized) {optimize();}; return get_gapstr() + get_prefixstr() + get_baseunit(); };

  BiggestNumerical_t get_engineers_reminder(int digits=2)
  {
    if(!m_optimized) optimize();
    BiggestNumerical_t reminder = m_value % m_multiply;
    // trim to digits
    BiggestNumerical_t limit = pow( 10, digits ) - 1;
    while(reminder/10 > limit)
      reminder/=10;
    reminder+=5;
    reminder/=10;
    return reminder;
  };

private:
  void optimize() // changes prefix and multiplier
  {
    m_pPrefixSelector = m_prefix + sizeof( NOPF_PREFIX_ARRAY )/2;
    assert( *m_pPrefixSelector == '\0' );

    // unix prefix and value aligning between 0.01 and 999.99 ...
    while( (m_value / m_multiply) >= 1000 )
    {
      m_multiply *= 1000;
      m_pPrefixSelector++;
      assert( m_pPrefixSelector >= m_prefix );
      assert( m_pPrefixSelector < m_prefix + sizeof( NOPF_PREFIX_ARRAY ) );
    }
    m_optimized = true;
    return;
  };

private:
  bool               m_optimized;
  bool               m_isBaseUnit;
  bool               m_isMinus;
  BiggestNumerical_t m_value;
  BiggestNumerical_t m_multiply;
  std::string        m_baseunit;
  const char         m_prefix[ 1 + sizeof( NOPF_PREFIX_ARRAY ) ];
  char const*        m_pPrefixSelector;
};


class NoPrintf
{
public:
#if defined( NOPRINTF_CAN_OVERLOADED )
  virtual
#endif
      ~NoPrintf() {};

  NoPrintf();
  NoPrintf( NoPrintf&& other );       // move CTor
  NoPrintf( NoPrintf const& other );  // copy CTor
  NoPrintf( std::string&& str );      // string move/swap CTor
  NoPrintf( std::string const& str ); // string CTor
  NoPrintf( const char* txt );        // text pointer CTor

public:                                           // operators
  NoPrintf&   operator=( NoPrintf&& other );      // move assignment
  NoPrintf&   operator=( NoPrintf const& other ); // copy assignment
  std::string operator=( const NoPrintf& lhs ) const;
  NoPrintf&   operator+=( const NoPrintf& rhs ) = delete;
  NoPrintf&   operator+=( const std::string& rhs );
  NoPrintf&   operator+=( const char* rhs );

public: // further public methods
  NoPrintf& init();
  NoPrintf& clean();
  //void trim();
  //void cut(siz_t max);

  NoPrintf&   operator()( const std::string& str ) { return set( str ); };
  NoPrintf&   operator()( const char* txt ) { return set( std::string( txt ? txt : "<nullptr>" ) ); };
  NoPrintf&   set( const std::string& str );
  NoPrintf&   append( const std::string& str );
  NoPrintf&   put();
  std::string get() const;

  template<typename T>
  NoPrintf& arg( const T& val, int width = INT_MIN )
  {
    std::string collect;
    if( val < 0 )
    {
      // because |INT_MIN| > INT_MAX, or in C: abs(INT_MIN)==INT_MAX+1, the conversion takes part in signed
      // number type room, but the result will only fit in unsigned same number type room.
      // so we do a (x - 1 + 1) operation to trick the C++ type system, trusting the compiler to not
      // really doing this operation...
      return this->arg(
          collect_number( static_cast<BiggestNumerical_t>( ( val + 1 ) * -1 ) + 1, collect, true, width ) );
    }
    else
      return this->arg( collect_number( static_cast<BiggestNumerical_t>( val ), collect, false, width ) );
  }

  NoPrintf& raw( const std::string& str, int width = 0, char fillchr = ' ' )
  {
    // if width <0 right handed aligned, cut or fill left side
    // if width >0 left handed aligned, cut or fill right
    m_args.emplace( m_args.end(), std::move( str ) );
    return *this;
  };

  NoPrintf& raw( const char* txt, int width = 0, char fillchr = ' ' )
  {
    if( nullptr != txt ) { return this->arg( std::string( txt ), width, fillchr ); }
    return *this;
  };

  NoPrintf& arg( const std::string& str, int width = 0, char fillchr = ' ' )
  {
    // until implementation of translation, this is just a 1:1 wrap
    return this->raw( str, width, fillchr );
  };

  NoPrintf& arg( const char* txt, int width = 0, char fillchr = ' ' )
  {
    // until implementation of translation, this is just a 1:1 wrap
    return this->raw( txt, width, fillchr );
  };

  NoPrintf& arg( unsigned long int uVal, int width = INT_MIN )
  {
    std::string collect;
    return this->arg( collect_number( uVal, collect, false, width ) );
  };

  template<typename T>
  NoPrintf& val( const T& val, const char* UnitAbbrev = "", int decimals = NoPF_Set::EngineeringDecimals )
  {
    return this->val( val, std::string( UnitAbbrev ), decimals );
  }

  template<typename T>
  NoPrintf& val( const T& val, const std::string& Unit = std::string( "" ), int decimals = NoPF_Set::EngineeringDecimals )
  {
    // unix prefix and value aligning between 0.01 and 999.99 ...
    UnitVal Engineers( val, Unit );
    std::string WholeNum, DecimalNum;
    collect_number( Engineers.get_engineers(), WholeNum, Engineers.is_negative(), INT_MIN );
    if( 0==decimals )
    {
      return this->arg( WholeNum + Engineers.get_engineers_unit() );
    }
    collect_number( Engineers.get_engineers_reminder(decimals), DecimalNum, false, 0-decimals, '0' );
    return this->arg( WholeNum + "." + DecimalNum + Engineers.get_engineers_unit() );
  }


private:
  std::string& collect_number( BiggestNumerical_t uVal, std::string& buffer, bool Minus = false, int width = INT_MIN, char override_fill = '\0' );

private:
  std::string              m_str;
  std::vector<std::string> m_args;
};

//} // namespace

#endif //ndef NO_PRINTF_CLASS
