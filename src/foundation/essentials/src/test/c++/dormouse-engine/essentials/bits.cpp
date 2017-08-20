#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/essentials/bits.hpp"

using namespace dormouse_engine::essentials;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(DormouseEngineEssentialsBitsTestSuite);

BOOST_AUTO_TEST_CASE(NumberOfBitsOnReturnsCorrectValue) {
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b0), 0);
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b1), 1);
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b10), 1);
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b11), 2);
	BOOST_CHECK_EQUAL(numberOfBitsOn(0b10110011100011110000), 10);
}

BOOST_AUTO_TEST_CASE(ReturnsValidEndianness) {
	const std::uint32_t ONE_TWO_THREE_FOUR = 0x01020304;
	const bool BIG_ENDIAN = (*reinterpret_cast<const std::uint8_t*>(&ONE_TWO_THREE_FOUR) == 1);

	BOOST_CHECK_EQUAL(isBigEndian(), BIG_ENDIAN);
}

BOOST_AUTO_TEST_CASE(ChangesEndianness) {
	const std::uint32_t ONE_TWO_THREE_FOUR = 0x01020304;
	const std::uint32_t FOUR_THREE_TWO_ONE = 0x04030201;

	BOOST_CHECK_EQUAL(changeEndianness(ONE_TWO_THREE_FOUR), FOUR_THREE_TWO_ONE);
	BOOST_CHECK_EQUAL(changeEndianness(changeEndianness(ONE_TWO_THREE_FOUR)), ONE_TWO_THREE_FOUR);
}

BOOST_AUTO_TEST_CASE(ChangingEndiannessOfCharDoesNothing) {
	const char ONE = 1;

	BOOST_CHECK_EQUAL(changeEndianness(ONE), ONE);
}

BOOST_AUTO_TEST_SUITE_END(/* DormouseEngineEssentialsBitsTestSuite */);

} // anonymous namespace
