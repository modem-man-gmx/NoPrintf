#ifndef NO_PRINTF_CUSTOMZING_H
#define NO_PRINTF_CUSTOMZING_H
// File to adopt settings in no_printf library
// just change this file and recompile no_printf
// ---------------------------------------------
// The places where this switches are used are
// typically looking like this:
//
// #if defined( NOPF_USE_FLOAT ) && (NOPF_USE_FLOAT>0)
//    float get_float_value();
// #endif //defined( NOPF_USE_FLOAT )
//
// to reflect the idea of 0/1 switching.
// ---------------------------------------------

#define NOPF_USE_FLOAT 0
#define NOPF_USE_DOUBLE 0
#define NOPF_USE_INT64 1
#define NOPF_USE_LONGLONG 1

#define NOPF_NUMERICAL_LEFTALIGN_FILLCHAR ' ' // do not change this to numericals, would get postpending '0000' so number could get wrong
#define NOPF_NUMERICAL_RIGHTALIGN_FILLCHAR ' '  // printf would use a '0', but printf also supports other outdated 0001234+ 000789- formatting ...

#define NOPF_USE_ARGCONFIG 1  // only for Ligtest Unit testing
#define NOPF_USE_DATA_ANALYSIS 1  // only for Ligtest Unit testing


// The next type has to be defined and has to be
// the biggest available type of this list:
// - unsigned short
// - unsigned int
// - unsigned long
// - unsigned long long
// if you choose a smaller one, you will get errors
// using .arg( too_big_type )
#if defined( NOPF_USE_INT64 ) && (NOPF_USE_INT64>0) && \
    defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG>0) && \
    defined( ULLONG_MAX ) && defined( __UINT64_MAX__ ) && (ULLONG_MAX >= __UINT64_MAX__)
  // both defined: decide the bigger one
  typedef unsigned long long BiggestNumerical_t;
#elif defined( NOPF_USE_LONGLONG ) && (NOPF_USE_LONGLONG>0)
  typedef unsigned long long BiggestNumerical_t;
#elif defined( NOPF_USE_INT64 ) && (NOPF_USE_INT64>0)
  typedef uint64_t BiggestNumerical_t;
#else
  typedef unsigned long BiggestNumerical_t;
#endif

// =============================================
// Consistency-Adjusting. May get moved elsewhere
// Better keep this untouched.
// =============================================

#if defined( NOPF_USE_DOUBLE ) && (NOPF_USE_DOUBLE>0)
#  undef NOPF_USE_FLOAT
#  define NOPF_USE_FLOAT 1
#endif //defined( NOPF_USE_DOUBLE )

#endif //ndef NO_PRINTF_CUSTOMZING_H
