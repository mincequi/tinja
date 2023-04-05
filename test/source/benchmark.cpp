#include <fstream>
#include <regex>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <tinja.hpp>

#include "inja.hpp"
#include "micro_mustache.hpp"
#include "kainjow_mustache.hpp"

constexpr size_t loopSize = 120;

void minifyHtml(std::string& str) {
    //std::cout << "HTML template minified from " << str.size();
    const std::regex rx2("\\s+<");
    const std::regex rx3(">\\s+");
    //_htmlTemplate = std::regex_replace(_htmlTemplate, rx1, "");
    str = std::regex_replace(str, rx2, "<");
    str = std::regex_replace(str, rx3, ">");
    //std::cout << " to " << str.size() << " bytes" << std::endl;
    str.shrink_to_fit();
}

std::string readHtmlFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    minifyHtml(str);
    return str;
}

std::string concat(const tinja::Template<>::Tokens& tinjaTokens) {
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
            REQUIRE(microDoc == kainjowDoc);
            REQUIRE(kainjowDoc == injaDoc);

            tinja::Template tinjaTempl;
            const auto nodeCount = tinjaTempl.parse(basicString);
            std::cout << "tinja> template has: " << nodeCount << " nodes";
            const auto tinjaDoc = concat(tinjaTempl.render(tinjaData));
            REQUIRE(tinjaDoc == injaDoc);
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


        BENCHMARK_ADVANCED("tinja")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                tinja::Template templ(basicString);
                return templ.render(tinjaData);
            });
        };

        BENCHMARK_ADVANCED("tinja --32_nodes_reserved")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                tinja::Template<32> templ(basicString);
                return templ.render(tinjaData);
            });
        };

        BENCHMARK_ADVANCED("tinja --concatenated")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                tinja::Template templ(basicString);
                return concat(templ.render(tinjaData));
            });
        };

        BENCHMARK_ADVANCED("tinja --32_nodes_reserved --concatenated")(Catch::Benchmark::Chronometer meter) {
            meter.measure([&] {
                tinja::Template<32> templ(basicString);
                return concat(templ.render(tinjaData));
            });
        };
    }

    SECTION("preparsed") {
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

        BENCHMARK_ADVANCED("tinja")(Catch::Benchmark::Chronometer meter) {
            tinja::Template templ(basicString);
            meter.measure([&] { return templ.render(tinjaData); });
        };

        BENCHMARK_ADVANCED("tinja --concatenated")(Catch::Benchmark::Chronometer meter) {
            tinja::Template templ(basicString);
            meter.measure([&] { return concat(templ.render(tinjaData)); });
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

        SECTION("sanity check") {
            kainjow::mustache::mustache kainjowTempl(mustacheString);
            const auto kainjowDoc = kainjowTempl.render(kainjowData);
            const auto injaDoc = inja::render(injaString, injaData);
            REQUIRE(kainjowDoc == injaDoc);


            tinja::Template tinjaTempl;
            const auto tinjaNodeCount = tinjaTempl.parse(tinjaString);
            const auto tinjaTokens = tinjaTempl.render(tinjaData);
            std::cout << "tinja> " << tinjaNodeCount << " nodes parse to " << tinjaTokens.size() << " tokens" << std::endl;
            REQUIRE(injaDoc == concat(tinjaTokens));
        }

        BENCHMARK_ADVANCED("kainjow_mustache")(Catch::Benchmark::Chronometer meter) {
            kainjow::mustache::mustache doc(mustacheString);
            meter.measure([&] { return doc.render(kainjowData); });
        };

        BENCHMARK_ADVANCED("inja")(Catch::Benchmark::Chronometer meter) {
            inja::Environment env;
            inja::Template templ = env.parse(injaString);
            meter.measure([&] { return env.render(templ, injaData); });
        };

        BENCHMARK_ADVANCED("tinja")(Catch::Benchmark::Chronometer meter) {
            tinja::Template templ(tinjaString);
            meter.measure([&] { return templ.render(tinjaData); });
        };

        BENCHMARK_ADVANCED("tinja --concatenated")(Catch::Benchmark::Chronometer meter) {
            tinja::Template templ(tinjaString);
            meter.measure([&] { return concat(templ.render(tinjaData)); });
        };
    }
}
