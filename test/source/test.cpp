#include <catch2/catch_test_macros.hpp>

#include <tinja.hpp>

TEST_CASE("Texts", "[tinja]") {
    std::string str = "";
    tinja::Template doc;
    tinja::DataMap data;
    REQUIRE(doc.parse(str) == 0);
    auto nodes = doc.render(data);
    REQUIRE(nodes.size() == 0);

    str = "T";
    REQUIRE(doc.parse(str) == 1);
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 1);
    REQUIRE(nodes.front().get() == "T");

    str = "{}}";
    REQUIRE(doc.parse(str) == 1);
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 1);
    REQUIRE(nodes.front().get() == "{}}");

    str = "{{}";
    REQUIRE(doc.parse(str) == 1);
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 1);
    REQUIRE(nodes.front().get() == "{{}");
}

TEST_CASE("Variables", "[tinja]") {
    std::string str = "{{}}";
    tinja::Template doc;
    tinja::DataMap data;
    REQUIRE(doc.parse(str) == 0);
    auto nodes = doc.render(data);
    REQUIRE(nodes.size() == 0);

    str = "{{V}}";
    REQUIRE(doc.parse(str) == 1);
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 0);

    std::string v1("V1");
    data["V"] = v1;
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 1);
    REQUIRE(nodes.front().get() == "V1");

    str = "{{V}} {{V2}}";
    REQUIRE(doc.parse(str) == 3);
    v1 = "V2";
    data["V2"] = tinja::StringRef(v1);
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 3);
    REQUIRE(nodes.front().get() == "V1");
    REQUIRE(nodes.at(1).get() == " ");
    REQUIRE(nodes.at(2).get() == "V2");
    v1 = "V3";
    REQUIRE(nodes.at(2).get() == "V3");
}

TEST_CASE("Arrays", "[tinja]") {
    std::string str = "{[]}";
    tinja::Template doc;
    tinja::DataMap data;
    REQUIRE(doc.parse(str) == 0);
    auto nodes = doc.render(data);
    REQUIRE(nodes.size() == 0);

    str = "{[D]}";
    REQUIRE(doc.parse(str) == 1);
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 1);
    REQUIRE(nodes.front().get() == "D");

    str = "{[{{V}}]}";
    REQUIRE(doc.parse(str) == 1);
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 0);

    data["V"] = tinja::Strings { "Va", "Vb" };
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 2);
    REQUIRE(nodes.front().get() == "Va");
    REQUIRE(nodes.back().get() == "Vb");

    str = "{[{{V}}{{V2}}]}";
    REQUIRE(doc.parse(str) == 1);
    tinja::Strings v1 { "Va", "Vb", "Vc", "Vd" };
    data["V"] = v1;
    data["V2"] = tinja::StringRefs { v1.begin(), v1.begin()+3 };
    nodes = doc.render(data);
    REQUIRE(nodes.size() == 6);
    REQUIRE(nodes.at(0).get() == "Va");
    REQUIRE(nodes.at(1).get() == "Va");
    REQUIRE(nodes.at(2).get() == "Vb");
    REQUIRE(nodes.at(3).get() == "Vb");
    REQUIRE(nodes.at(4).get() == "Vc");
    REQUIRE(nodes.at(5).get() == "Vc");


}
