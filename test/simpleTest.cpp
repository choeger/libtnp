#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>

#include "unaryAlgebraic.hpp"
#include "analyticFunctions.hpp"
#include "numberGenerator.hpp"

using namespace boost::unit_test;

using namespace tnp::test;

test_suite*
init_unit_test_suite( int argc, char* argv[] ) {
 
  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testEquality, testNumbers().begin(), testNumbers().end() ) );

  
  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testSelfSubtraction, testNumbers().begin(), testNumbers().end() ) );

  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testAdditionWithZero, testNumbers().begin(), testNumbers().end() ) );

  
  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testMultiplicationWithConstantOne, testNumbers().begin(), testNumbers().end() ) );

  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testMultiplicationWithConstantTwo, testNumbers().begin(), testNumbers().end() ) );

  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testMultiplicationWithOne, testNumbers().begin(), testNumbers().end() ) );

  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testMultiplicationWithTwo, testNumbers().begin(), testNumbers().end() ) );
  
  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testManyMultiplicationsWithOne, testDimensions.begin(), testDimensions.end() ) );

  const std::vector<UnaryAnalyticFunctionTest>& analyticTestCases = UnaryAnalyticFunctionTest::testCases();

  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testUnaryAnalyticFunction, analyticTestCases.begin(), analyticTestCases.end() ) );

  /*
  framework::master_test_suite().
    add( BOOST_PARAM_TEST_CASE( &testSelfDivision, testNumbers().begin(), testNumbers().end() ) );
  */

  return 0;
}

