#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <tinja.hpp>

#include <bustache/format.hpp>
#include <bustache/render/string.hpp>
#include <bustache/model.hpp>

#include "bustache_nlohmann_model.hpp"
#include "bustache_model.hpp"
#include "inja.hpp"
#include "micro_mustache.hpp"
#include "kainjow_mustache.hpp"
#include "util.hpp"

constexpr size_t loopSize = 60;

std::string concat(const tinja::Template::Tokens& tinjaTokens) {
    std::string str;
    size_t toReserve = 0;
    for (const auto& t : tinjaTokens) {
        toReserve += t.get().size();
    }
    str.reserve(toReserve);
    for (const auto& t : tinjaTokens) {
        str += t;
    }
    return str;
}

TEST_CASE("Performance comparison", "[benchmark]") {
    const auto basicString = readHtmlFile("circuco_basic.html");
    const auto injaString = readHtmlFile("circuco_inja.html");
    const auto tinjaString = readHtmlFile("circuco_tinja.html");
    const auto mustacheString = readHtmlFile("circuco_mustache.html");

    moustache_variable_t moustacheData[] = {
        {"f", "06:00"},
        {"t", "16:00"},
        {"d", "3 min"},
        {"i", "30 min"},
        {"r", "0.3 °C/s"},
        {"v", "52.3 °C"},
        {"p", "56"},
        {"dv", "-0.1 °C/s"},
        {"dp", "33"},
        {"rc", "bg-warning"},
        {"ac", "bg-warning progress-bar-striped progress-bar-animated"},
        {"fh", "123434"},
        {"fb", "23444"},
        {"hf", "45"},
        //{"sh"] = std::vector<std::string>{"1", "2", "3", "4"};
        {"maxH", "99"}
        //{"ah"] = std::vector<std::string>{"2", "3", "4", "5", "6"};
    };

    inja::json injaData; {
        injaData["f"] = "06:00";
        injaData["t"] = "16:00";
        injaData["d"] = "3 min";
        injaData["i"] = "30 min";
        injaData["r"] = "0.3 °C/s";
        injaData["v"] = "52.3 °C";
        injaData["p"] = "56";
        injaData["dv"] = "-0.1 °C/s";
        injaData["dp"] = "33";
        injaData["rc"] = "bg-warning";
        injaData["ac"] = "bg-warning progress-bar-striped progress-bar-animated";
        injaData["fh"] = "123434";
        injaData["fb"] = "23444";
        injaData["hf"] = "45";
        //_data["sh"] = std::vector<std::string>{"1", "2", "3", "4"};
        injaData["maxH"] = "99";
        //_data["ah"] = std::vector<std::string>{"2", "3", "4", "5", "6"};
    }

    std::unordered_map<std::string, std::string> bustacheData; {
        bustacheData["f"] = "06:00";
        bustacheData["t"] = "16:00";
        bustacheData["d"] = "3 min";
        bustacheData["i"] = "30 min";
        bustacheData["r"] = "0.3 °C/s";
        bustacheData["v"] = "52.3 °C";
        bustacheData["p"] = "56";
        bustacheData["dv"] = "-0.1 °C/s";
        bustacheData["dp"] = "33";
        bustacheData["rc"] = "bg-warning";
        bustacheData["ac"] = "bg-warning progress-bar-striped progress-bar-animated";
        bustacheData["fh"] = "123434";
        bustacheData["fb"] = "23444";
        bustacheData["hf"] = "45";
        //_data["sh"] = std::vector<std::string>{"1", "2", "3", "4"};
        bustacheData["maxH"] = "99";
        //_data["ah"] = std::vector<std::string>{"2", "3", "4", "5", "6"};
    }

    bustache::test::object bustacheDataLoop {
        {"f", "06:00"},
        {"t", "16:00"},
        {"d", "3 min"},
        {"i", "30 min"},
        {"r", "0.3 °C/s"},
        {"v", "52.3 °C"},
        {"p", "56"},
        {"dv", "-0.1 °C/s"},
        {"dp", "33"},
        {"rc", "bg-warning"},
        {"ac", "bg-warning progress-bar-striped progress-bar-animated"},
        {"fh", "123434"},
        {"fb", "23444"},
        {"hf", "45"},
        //{"sh"] = std::vector<std::string>{"1", "2", "3", "4"};
        {"maxH", "99"}
        //{"ah"] = std::vector<std::string>{"2", "3", "4", "5", "6"};
    };

    tinja::DataMap tinjaData; {
        tinjaData["f"] = "06:00";
        tinjaData["t"] = "16:00";
        tinjaData["d"] = "3 min";
        tinjaData["i"] = "30 min";
        tinjaData["r"] = "0.3 °C/s";
        tinjaData["v"] = "52.3 °C";
        tinjaData["p"] = "56";
        tinjaData["dv"] = "-0.1 °C/s";
        tinjaData["dp"] = "33";
        tinjaData["rc"] = "bg-warning";
        tinjaData["ac"] = "bg-warning progress-bar-striped progress-bar-animated";
        tinjaData["fh"] = "123434";
        tinjaData["fb"] = "23444";
        tinjaData["hf"] = "45";
        //_data["sh"] = std::vector<std::string>{"1", "2", "3", "4"};
        tinjaData["maxH"] = "99";
        //_data["ah"] = std::vector<std::string>{"2", "3", "4", "5", "6"};
    }

    kainjow::mustache::object kainjowData {
        {"f", "06:00"},
        {"t", "16:00"},
        {"d", "3 min"},
        {"i", "30 min"},
        {"r", "0.3 °C/s"},
        {"v", "52.3 °C"},
        {"p", "56"},
        {"dv", "-0.1 °C/s"},
        {"dp", "33"},
        {"rc", "bg-warning"},
        {"ac", "bg-warning progress-bar-striped progress-bar-animated"},
        {"fh", "123434"},
        {"fb", "23444"},
        {"hf", "45"},
        //{"sh"] = std::vector<std::string>{"1", "2", "3", "4"};
        {"maxH", "99"}
        //{"ah"] = std::vector<std::string>{"2", "3", "4", "5", "6"};
    };

    SECTION("cold") {
        SECTION("sanity check") {
            REQUIRE(injaData.size() == tinjaData.size());

            const auto microDoc = moustache_render(basicString, moustacheData);
            kainjow::mustache::mustache kainjowTempl(basicString);
            const auto kainjowDoc = kainjowTempl.render(kainjowData);
            const auto injaDoc = inja::render(basicString, injaData);
            bustache::format bustacheTempl(basicString);
            const auto bustacheDoc = bustache::to_string(bustacheTempl(bustacheData));
            REQUIRE(microDoc == kainjowDoc);
            REQUIRE(kainjowDoc == injaDoc);
            REQUIRE(injaDoc == bustacheDoc);

            tinja::Template tinjaTempl;
            tinja::Template::Tokens tinjaTokens;
            const auto nodeCount = tinjaTempl.parse(basicString);
            std::cout << "tinja> template has: " << nodeCount << " nodes";
            tinjaTempl.renderTo(tinjaData, tinjaTokens);
            const auto tinjaDoc = concat(tinjaTokens);
            REQUIRE(tinjaDoc == bustacheDoc);
        }

        BENCHMARK_ADVANCED("micro_mustache")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] { return moustache_render(basicString, moustacheData); });
        };

        BENCHMARK_ADVANCED("kainjow_mustache")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                kainjow::mustache::mustache templ(basicString);
                return templ.render(kainjowData);
            });
        };

        BENCHMARK_ADVANCED("inja")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] { return inja::render(basicString, injaData); });
        };

        BENCHMARK_ADVANCED("bustache")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                bustache::format bustacheTempl(basicString);
                return bustache::to_string(bustacheTempl(bustacheData));
            });
        };

        BENCHMARK_ADVANCED("tinja")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                tinja::Template templ(basicString);
                tinja::Template::Tokens tokens;
                return templ.renderTo(tinjaData, tokens);
            });
        };

        BENCHMARK_ADVANCED("tinja --32_nodes_reserved")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                tinja::Template templ(basicString, 32);
                tinja::Template::Tokens tokens;
                return templ.renderTo(tinjaData, tokens);
            });
        };

        BENCHMARK_ADVANCED("tinja --concat")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                tinja::Template templ(basicString);
                tinja::Template::Tokens tokens;
                templ.renderTo(tinjaData, tokens);
                return concat(tokens);
            });
        };

        BENCHMARK_ADVANCED("tinja --32_nodes_reserved --concat")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                tinja::Template templ(basicString, 32);
                tinja::Template::Tokens tokens;
                templ.renderTo(tinjaData, tokens);
                return concat(tokens);
            });
        };
    }

    SECTION("preparsed") {
        tinja::Template::Tokens tinjaTokens;

        BENCHMARK_ADVANCED("micro_mustache")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] { return moustache_render(basicString, moustacheData); });
        };

        BENCHMARK_ADVANCED("kainjow_mustache")(Catch::Benchmark::Chronometer meter) {
            kainjow::mustache::mustache doc(basicString);
            meter.measure([&] { return doc.render(kainjowData); });
        };

        BENCHMARK_ADVANCED("inja")(Catch::Benchmark::Chronometer meter) {
            inja::Environment env;
            inja::Template templ = env.parse(basicString);
            meter.measure([&] { return env.render(templ, injaData); });
        };

        BENCHMARK_ADVANCED("bustache")(Catch::Benchmark::Chronometer meter) {
            bustache::format bustacheTempl(basicString);
            meter.measure([&] { return bustache::to_string(bustacheTempl(bustacheData)); });
        };

        BENCHMARK_ADVANCED("tinja")(Catch::Benchmark::Chronometer meter) {
            tinja::Template templ(basicString);
            meter.measure([&] { return templ.renderTo(tinjaData, tinjaTokens); });
        };

        BENCHMARK_ADVANCED("tinja --concat")(Catch::Benchmark::Chronometer meter) {
            tinja::Template templ(basicString);
            meter.measure([&] {
                templ.renderTo(tinjaData, tinjaTokens);
                return concat(tinjaTokens);
            });
        };
    }

    SECTION("arrays") {
        std::vector<std::string> ah;
        std::vector<std::string> sh;
        for (int i = 0; i < loopSize; ++i) {
            ah.push_back(std::to_string(i+2));
            sh.push_back(std::to_string(i+1));
        }

        kainjow::mustache::list kainjowHs;
        for (auto i = 0; i < loopSize; ++i) {
            kainjow::mustache::object o;
            o["s"] = sh.at(i);
            o["a"] = ah.at(i);
            kainjowHs.push_back(o);
        }
        kainjowData["hs"] = kainjowHs;

        // TODO: this does not work
        bustache::test::array bustacheHs;
        for (auto i = 0; i < loopSize; ++i) {
            bustache::test::object o {
                { "s", sh.at(i) },
                { "a", ah.at(i) }
            };
            bustacheHs.push_back(o);
        }
        bustacheDataLoop.push_back( {"hs", bustacheHs } );

        auto injaHs = inja::json::array();
        for (auto i = 0; i < loopSize; ++i) {
            inja::json o;
            o["s"] = sh.at(i);
            o["a"] = ah.at(i);
            injaHs.push_back(o);
        }
        injaData["hs"] = injaHs;

        tinjaData["sh"] = sh;
        tinjaData["ah"] = ah;

        kainjow::mustache::mustache kainjowTempl(mustacheString);
        inja::Environment injaEnv;
        inja::Template injaTempl = injaEnv.parse(injaString);
        bustache::format bustacheTempl(mustacheString);
        tinja::Template tinjaTempl(tinjaString);
        tinja::Template::Tokens tinjaTokens;

        SECTION("sanity check") {
            const auto kainjowDoc = kainjowTempl.render(kainjowData);
            const auto injaDoc = inja::render(injaString, injaData);
            const auto bustacheDocJson = bustache::to_string(bustacheTempl(injaData));
            const auto bustacheDocNative = bustache::to_string(bustacheTempl(bustacheData));
            const auto tinjaNodeCount = tinjaTempl.parse(tinjaString);
            tinja::Template::Tokens tinjaTokens;
            tinjaTempl.renderTo(tinjaData, tinjaTokens);
            std::cout << "tinja> " << tinjaNodeCount << " nodes parse to " << tinjaTokens.size() << " tokens" << std::endl;
            REQUIRE(kainjowDoc == injaDoc);
            REQUIRE(injaDoc == bustacheDocJson);
            //REQUIRE(bustacheDocJson == bustacheDocNative);
            REQUIRE(bustacheDocJson == concat(tinjaTokens));
        }

        BENCHMARK_ADVANCED("kainjow_mustache")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] { return kainjowTempl.render(kainjowData); });
        };

        BENCHMARK_ADVANCED("inja")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] { return injaEnv.render(injaTempl, injaData); });
        };

        BENCHMARK_ADVANCED("bustache --concat --nlohmann")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] { return bustache::to_string(bustacheTempl(injaData)); });
        };

        //BENCHMARK_ADVANCED("bustache --native")(Catch::Benchmark::Chronometer meter) {
        //    meter.measure([&] { return bustache::to_string(bustacheTempl(bustacheDataLoop)); });
        //};

        BENCHMARK_ADVANCED("tinja")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] { return tinjaTempl.renderTo(tinjaData, tinjaTokens); });
        };

        BENCHMARK_ADVANCED("tinja --concat")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                tinjaTempl.renderTo(tinjaData, tinjaTokens);
                return concat(tinjaTokens);
            });
        };
    }
}
