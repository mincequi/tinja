#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace tinja {

using String = std::string;
using StringRef = std::reference_wrapper<const String>;
using Strings = std::vector<String>;
using StringRefs = std::vector<StringRef>;
using Data = std::variant<String,StringRef,Strings,StringRefs>;
using DataMap = std::unordered_map<String, Data>;

// This is our overload operator
template<typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

class Template {
public:
    using Tokens = StringRefs;

    Template(size_t reserveNodes = 0) :
        _lastNodeCount(reserveNodes) {
    }

    Template(const String& str, size_t reserveNodes = 0) :
        _lastNodeCount(reserveNodes) {
        parse(str);
    }

    // Parse input string to nodes
    size_t parse(const String& str) {
        _nodes.clear();
        _nodes.reserve(_lastNodeCount);
        State state = State::Text;
        size_t pos = 0;
        size_t nextPos = 0;

        while (pos != String::npos) {
            switch (state) {
            case State::Text:
                state = parseText(str, pos, nextPos);
                break;
            case State::Variable:
                state = parseUntil<1>(str, pos, "}}", nextPos);
                break;
            case State::Array:
                state = parseUntil<2>(str, pos, "]}", nextPos);
                break;
            }
            pos = nextPos;
        }
        _lastNodeCount = _nodes.size();
        return _nodes.size();
    };

    void renderTo(const DataMap& dataMap, Tokens& tokens) const {
        tokens.clear();
        renderTo(dataMap, tokens, 0);
    }

private:
    using Text = String;
    using Variable = String;
    using Node = std::variant<Text, Variable, Template>;

    enum class State {
        Text,
        Variable,
        Array
    };

    State parseText(const String& str, size_t startPos, size_t& nextPos) {
        nextPos = str.find("{", nextPos);
        if (nextPos != str.npos && nextPos+3 < str.size()) {
            switch (str.at(nextPos+1)) {
            case '{':
                pushNode<0>(str, startPos, nextPos);
                nextPos += 2;
                return State::Variable;
            case '[':
                pushNode<0>(str, startPos, nextPos);
                nextPos += 2;
                return State::Array;
            default:
                parseText(str, startPos, ++nextPos);
            }
        }
        nextPos = String::npos;
        pushNode<0>(str, startPos, nextPos);
        return State::Text;
    }

    template<size_t S>
    State parseUntil(const String& str, size_t startPos, const String& until, size_t& nextPos) {
        nextPos = str.find(until, startPos);
        if (nextPos != str.npos) {
            pushNode<S>(str, startPos, nextPos);
        }
        nextPos = (nextPos != str.npos && nextPos+until.size() < str.size()) ? nextPos+until.size() : String::npos;
        return State::Text;
    }

    template<size_t S>
    void pushNode(const String& str, size_t from, size_t to) {
        if (from >= to || str.size() <= from)
            return;
        _nodes.emplace_back(Node { std::in_place_index<S>, str.substr(from, to-from) });
    }

    // Render tokens with data map
    void renderTo(const DataMap& dataMap, Tokens& tokens, size_t index) const {
        for (const auto& node : _nodes) {
            switch (node.index()) {
            case 0:
                tokens.push_back(std::get<0>(node));
                break;
            case 1: {
                const auto& var = std::get<1>(node);
                if (dataMap.count(var)) {
                    std::visit(Overload {
                        // Render regular variable
                        [&](const String& str) {
                            if (!str.empty())
                                tokens.push_back(str);
                        },
                        // Render array variable
                        [&](const Strings& v) {
                            if (!v.at(index).empty())
                                tokens.push_back(v.at(index));
                        },
                        [&](const StringRefs& v) {
                            if (!v.at(index).get().empty())
                                tokens.push_back(v.at(index));
                        }
                    }, dataMap.at(var));
                }
                break;
            }
            case 2: {
                const auto& doc = std::get<2>(node);
                const auto loopLength_ = doc.loopLength(dataMap);
                for (size_t i = 0; i < loopLength_; ++i) {
                    doc.renderTo(dataMap, tokens, i);
                }
                break;
            }
            default:
                break;
            }
        }
    }

    // Obtain loop length for vectorized variables in array
    size_t loopLength(const DataMap& dataMap) const {
        size_t length = std::numeric_limits<size_t>::max();
        for (const auto& v : _nodes) {
            if (const auto* pval = std::get_if<1>(&v)) {
                if (dataMap.count(*pval)) {
                    std::visit(Overload {
                        // Regular variables are ignored for loop length
                        [](const String&) {},
                        // Vectorized variable
                        [&](const Strings& v) {
                            length = std::min(length, v.size());
                        },
                        [&](const StringRefs& v) {
                            length = std::min(length, v.size());
                        },
                    }, dataMap.at(*pval));
                } else {
                    // Key not found
                    length = 0;
                    break;
                }
            }
        }
        return length == std::numeric_limits<size_t>::max() ? 1 : length;
    }

    std::vector<Node> _nodes;
    size_t _lastNodeCount = 0;
};

} // namespace tinja
