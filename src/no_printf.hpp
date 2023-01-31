#ifndef NO_PRINTF_CLASS
#define NO_PRINTF_CLASS

#include <cstddef>
#include <string>
#include <vector>

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
  NoPrintf& arg( const T& val )
  {
    std::string collect;
    if( val < 0 )
    {
      return this->arg( collect_int( val * -1, collect, true ) );
    }
    else
    {
      return this->arg( collect_int( val, collect ) );
    };
  }

  //template<typename fake>
  NoPrintf& arg( const std::string& str )
  {
    m_args.emplace( m_args.end(), std::move( str ) );
    return *this;
  };

  //template<typename fake>
  NoPrintf& arg( const char* txt )
  {
    if( nullptr != txt )
    {
      return this->arg( std::string( txt ) );
    }
    return *this;
  };

  //template<typename fake>
  NoPrintf& arg( unsigned long int uVal )
  {
    std::string collect;
    return this->arg( collect_int( uVal, collect ) );
  };


private:
  std::string& collect_int( unsigned long int uVal, std::string& buffer, bool Minus = false );

private:
  std::string              m_str;
  std::vector<std::string> m_args;
};


//} // namespace

#endif //ndef NO_PRINTF_CLASS
