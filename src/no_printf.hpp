#ifndef NO_PRINTF_CLASS
#define NO_PRINTF_CLASS

#include <climits>
#include <cstddef>
#include <string>
#include <vector>

#include <no_printf_customizing.h>

//namespace No {

template<typename T>
struct type2type
{
  typedef T type; // dirty hack to teach g++ the template specialisation for methods
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
  NoPrintf&   operator+=( const NoPrintf& rhs );
  NoPrintf&   operator+=( const std::string& rhs );
  NoPrintf&   operator+=( const char* rhs );

public: // further public methods
  void init();
  void clean();
  //void trim();
  //void cut(siz_t max);

  NoPrintf&   set( const std::string& str );
  NoPrintf&   append( const std::string& str );
  NoPrintf&   put();
  std::string get() const;

  //template<typename T, typename fake=void>
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
      return this->arg( collect_number( static_cast<BiggestNumerical_t>( (val+1) * -1 ) + 1, collect, true, width ) );
    }
    else { return this->arg( collect_number( static_cast<unsigned long int>( val ), collect, false, width ) ); };
  }

  NoPrintf& arg( const std::string& str, int width = 0, char fillchr = ' ' )
  {
    // if width <0 right handed aligned, cut or fill left side
    // if width >0 left handed aligned, cut or fill right
    m_args.emplace( m_args.end(), std::move( str ) );
    return *this;
  };

  NoPrintf& arg( const char* txt, int width = 0, char fillchr = ' ' )
  {
    if( nullptr != txt ) { return this->arg( std::string( txt ), width, fillchr ); }
    return *this;
  };


  NoPrintf& arg( unsigned long int uVal, int width = INT_MIN )
  {
    std::string collect;
    return this->arg( collect_number( uVal, collect, false, width ) );
  };


private:
  std::string& collect_number( BiggestNumerical_t uVal, std::string& buffer, bool Minus = false, int width = INT_MIN );

private:
  std::string              m_str;
  std::vector<std::string> m_args;
};


//} // namespace

#endif //ndef NO_PRINTF_CLASS
