#include "catch.hpp"
#include "infrastructure/metaspace_gsn.hpp"

TEST_CASE("Test metaspace_gsn ctors","[shared],[metaspace_gsn") {
	REQUIRE_NOTHROW(metaspace_gsn msgsn);
}

TEST_CASE("Test metaspace_gsn.has_outcode matching","[shared],[metaspace_gsn") {
	metaspace_gsn g;
	std::string outcode;
	
	REQUIRE(g.has_outcode("ZY44", outcode));
	REQUIRE(outcode == "zy44");
	
	REQUIRE(g.has_outcode("ZY44 U88", outcode));
	REQUIRE(outcode == "zy44");

	REQUIRE(g.has_outcode("ZY44U88", outcode));
	REQUIRE(outcode == "zy44");
	
	REQUIRE(g.has_outcode("zy44", outcode));
	REQUIRE(outcode == "zy44");
	
	REQUIRE(g.has_outcode("Zy44", outcode));
	REQUIRE(outcode == "zy44");
	
	REQUIRE(g.has_outcode("zY44", outcode));
	REQUIRE(outcode == "zy44");
	
	REQUIRE(g.has_outcode("zY44u88", outcode));
	REQUIRE(outcode == "zy44");
	
	REQUIRE_FALSE(g.has_outcode("ZYT42", outcode));
	REQUIRE(outcode == "");
	
	REQUIRE_FALSE(g.has_outcode("hello ZY44 U88", outcode));
	REQUIRE(outcode == "");
}

TEST_CASE("Test GSN Resolution","[shared],[metaspace_gsn") {
	metaspace_gsn g;

	SECTION("resolution exception") {
		REQUIRE_THROWS_AS(g.resolve_gsn(""),metaspace_query_fail);
	}

	SECTION("Outcode resolution success") {
		auto s = g.resolve_gsn("tn37");
		REQUIRE(s.length() > 0);
		REQUIRE(s == "esusx");
	}
	
	SECTION("Outcode resolution fail") {
		auto s = g.resolve_gsn("zy44");
		REQUIRE(s.length() == 0);
	}

	
	SECTION("County resolution success") {
		auto s = g.resolve_gsn("East Sussex");
		REQUIRE(s.length() > 0);
		REQUIRE(s == "esusx");
	}
	
	SECTION("county resolution fail") {
		auto s = g.resolve_gsn("Dorset");
		REQUIRE(s.length() == 0);
	}
}
