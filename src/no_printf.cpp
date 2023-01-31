#include <algorithm>
#include <cstdlib>
#include <cstring>

#ifdef DEBUG // only for testing
#  include <iostream>
#endif // DEBUG

#include "no_printf.hpp"


NoPrintf::NoPrintf()
    : m_str()
    , m_args()
{
  init();
}


// string move/swap CTor
NoPrintf::NoPrintf( std::string&& str )
    : NoPrintf()
{
  std::swap( m_str, str );
}


// string assign/copy CTor
NoPrintf::NoPrintf( std::string const& str )
    : NoPrintf()
{
  m_str = str;
}


// plain char pointer CTor, using CTor Delegation with temporary string
NoPrintf::NoPrintf( const char* txt )
    : NoPrintf( std::string( txt ? txt : "" ) )
{
}


// move CTor, no CTor Delegation, because empty containers are well enough for MOVE
NoPrintf::NoPrintf( NoPrintf&& other )
    : m_str()
    , m_args()
{
  std::swap( m_str, other.m_str );
  std::swap( m_args, other.m_args );
}


// copy CTor, no CTor Delegation, because containers will inherit the content from other
NoPrintf::NoPrintf( NoPrintf const& other )
{
  m_str = other.m_str;
  m_args = other.m_args;
}


NoPrintf& NoPrintf::operator=( NoPrintf&& other ) // move assignment
{
  if( &other != this )
  {
    std::swap( m_str, other.m_str );
    std::swap( m_args, other.m_args );
  }
  return *this;
}


NoPrintf& NoPrintf::operator=( NoPrintf const& other ) // copy assignment
{
  if( &other != this )
  {
    m_str = other.m_str;
    m_args = other.m_args;
  }
  return *this;
}


// ----------- impl ---------------


void NoPrintf::init() // single '$' sign at offset 0 for '$$'
{
  m_args.reserve( 10 );
  m_args.clear();
  m_args.emplace( m_args.end(), std::move( std::string( "$" ) ) ); // single '$' sign at offset 0 for '$$'
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


// minimum compiler: gnu++11 or C++14, to have RVO (return value optimisation)
std::string NoPrintf::get() const
{
  // ToDo: do not work on a string, just use a growing list of iterators pointing to the elements, then spool out the list-refrences?
  std::string work( m_str );

  size_t offs = work.find( '$' );
  while( std::string::npos != offs && work.length() > offs )
  {
    char   placehold_chr = work[ offs + 1 ];
    size_t placehold_idx = placehold_chr - '0';

    // if you feel this could better be an if(range1)-elif(range2)-endif construct, think of easy extension to $0, $a..$z. If you still ... do benchmarks.
    switch( placehold_chr )
    {
      case '$':
      {
        work.erase( offs + 1, 1 );
        offs++;
      };
      break;
      case '1': // fallthrough
      case '2': // fallthrough
      case '3': // fallthrough
      case '4': // fallthrough
      case '5': // fallthrough
      case '6': // fallthrough
      case '7': // fallthrough
      case '8': // fallthrough
      case '9':
      {
        work.replace( offs, 2, m_args[ placehold_idx ] );
        offs += m_args[ placehold_idx ].length();
      };
      break;
      default:
      {
        offs++;
      };
      break;
    }
    offs = work.find( '$', offs ); // find next $
  }

  return work;
}


// give an functor as argument with kind of puts() as default?
NoPrintf& NoPrintf::put( /*optionally: give function pointer?*/ )
{
  puts( this->get().c_str() );
  return *this;
}


std::string NoPrintf::operator=( const NoPrintf& lhs ) const
{
  return lhs.get();
}


NoPrintf& NoPrintf::operator+=( const NoPrintf& rhs )
{
  m_str += rhs.m_str;
  // m_args will not be joined here, because args shall only be give right befor put().
  return *this;
}


NoPrintf& NoPrintf::operator+=( const std::string& rhs )
{
  m_str += rhs;
  return *this;
}


NoPrintf& NoPrintf::operator+=( const char* rhs )
{
  if( rhs ) m_str += std::string( rhs );
  return *this;
}


// a hopefully smaller and faster implementation compared with ltoa() and to_string()
std::string& NoPrintf::collect_int( unsigned long int uVal, std::string& buffer, bool Minus )
{
  if( uVal == 0 ) { buffer = "0"; }
  else
  {
    while( uVal != 0 )
    {
      buffer.push_back( '0' + ( uVal % 10 ) );
      uVal /= 10;
    }
    if( Minus ) { buffer.push_back( '-' ); }
    std::reverse( buffer.begin(), buffer.end() );
  }
  return buffer;
}
