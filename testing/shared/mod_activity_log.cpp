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
		REQUIRE_THROWS_AS(t.header().hash, transaction_invalid_state);
	}
}

TEST_CASE("Test Record transaction", "[shared],[activity_log],[log_transaction]" ) {
	activity_log log;
	auto t = log.begin_transaction();
	auto hdr = t.generate_header();
	SECTION("Create record") {
		hdr.timestamp = get_timestamp();
		hdr.type = activity_type::trusted_verify;
		hdr.signature[21] = 0xe;
		
		auto act = t.generate_record(hdr);
		REQUIRE(act.header().hash != hash512{0});
		REQUIRE(act.header().type == activity_type::trusted_verify);
	}
	
	SECTION("Add Record") {
		REQUIRE(t.size() == 0);
		hdr.timestamp = get_timestamp();
		hdr.type = activity_type::trusted_verify;
		hdr.signature[21] = 0xf;
		
		auto act_a = t.generate_record(hdr);
		t.write_record(act_a);
		REQUIRE(t.size() == 1);		
	}
	
	SECTION("Add Multiple Records") {
		hdr.timestamp = get_timestamp();
		hdr.type = activity_type::trusted_verify;
		hdr.signature[21] = 0xf;
		
		auto act_a = t.generate_record(hdr);
		t.write_record(act_a);

		hdr.type = activity_type::root_verify;
		hdr.timestamp = get_timestamp();
		
		auto act_b = t.generate_record(hdr);
		t.write_record(act_b);
		REQUIRE(t.size() == 2);	
	}
}

TEST_CASE("Validate log hash", "[shared],[activity_log],[log_transaction]") {
	activity_log log;
	
	{
		auto t = log.begin_transaction();
		auto hdr = t.generate_header();
		
		hdr.timestamp = get_timestamp();
		hdr.type = activity_type::trusted_verify;
		hdr.signature[21] = 0xf;
		
		auto act_a = t.generate_record(hdr);
		t.write_record(act_a);
		
		REQUIRE(t.log_verification() == true);

		hdr.type = activity_type::root_verify;
		hdr.timestamp = get_timestamp();
		
		auto act_b = t.generate_record(hdr);
		t.write_record(act_b);
		REQUIRE(t.log_verification() == true);
	}
	
	REQUIRE(log.transacting() == false);
}