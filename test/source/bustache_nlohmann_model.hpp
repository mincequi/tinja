#ifndef BUSTACHE_NLOHMANN_MODEL_HPP
#define BUSTACHE_NLOHMANN_MODEL_HPP

#include <json.hpp>

template<>
struct bustache::impl_model<nlohmann::json> {
    impl_model() = delete;
};

template<>
struct bustache::impl_compatible<nlohmann::json> {
    static value_ptr get_value_ptr(nlohmann::json const& self) {
        nlohmann::json::value_t const kind(self);
        switch (kind) {
        case nlohmann::json::value_t::boolean: return value_ptr(self.get_ptr<nlohmann::json::boolean_t const*>());
        case nlohmann::json::value_t::number_integer: return value_ptr(self.get_ptr<nlohmann::json::number_integer_t const*>());
        case nlohmann::json::value_t::number_unsigned: return value_ptr(self.get_ptr<nlohmann::json::number_unsigned_t const*>());
        case nlohmann::json::value_t::number_float: return value_ptr(self.get_ptr<nlohmann::json::number_float_t const*>());
        case nlohmann::json::value_t::string: return value_ptr(self.get_ptr<nlohmann::json::string_t const*>());
        case nlohmann::json::value_t::array: return value_ptr(self.get_ptr<nlohmann::json::array_t const*>());
        case nlohmann::json::value_t::object: return value_ptr(self.get_ptr<nlohmann::json::object_t const*>());
        }
        return value_ptr();
    }
};

#endif // BUSTACHE_NLOHMANN_MODEL_HPP
