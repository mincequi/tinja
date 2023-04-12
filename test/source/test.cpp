#include <catch2/catch_test_macros.hpp>

#include <tinja.hpp>

TEST_CASE("Texts", "[tinja]") {
    std::string str = "";
    tinja::Template templ;
    tinja::Template::Tokens tokens;
    tinja::DataMap data;
    REQUIRE(templ.parse(str) == 0);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 0);

    str = "T";
    REQUIRE(templ.parse(str) == 1);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens.front().get() == "T");

    str = "{}}";
    REQUIRE(templ.parse(str) == 1);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens.front().get() == "{}}");

    str = "{{}";
    REQUIRE(templ.parse(str) == 1);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens.front().get() == "{{}");
}

TEST_CASE("Variables", "[tinja]") {
    std::string str = "{{}}";
    tinja::Template templ;
    tinja::Template::Tokens tokens;
    tinja::DataMap data;
    REQUIRE(templ.parse(str) == 0);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 0);

    str = "{{V}}";
    REQUIRE(templ.parse(str) == 1);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 0);

    std::string v1("V1");
    data["V"] = v1;
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens.front().get() == "V1");

    str = "{{V}} {{V2}}";
    REQUIRE(templ.parse(str) == 3);
    v1 = "V2";
    data["V2"] = tinja::StringRef(v1);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.front().get() == "V1");
    REQUIRE(tokens.at(1).get() == " ");
    REQUIRE(tokens.at(2).get() == "V2");
    v1 = "V3";
    REQUIRE(tokens.at(2).get() == "V3");
}

TEST_CASE("Arrays", "[tinja]") {
    std::string str = "{[]}";
    tinja::Template templ;
    tinja::Template::Tokens tokens;
    tinja::DataMap data;
    REQUIRE(templ.parse(str) == 0);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 0);

    str = "{[D]}";
    REQUIRE(templ.parse(str) == 1);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens.front().get() == "D");

    str = "{[{{V}}]}";
    REQUIRE(templ.parse(str) == 1);
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 0);

    data["V"] = tinja::Strings { "Va", "Vb" };
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens.front().get() == "Va");
    REQUIRE(tokens.back().get() == "Vb");

    str = "{[{{V}}{{V2}}]}";
    REQUIRE(templ.parse(str) == 1);
    tinja::Strings v1 { "Va", "Vb", "Vc", "Vd" };
    data["V"] = v1;
    data["V2"] = tinja::StringRefs { v1.begin(), v1.begin()+3 };
    templ.renderTo(data, tokens);
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens.at(0).get() == "Va");
    REQUIRE(tokens.at(1).get() == "Va");
    REQUIRE(tokens.at(2).get() == "Vb");
    REQUIRE(tokens.at(3).get() == "Vb");
    REQUIRE(tokens.at(4).get() == "Vc");
    REQUIRE(tokens.at(5).get() == "Vc");
}
