#ifndef NO_PRINTF_CLASS
#define NO_PRINTF_CLASS

#include <string>
#include <vector>

class NoPrintf
{
public:
  NoPrintf();
  ~NoPrintf() {};

  NoPrintf( std::string&& str );       // string move/swap CTor
  NoPrintf( std::string const& str );  // string CTor
  NoPrintf( const char* txt );         // text pointer CTor

  NoPrintf( NoPrintf&& other );                 // move CTor
  NoPrintf( NoPrintf const& other );            // copy CTor

  NoPrintf& operator=( NoPrintf&& other );      // move assignment
  NoPrintf& operator=( NoPrintf const& other ); // copy assignment

public: // further public methods
  void init();
  void clean();
  //void trim();
  //void cut(siz_t max);

  NoPrintf& set( const std::string& str );
  NoPrintf& append( const std::string& str );
  NoPrintf& put();
  const std::string& get_ref();
  std::string get();

  NoPrintf& arg( const std::string& str );
  NoPrintf& arg( const char* txt );
  NoPrintf& arg( unsigned long int uVal );
  NoPrintf& arg( signed long int iVal );


  NoPrintf& operator+=( const NoPrintf& rhs );
  NoPrintf& operator+=( const std::string& rhs );
  NoPrintf& operator+=( const char* rhs );

private:
  std::string m_str;
  std::vector<std::string> m_args;
};

#endif //ndef NO_PRINTF_CLASS
