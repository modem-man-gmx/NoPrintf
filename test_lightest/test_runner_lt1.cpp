#include <string>
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

TEST(TestTimerMacros)
{
  int i = 0;
  REQ(TIMER(i++), >=, 0);  // Run once and measure the time
  REQ(AVG_TIMER(i++, 100), >=, 0);  // Run several times and return the average time
}

TEST(Default_NoPrintf_handling)
{
  NoPrintf Default;
  MUST(
  REQ(  Default.get(), ==, std::string() )
  );
  REQ(  Default.get(), ==, std::string("") );
  REQ( *Default.get().c_str(), ==, '\0' );
}

TEST(NoPrintf_Empty_Creations)
{
  SUB( EmptyString )
  {
    NoPrintf EmptyString("");
    REQ(  EmptyString.get(), ==, std::string() );
    REQ(  EmptyString.get(), ==, std::string("") );
    REQ( *EmptyString.get().c_str(), ==, '\0' );
    REQ(  EmptyString.get().c_str(), !=, static_cast<const char*>(nullptr) );
  };

  SUB( EmptyString_and_args )
  {
    NoPrintf EmptyString("");
    EmptyString.arg("invisible").arg(0L);
    REQ(  EmptyString.get(), ==, std::string() );
    REQ(  EmptyString.get(), ==, std::string("") );
    REQ( *EmptyString.get().c_str(), ==, '\0' );
    REQ(  EmptyString.get().c_str(), !=, static_cast<const char*>(nullptr) );
  };

  SUB( Anonymous_default )
  {
    REQ(  NoPrintf().get(), ==, std::string() );
    REQ(  NoPrintf().get(), ==, std::string("") );
    REQ( *NoPrintf().get().c_str(), ==, '\0' );
    REQ(  NoPrintf().get().c_str(), !=, static_cast<const char*>(nullptr) );
  };

  SUB( Anonymous_empty )
  {
    const char *nix="\0";
    std::string leer("");
    std::string deflt;
    REQ(  NoPrintf( ""  ).get(), ==, std::string() );
    REQ(  NoPrintf( nix ).get(), ==, std::string("") );
    REQ( *NoPrintf( leer).get().c_str(), ==, '\0' );
    REQ(  NoPrintf(deflt).get().c_str(), !=, static_cast<const char*>(nullptr) );
  };

  SUB( Anonymous_and_args )
  {
    REQ(  NoPrintf().arg("invisible").arg(0L).get(), ==, std::string() );
    REQ(  NoPrintf().arg("invisible").arg(0L).get(), ==, std::string("") );
    REQ( *NoPrintf().arg("invisible").arg(0L).get().c_str(), ==, '\0' );
    REQ(  NoPrintf().arg("invisible").arg(0L).get().c_str(), !=, static_cast<const char*>(nullptr) );
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
  std::cout << "-----------------------------" << std::endl;
  std::cout << "Failed tests:" << std::endl;
  data->IterSons( [] (const lightest::Data* item )
    {
      const lightest::DataSet* test = static_cast<const lightest::DataSet*>(item);
      if (test->GetFailed())
      {
        std::cout << " * " << test->GetName() << std::endl;
      }
    }
  ); // lambda-end
  std::cout << "-----------------------------" << std::endl;
}
