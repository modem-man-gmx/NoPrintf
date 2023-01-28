#include <string>
#include <limits>
#include "no_printf.hpp"

#include "lightest/lightest.h"
//#define __FILE__ "test.cpp"


CONFIG(Configurations) {
  for (; argn > 0; argn--, argc++)
  {
    if (std::string(*argc) == "--no-color") NO_COLOR();
    if (std::string(*argc) == "--no-output") NO_OUTPUT();
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

TEST(Default_NoPrintf_handling)
{
  NoPrintf Default;
  MUST(
  REQ(  Default.get(), ==, std::string() )
  );
  REQ(  Default.get(), ==, std::string("") );
  REQ( *Default.get().c_str(), ==, '\0' );
}

TEST(Can_create_Empty_Strings)
{
  SUB( EmptyString )
  {
    NoPrintf EmptyString("");
    REQ(  EmptyString.get(), ==, std::string() );
    REQ(  EmptyString.get(), ==, std::string("") );
    REQ( *EmptyString.get().c_str(), ==, '\0' );
    REQ(  EmptyString.get().c_str(), !=, static_cast<const char*>(nullptr) );
  };

  SUB( EmptyString_ignores_args )
  {
    NoPrintf EmptyString("");
    EmptyString.arg("invisible").arg(0L);
    REQ(  EmptyString.get(), ==, std::string() );
    REQ(  EmptyString.get(), ==, std::string("") );
    REQ( *EmptyString.get().c_str(), ==, '\0' );
    REQ(  EmptyString.get().c_str(), !=, static_cast<const char*>(nullptr) );
  };

  SUB( Anonymous_default_is_empty )
  {
    REQ(  NoPrintf().get(), ==, std::string() );
    REQ(  NoPrintf().get(), ==, std::string("") );
    REQ( *NoPrintf().get().c_str(), ==, '\0' );
    REQ(  NoPrintf().get().c_str(), !=, static_cast<const char*>(nullptr) );
  };

  SUB( Anonymous_empty_is_empty )
  {
    const char *nix="\0";
    std::string leer("");
    std::string deflt;
    REQ(  NoPrintf( ""  ).get(), ==, std::string() );
    REQ(  NoPrintf( nix ).get(), ==, std::string("") );
    REQ( *NoPrintf( leer).get().c_str(), ==, '\0' );
    REQ(  NoPrintf(deflt).get().c_str(), !=, static_cast<const char*>(nullptr) );
  };

  SUB( Anonymous_empty_ignores_args )
  {
    REQ(  NoPrintf().arg("invisible").arg(0L).get(), ==, std::string() );
    REQ(  NoPrintf().arg("invisible").arg(0L).get(), ==, std::string("") );
    REQ( *NoPrintf().arg("invisible").arg(0L).get().c_str(), ==, '\0' );
    REQ(  NoPrintf().arg("invisible").arg(0L).get().c_str(), !=, static_cast<const char*>(nullptr) );
  };

  SUB( Numerical_int )
  {
    REQ( std::string("a) Integer A=0, B=1, C=-1."), ==, NoPrintf("a) Integer A=$1, B=$2, C=$3.")
                                                       .arg(static_cast<int>(0))
                                                       .arg(static_cast<int>(1))
                                                       .arg(static_cast<int>(-1))
                                                       .get() );
    REQ( std::string("b) Integer A=0, B=255."), ==, NoPrintf("b) Integer A=$1, B=$2.")
                                                       .arg(static_cast<int>(std::numeric_limits<uint8_t>::min()))
                                                       .arg(static_cast<int>(std::numeric_limits<uint8_t>::max()))
                                                       .get() );
    REQ( std::string("c) Integer A=-128, B=127."), ==, NoPrintf("c) Integer A=$1, B=$2.")
                                                       .arg(static_cast<int>(std::numeric_limits<int8_t>::min()))
                                                       .arg(static_cast<int>(std::numeric_limits<int8_t>::max()))
                                                       .get() );
    REQ( std::string("d) Integer A=0, B=65535."), ==, NoPrintf("d) Integer A=$1, B=$2.")
                                                       .arg(static_cast<int>(std::numeric_limits<uint16_t>::min()))
                                                       .arg(static_cast<int>(std::numeric_limits<uint16_t>::max()))
                                                       .get() );
    REQ( std::string("e) Integer A=-32768, B=32767."), ==, NoPrintf("e) Integer A=$1, B=$2.")
                                                       .arg(static_cast<int>(std::numeric_limits<int16_t>::min()))
                                                       .arg(static_cast<int>(std::numeric_limits<int16_t>::max()))
                                                       .get() );
    REQ( std::string("f) Integer A=0, B=65535."), ==, NoPrintf("f) Integer A=$1, B=$2.")
                                                       .arg(static_cast<int>(std::numeric_limits<unsigned short>::min()))
                                                       .arg(static_cast<int>(std::numeric_limits<unsigned short>::max()))
                                                       .get() );
    REQ( std::string("g) Integer A=-32768, B=32767."), ==, NoPrintf("g) Integer A=$1, B=$2.")
                                                       .arg(static_cast<int>(std::numeric_limits<signed short>::min()))
                                                       .arg(static_cast<int>(std::numeric_limits<signed short>::max()))
                                                       .get() );
    // must fail, because signed int overflow in $2
    REQ( std::string("h) Integer A=0, B=4294967295."), !=, NoPrintf("h) Integer A=$1, B=$2.")
                                                       .arg(static_cast<int>(std::numeric_limits<unsigned int>::min()))
                                                       .arg(static_cast<int>(std::numeric_limits<unsigned int>::max()))
                                                       .get() );

    REQ( std::string("i) Integer A=-2147483648, B=2147483647."), ==, NoPrintf("i) Integer A=$1, B=$2.")
                                                       .arg(static_cast<int>(std::numeric_limits<signed int>::min()))
                                                       .arg(static_cast<int>(std::numeric_limits<signed int>::max()))
                                                       .get() );
    REQ( std::string("j) Integer A=-2147483648, B=2147483647."), ==, NoPrintf("j) Integer A=$1, B=$2.")
                                                       .arg(static_cast<int>(std::numeric_limits<int>::min()))
                                                       .arg(static_cast<int>(std::numeric_limits<int>::max()))
                                                       .get() );
  };
}


#define JOHANNES_1_1a "Im Anfang war das Wort, und das Wort war bei Gott, und das Wort war Gott."
#define JOHANNES_1_1b "Im Anfang war es bei Gott."
#define JOHANNES_1_1c "Alles ist durch das Wort geworden und ohne das Wort wurde nichts, was geworden ist."
#define JOHANNES_1_1d "In ihm war das Leben und das Leben war das Licht der Menschen."

#define First_Thing_A "das Wort"
#define First_Thing_B "der Urknall"
#define JWH           "Gott"
#define ATHEIST       "Singular"

#define BIBLE_OPENING_LINES  JOHANNES_1_1a "\r\n" JOHANNES_1_1b "\r" JOHANNES_1_1c "\n" JOHANNES_1_1d "\t."

#define BIBLE_OPENING_TMPL "Im Anfang war $1, und $1 war bei $2, und $1 war $2." \
 "\r\n" \
 "Im Anfang war es bei $2." \
 "\r" \
 "Alles ist durch $1 geworden und ohne $1 wurde nichts, was geworden ist." \
 "\n" \
 "In ihm war das Leben und das Leben war das Licht der Menschen." \
 "\t."


TEST(NoPrintf_SimpleString_Creations)
{
  SUB( SimpleString )
  {
    NoPrintf OneWord("Wort");
    REQ(  OneWord.get(), ==, std::string("Wort") );
    REQ(  OneWord.get(), !=, std::string("wort") );
    REQ(  OneWord.get(), !=, std::string("WORT") );
    REQ(  OneWord.get(), !=, std::string("Wort\n") );
  };

  SUB( Simple_Sentence )
  {
    NoPrintf OneSentence( BIBLE_OPENING_LINES );
    REQ(  OneSentence.get(), ==, std::string( BIBLE_OPENING_LINES ) );
    REQ(  OneSentence.get(), ==, std::string( JOHANNES_1_1a "\r\n" JOHANNES_1_1b "\r" JOHANNES_1_1c "\n" JOHANNES_1_1d "\t.") );
  };

  SUB( Replace_Dollars )
  {
    NoPrintf Dollars_1( "3$ 4ct, aber 7$$ 6ct." ); Dollars_1.arg(3ul).arg(4ul).arg(7ul).arg(6ul);
    REQ( Dollars_1.get(), ==, std::string( "3$ 4ct, aber 7$ 6ct." ) );

    NoPrintf Dollars_2( "$1$ $2ct, aber $3$$ $4ct." ); Dollars_2.arg(3ul).arg(4ul).arg(7ul).arg(6ul);
    REQ( Dollars_2.get(), ==, std::string( "3$ 4ct, aber 7$ 6ct." ) );
  };

  SUB( Creation_of_all )
  {
    NoPrintf Christian( BIBLE_OPENING_TMPL ); Christian.arg(First_Thing_A).arg(JWH);
    NoPrintf Heretics; Heretics.set( BIBLE_OPENING_TMPL ).arg(First_Thing_B).arg(ATHEIST);
    REQ(  Christian.get(), ==, std::string( BIBLE_OPENING_LINES ) );
    REQ(  Heretics.get(),  !=, std::string( BIBLE_OPENING_LINES ) );
  };
}


// To test DATA
DATA(GetFailedTests)
{
  unsigned int failedTestCount = 0;
  std::cout << "-----------------------------" << std::endl;
  std::cout << "Failed tests:" << std::endl;
  data->IterSons( [&failedTestCount] (const lightest::Data* item )
    {
      const lightest::DataSet* test = static_cast<const lightest::DataSet*>(item);
      if (test->GetFailed())
      {
        std::cout << " * " << test->GetName() << std::endl;
        failedTestCount++;
      }
    }
  ); // lambda-end
  std::cout << failedTestCount << " test" << (failedTestCount > 1 ? "s" : "")
            << " failed." << std::endl;
  std::cout << "-----------------------------" << std::endl;
  if (lightest::toOutput) {
    std::cout << "Now the overall report:" << std::endl;
  }
  return; // just for easy breakpoint setting while debugging failed tests.
}
