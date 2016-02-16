#include "catch.hpp"
#include "activity_log.hpp"
#include "log_segment.hpp"
#include "mocks/activity_m.hpp"
#include "mocks/activity_log_m.hpp"

TEST_CASE( "Test activity_log ctors", "[shared],[activity_log]" ) {
	SECTION("activity_log Default") {
		activity_log log;
	}	
}

TEST_CASE("Test log_segment ctor", "[shared],[activity_log],[log_segment]") {
    log_utility::container_type log;

	char data[11] = "HelloWorld";
	auto hash = std::move(hash_sha512(data, 10));

    for(auto i = 0u; i < 5; i++)
        log.push_back(activity_m());

	REQUIRE(log.size() == 5);

	log_segment segment(hash, std::move(log));
	REQUIRE(segment.size() == 5);
	REQUIRE(segment.header().hash == hash);
	REQUIRE(segment.header().num_records == 5);
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

TEST_CASE("Test log segments", "[shared],[activity_log],[log_transaction],[log_segment]") {
	activity_log_m l(10);
	
	
	auto t = l.begin_transaction();
	auto s = 4;
	hash512 split;
	for(const auto& r : t) {
		if(s == 0) {
			split = r.header().hash;
			break;
		}
		s--;
	}

	auto segment = t.segment_from(split);

	REQUIRE(segment.size() == 10-4-1);
	REQUIRE(segment.header().num_records == 10-4-1);
	REQUIRE(segment.header().hash == t.header().hash);
	auto inc = false;
	auto it = segment.cbegin();
	
	for(const auto& r : t) {
		if(inc) {
			REQUIRE(r.header().hash == (*it).header().hash);
			it++;
					
		}
		if(r.header().hash == split) inc = true;
	}
}

TEST_CASE("Test log_segment de/serialise", "[shared],[activity_log],[log_transaction],[log_segment]") {
	activity_log_m l(10);
	
	
	auto t = l.begin_transaction();
	auto s = 4;
	hash512 split;
	for(const auto& r : t) {
		if(s == 0) {
			split = r.header().hash;
			break;
		}
		s--;
	}

	auto segment = t.segment_from(split);
	auto serial = segment.serialise();
	
	log_segment deseg(serial);
	REQUIRE(deseg.size() == segment.size());
	auto it = segment.cbegin();
	for(const auto& r : deseg) {
		REQUIRE(r.header().hash == (*it).header().hash);
		REQUIRE(r.header().msg_size == (*it).header().msg_size);
		REQUIRE(r.header().timestamp == (*it).header().timestamp);
		REQUIRE(r.content() == (*it).content());
		it++;
	}
}

TEST_CASE("Test inserting segment", "[shared],[activity_log],[log_transaction],[log_segment]") {
	activity_log_m c_log(10);
	activity_log_m p_log(c_log, 4);
	auto tp = p_log.begin_transaction();
	auto tc = c_log.begin_transaction();
	
	SECTION("Sanity") {
		REQUIRE(tp.size() == 4);
		REQUIRE(tp.header().num_records == 4);
		REQUIRE(tp.header().hash != tc.header().hash);
	}
	
	SECTION("Actual insertion and verification") {
		auto segment = tc.segment_from(tp.cback().header().hash);
		tp.write_segment(segment);
		REQUIRE(tp.header().hash == tc.header().hash);
		REQUIRE(tp.header().num_records == tc.header().num_records);
	}
}