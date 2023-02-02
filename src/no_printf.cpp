#include <algorithm>
#include <cstdlib>
#include <cstring>

#ifdef DEBUG          // only for testing
#  include <iostream> // cerr
#  include <ostream>  // endl
//#  include <signal.h> // only for debugging and not needed on all platforms
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


void NoPrintf::init()
{
  m_args.reserve( 1 + 1 + 9 );
  m_args.clear();
  // a failure hint "<invalid>" offset 0 for '$$'
  m_args.emplace( m_args.end(), std::move( std::string( "<invalid>" ) ) ); // for non-existing $x indices
  // single '$' sign at offset 1 for '$$'
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
    //#error "work.replace() invalidates the offset and perhaps the length"
    char   placehold_chr = work[ offs + 1 ];
    size_t placehold_idx = 1 + placehold_chr - '0'; // $$ -> idx 1, $1 -> idx 2
    if( placehold_idx >= m_args.size() )            // -> set to "<invalid>"
    {
      placehold_idx = 0;
    }

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
        //std::cerr << "Work before:\"" << work << "\", len=" << work.length() << ", idx =" << placehold_idx << std::endl;
        work.replace( offs, 2, m_args[ placehold_idx ] );
        //std::cerr << "Work after :\"" << work << "\", len=" << work.length() << ", offs=" << offs << std::endl;
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
// all values handled as positive long unsigned, the Minus is just an extra flag
// width: INT_MIN (default) : do not use format-fillup or cut, same as if(Minus) {printf("-");} printf("%lu"),uVal);
// width: positive value    : use right align and prefill with 0000. Same as if(Minus) {printf("-");} printf("%.7lu"),uVal);
// width: negative value    : use left  align and postfill with ' '. Same as if(Minus) {printf("-");} printf("%-7lu"),uVal);
// width: 0                 : just suppress value 0. Same as                 if(Minus) {printf("-");} printf("%.0lu"),uVal);
std::string& NoPrintf::collect_int( unsigned long int uVal, std::string& buffer, bool Minus, int width )
{
  bool bRightAlign = ( width > 0 ) ? true : false;
  bool bLeftAlign = false;
  if( width > INT_MIN && width < 0 )
  {
    bLeftAlign = true;
    width *= -1;
  }
  int filling = width;

  if( 0lu == uVal && 0 == width ) // emulate printf("%.0ld"),uVal)
  {
    buffer.clear();
    return buffer;
  }
  else if( 0lu == uVal ) // not yet return, format filling could take place!
  {
    buffer = "0";
    filling--;
  }

  {
    while( uVal != 0 )
    {
      buffer.push_back( '0' + ( uVal % 10 ) );
      uVal /= 10;
      filling--;
    }

    if( Minus ) { filling--; }

    if( bRightAlign && filling > 0 ) { buffer.append( filling, '0' ); }

    if( Minus ) { buffer.push_back( '-' ); }
  }
  std::reverse( buffer.begin(), buffer.end() );

  if( bLeftAlign && filling > 0 ) { buffer.append( filling, ' ' ); }

  return buffer;
}
