#include "catch.hpp"
#include "activity_log.hpp"

TEST_CASE( "Test activity_log ctors", "[shared],[activity_log]" ) {
	SECTION("activity_log Default") {
		activity_log log;
	}	
}

TEST_CASE("Test transaction objects") {
	activity_log log;
	
	SECTION("Basic ctor and dtor") {
		REQUIRE(log.transacting() == false);
		
		{
			auto t = log.begin_transaction();
			REQUIRE(log.transacting() == true);
		}
		
		REQUIRE(log.transacting() == false);		
	}
	
	SECTION("Test invalid_state exception") {
		auto t = log.begin_transaction();
		auto u = std::move(t);
		REQUIRE_THROWS_AS(t.read_hash(), transaction_invalid_state);
	}
}