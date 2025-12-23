#include <nlohmann/json.hpp>
#include <print>
#include <ranges>
#include <string>
#include <variant>

struct UserDto {
    std::string Name = {};
    std::string Email = {};
    int Id = 0;
};

struct ProductDto {
    std::string Name = {};
    double Price = 0.0;
    int Stock = 0;
    int Id = 0;
};

struct OrderDto {
    std::vector<int> Products = {};
    double Total = 0.0;
    int Id = 0;
    int User = 0;
};

class JsonVisitor {
public:
    // one business logic / algo for multiple types ? if constexpr && visitor
    // pattern is good
    template <typename T>
    static inline T To(const nlohmann::json& json) {
        if constexpr (std::same_as<T, UserDto>) {
            return toUser(json);
        } else if constexpr (std::same_as<T, ProductDto>) {
            return toProduct(json);
        } else if constexpr (std::same_as<T, OrderDto>) {
            return toOrder(json);
        } else {
            std::unreachable();
        }
    }

private:
    // different nlohmann data access methods in each
    static inline UserDto toUser(nlohmann::json const& json) {
        return UserDto{
            .Name  = json.at("name").get<std::string>(),
            .Email = json.at("email").get<std::string>(),
            .Id    = json.at("id").get<int>()
        };
    }

    static inline ProductDto toProduct(nlohmann::json const& json) {
        return ProductDto{
            .Name  = json.value("name", ""),
            .Price = json.value("price", 0.0),
            .Stock = json.value("stock", 0),
            .Id    = json.value("id", -1)
        };
    }

    static inline OrderDto toOrder(nlohmann::json const& json) {
        OrderDto order;
        json.at("products").get_to(order.Products);
        json.at("total").get_to(order.Total);
        json.at("id").get_to(order.Id);
        json.at("user").get_to(order.User);

        return order;
    }
};

int main() {
    // prepare dummy data
    nlohmann::json user = {
        {"name", "hakan gedek"},
        {"id", 1},
        {"email", "hgedek@pm.me"}
    };

    nlohmann::json product = {
        {"name", "thinkpad p3"},
        {"price", 888},
        {"stock", 33},
        {"id", 31}
    };

    nlohmann::json order = {
        {"products", {31}},
        {"total", 888},
        {"id", 21},
        {"user", 1}
    };

    // prepare the dtos
    auto usrDto = JsonVisitor::To<UserDto>(user);
    auto proDto = JsonVisitor::To<ProductDto>(product);
    auto ordDto = JsonVisitor::To<OrderDto>(order);

    using Dto = std::variant<UserDto, ProductDto, OrderDto>;
    std::vector<Dto> dtos = {usrDto, proDto, ordDto};

    // ready to use visitor
    for (auto const& dto : dtos) {
        std::visit(
            [](const auto& data) {
                using T = std::decay_t<decltype(data)>;

                if constexpr (std::same_as<T, UserDto>) {
                    std::println("User => Id: {} Name: {} Email: {}",
                                 data.Id, data.Name, data.Email);
                } else if constexpr (std::same_as<T, ProductDto>) {
                    std::println("Product => Id: {} Name: {} Price: {} Stock: {}",
                                 data.Id, data.Name, data.Price, data.Stock);
                } else if constexpr (std::same_as<T, OrderDto>) {
                    auto Products =
                        data.Products
                        | std::views::transform([](int n) {
                              return std::to_string(n);
                          })
                        | std::views::join_with(',')
                        | std::ranges::to<std::string>();

                    std::println(
                        "Order => Id: {} User: {} Total: {} Products: {}",
                        data.Id, data.User, data.Total, Products);
                } else {
                    std::unreachable();
                }
            },
            dto);
    }
}