#ifndef NO_PRINTF_CLASS
#define NO_PRINTF_CLASS

#include <no_printf_customizing.h>

#include <climits>
#include <cstddef>
#include <string>
#include <vector>

//namespace No {


struct NoPF_Set // static Settings
{
  static char
      FillCharAlignLeft; //   = NOPF_NUMERICAL_LEFTALIGN_FILLCHAR    -- do not change this to numericals, would get postpending '0000' so number could get wrong
  static char
      FillCharAlignRight; //  = NOPF_NUMERICAL_RIGHTALIGN_FILLCHAR   -- printf would use a '0', but printf also supports other outdated 0001234+ 000789- formatting ...
  static bool
      FillCharScience; //     = NOPF_SCIENTIFICALLY_CORRECT_SPACING  -- true: "14 km/h, 230 V, 37°C", false: "14km/h, 230V, 37°C"
  static int
      EngineeringDecimals; // = NOPF_ENGINNERING_DECIMALS_DEFAULT    -- 2 -> "230.00 V" *5 = "1.15 kV" /100 = "11.50 V" /100 = "0.11 mV"
  static char
      DecimalsDelimitter; //  = NOPF_ENGINNERING_DECIMALS_DELIMITTER -- '.' for most English languages, ',' for Germany. ToDo: set locale-dependent?
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
  void init();
  void clean();
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
    const int   decimals( 0 ); // no decimals for integral integer types
    std::string collect;
    if( val < 0 )
    {
      // because |INT_MIN| > INT_MAX, or in C: abs(INT_MIN)==INT_MAX+1, the conversion takes part in signed
      // number type room, but the result will only fit in unsigned same number type room.
      // so we do a (x - 1 + 1) operation to trick the C++ type system, trusting the compiler to not
      // really doing this operation...
      return this->arg(
          collect_number( static_cast<BiggestNumerical_t>( ( val + 1 ) * -1 ) + 1, collect, true, decimals, width ) );
    }
    else
      return this->arg( collect_number( static_cast<BiggestNumerical_t>( val ), collect, false, decimals, width ) );
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
    return this->arg( collect_number( uVal, collect, false, 0, width ) );
  };

  template<typename T>
  NoPrintf& val( const T& val, const char* UnitAbbrev = "", int decimals = NoPF_Set::EngineeringDecimals )
  {
    return this->val( val, std::string( UnitAbbrev ), decimals );
  }

  template<typename T>
  NoPrintf& val( const T& val, const std::string& Unit = std::string( "" ), int decimals = NoPF_Set::EngineeringDecimals )
  {
    std::string degree_sign( "\u00b0" );
    std::string collect, phys_unit;
    // all units but "°C" (or empties) must have a space after the number (if scientifically correct notation is true)
    if( NoPF_Set::FillCharScience && Unit.length() && 0 != Unit.find( degree_sign ) ) phys_unit += " ";
    phys_unit += Unit;

    // unix prefix and value aligning between 0.01 and 999.99 to come ...

    if( val < 0 )
      return this->arg(
          collect_number( static_cast<BiggestNumerical_t>( ( val + 1 ) * -1 ) + 1, collect, true, decimals, INT_MIN ) +
          phys_unit );
    else
      return this->arg( collect_number( static_cast<BiggestNumerical_t>( val ), collect, false, decimals, INT_MIN ) +
                        phys_unit );
  }


private:
  std::string& collect_number( BiggestNumerical_t uVal, std::string& buffer, bool Minus = false, int decimals = 0,
                               int width = INT_MIN );

private:
  std::string              m_str;
  std::vector<std::string> m_args;
};


//} // namespace

#endif //ndef NO_PRINTF_CLASS
