
#ifndef FLUX_OP_FOR_EACH_HPP_INCLUDED
#define FLUX_OP_FOR_EACH_HPP_INCLUDED

#include <flux/op/for_each_while.hpp>

namespace flux {

namespace detail {

struct for_each_fn {

    template <sequence Seq, typename Func, typename Proj = std::identity>
        requires std::invocable<Func&, projected_t<Proj, Seq>>
    constexpr auto operator()(Seq&& seq, Func func, Proj proj = {}) const -> Func
    {
        (void) flux::for_each_while(FLUX_FWD(seq), [&](auto&& elem) {
            std::invoke(func, std::invoke(proj, FLUX_FWD(elem)));
            return true;
        });
        return func;
    }
};

} // namespace detail

inline constexpr auto for_each = detail::for_each_fn{};

template <typename D>
template <typename Func, typename Proj>
    requires std::invocable<Func&, projected_t<Proj, D>>
constexpr auto lens_base<D>::for_each(Func func, Proj proj) -> Func
{
    return flux::for_each(derived(), std::move(func), std::move(proj));
}

} // namespace flux

#endif