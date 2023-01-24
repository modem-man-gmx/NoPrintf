#include <cstdlib>
#include <cstring>
#include <algorithm>

#ifdef DEBUG  // only for testing
#  include <iostream>
#endif // DEBUG

#include "no_printf.hpp"


NoPrintf::NoPrintf( NoPrintf&& other )                 // move CTor
{
  std::swap(m_str,other.m_str);
  std::swap(m_args,other.m_args);
}


NoPrintf::NoPrintf( NoPrintf const& other )            // copy CTor
{
  m_str = other.m_str;
  m_args= other.m_args;
}


NoPrintf& NoPrintf::operator=( NoPrintf&& other )      // move assignment
{
  if( &other != this )
  {
    std::swap(m_str,other.m_str);
    std::swap(m_args,other.m_args);
  }
  return *this;
}


NoPrintf& NoPrintf::operator=( NoPrintf const& other ) // copy assignment
{
  if( &other != this )
  {
    m_str = other.m_str;
    m_args= other.m_args;
  }
  return *this;
}



// todo: check, if C++14 does support nested CTors and if Platform.IO ESP supports C++14
// otherwise kind of default init possible?
NoPrintf::NoPrintf() : m_str(), m_args()
{
  init();
}


// string move/swap CTor
NoPrintf::NoPrintf( std::string&& str )
{
  init();
  std::swap(m_str,str);
}


NoPrintf::NoPrintf( std::string const& str ) : m_str( str ), m_args()
{
  init();
}


NoPrintf::NoPrintf( const char* txt ) : m_str( txt?txt:"" ), m_args()
{
  init();
}


// ----------- impl ---------------


void NoPrintf::init()  // single '$' sign at offset 0 for '$$'
{
  m_args.reserve(10);
  m_args.clear();
  m_args.emplace(m_args.end(), std::move(std::string("$"))); // single '$' sign at offset 0 for '$$'
}


void NoPrintf::clean()
{
  init();
  m_str.clear();
}


NoPrintf& NoPrintf::append( const std::string& str )
{
  init(); // no arguments until end of append
  m_str += str;
  return *this;
}


NoPrintf& NoPrintf::set( const std::string& str )
{
  init(); // no arguments until end of set
  m_str = str;
  return *this;
}


const std::string& NoPrintf::get_ref()
{
  size_t argnum = m_args.size();

  for( size_t i=0 ; i<10 && i<argnum ; i++ )
  {
    char placehold[3] = "$x";
    placehold[1] = '0' + i;
    if(0==i) placehold[1] = '$'; // for replacing $$ by single $
    size_t offs=0;

    do
    {
      size_t ph_len = strlen(placehold);
      offs = m_str.find( placehold, offs );
      if( std::string::npos != offs )
      {
        m_str.replace( offs, ph_len, m_args[ i ] );
        offs += ph_len; // or better += m_args[ i ].length() to skip over the newly inserted part?
      }
    } while( std::string::npos != offs );
  }

  return m_str;
}


// do we need the distinction between ref and copy? in C++14 we have RVO, in 17 it is mandatory ... depends on what we have at ESP/PlatformIO ...
std::string NoPrintf::get()
{
  std::string res = this->get_ref();
  return res;
}


// give an functor as argument with kind of puts() as default?
NoPrintf& NoPrintf::put( /*optionally: give function pointer?*/ )
{
  puts( this->get_ref().c_str() );
  return *this;
}


NoPrintf& NoPrintf::operator +=(const NoPrintf& rhs)
{
  m_str += rhs.m_str;
  // m_args will not be joined here, because args shall only be give right befor put().
  return *this;
}


NoPrintf& NoPrintf::operator +=(const std::string& rhs)
{
  m_str += rhs;
  return *this;
}


NoPrintf& NoPrintf::operator +=(const char* rhs)
{
  if(rhs)
    m_str += std::string(rhs);
  return *this;
}


NoPrintf& NoPrintf::arg(const std::string& str)
{
  //m_args.push_back(str);
  m_args.emplace(m_args.end(), std::move(str));

# if defined(DEBUG) && 0  // only for testing
  std::cout << "args=" << m_args.size() << std::endl;
  for( auto a : m_args )
  { std::cout << a << ", ";
  }
  std::cout << "\n";
# endif
  return *this;
}


NoPrintf& NoPrintf::arg(const char* txt)
{
  if(txt)
    { return this->arg( std::string(txt) );
    }
  return *this;
}


// a hopefully smaller and faster implementation compared with ltoa() and to_string()
static std::string& collect_int( unsigned int uVal, std::string& buffer, bool Minus = false )
{
  if( uVal==0 )
  {
    buffer = "0";
  }
  else
  {
    while( uVal != 0 )
    {
      buffer.push_back( '0' + (uVal % 10) );
      uVal /= 10;
    }
    if( Minus )
    { buffer.push_back('-');
    }
    std::reverse( buffer.begin(), buffer.end() );
  }
  return buffer;
}


// need to introduce template and template specification soon
NoPrintf& NoPrintf::arg( unsigned long int uVal )
{
  std::string collect;
  return this->arg( collect_int( uVal, collect ) );
}


NoPrintf& NoPrintf::arg( signed long int iVal )
{
  std::string collect;
  if(iVal<0)
    return this->arg( collect_int( iVal * -1, collect, true ) );
  else
    return this->arg( collect_int( iVal, collect ) );
}


