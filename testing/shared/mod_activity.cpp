#include "catch.hpp"
#include "activity.hpp"
#include "mocks/activity_m.hpp"

TEST_CASE( "Test ctors", "[shared],[activity]" ) {
	char data[11] = "HelloWorld";
	auto control = hash_sha512(data, 10);

	SECTION("activity_header Default") {
		activity_header hdr;
		
		REQUIRE(hdr.hash[6] == 0);
		REQUIRE(hdr.timestamp == 0);
		REQUIRE(hdr.signature[253] == 0);
		REQUIRE(hdr.type == activity_type::undefined);
	}

	SECTION("activity Default")  {
		activity act;
		REQUIRE(act.size() == sizeof(activity_header));
		REQUIRE(act.size_partial() == sizeof(activity_header) - sizeof(activity_header::hash));
		REQUIRE(act.content().length() == 0);
		REQUIRE(act.content() == "");
		
		
		auto& hdr = act.header();
		REQUIRE(hdr.hash == hash512{0});
		REQUIRE(hdr.timestamp == 0);
		REQUIRE(hdr.signature[253] == 0);
		REQUIRE(hdr.type == activity_type::undefined);
	}

	SECTION("activity Supplied Header")  {

		activity_header_m hdr;
		activity act(hdr);
		
		auto h = act.header();

		REQUIRE(h.hash != control);
		REQUIRE(h.timestamp != 0);
		REQUIRE(h.signature[4] == 21);
		REQUIRE(h.type == activity_type::root_verify);
	}
	
	SECTION("activity supplied header and content") {
		activity_header_m hdr;
		activity act(hdr, "foobar");
		
		auto h = act.header();

		REQUIRE(h.hash != control);
		REQUIRE(h.timestamp != 0);
		REQUIRE(h.signature[4] == 21);
		REQUIRE(h.type == activity_type::root_verify);
		REQUIRE(act.content() == "foobar");		
	}
	
	SECTION("activity supplied header and content, with link") {
		activity_header_m hdr;
		activity previous(hdr, "foobar");
		activity act(hdr, "foobar", previous.header().hash);
		auto h = act.header();
		REQUIRE(h.hash != previous.header().hash);
	}
}

TEST_CASE( "Test de/serialise", "[shared],[activity]" ) {
	activity_m act;
	char data[11] = "HelloWorld";
	auto control = hash_sha512(data, 10);

	SECTION("Check mock header") {
		const auto& hdr = act.header();
		REQUIRE(hdr.hash != control);
		REQUIRE(hdr.timestamp != 0);
		REQUIRE(hdr.signature[4] == 21);
		REQUIRE(hdr.type == activity_type::root_verify);
	}
	
	SECTION("Raw serialised data") {
		auto serial = act.serialise();
		activity_header* hdr = reinterpret_cast<activity_header*>(serial);

		REQUIRE(hdr->hash != control);
		REQUIRE(hdr->timestamp != 0);
		REQUIRE(hdr->signature[4] == 21);
		REQUIRE(hdr->type == activity_type::root_verify);
		REQUIRE(hdr->msg_size == act.content().length());
	}
	
	SECTION("De-serialise") {
		activity checker;
		
		auto serial = act.serialise();		
		checker.deserialise(serial);
		const auto& hdr = checker.header();
		REQUIRE(hdr.hash == act.header().hash);
		REQUIRE(hdr.timestamp != 0);
		REQUIRE(hdr.signature[4] == 21);
		REQUIRE(hdr.type == activity_type::root_verify);
		REQUIRE(hdr.msg_size == act.content().length());
		REQUIRE(checker.content() == act.content());
	}
}