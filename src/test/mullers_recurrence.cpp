#include <cnl/all.h>

#include <iostream>
#include <string>
#include <type_traits>

constexpr auto max_iterations = 55;

using namespace cnl::literals;
using cnl::abs;
using cnl::elastic_integer;
using cnl::fixed_point;
using cnl::fraction;
using cnl::int64;
using cnl::make_fraction;
using cnl::numeric_limits;
using cnl::quotient;
using cnl::static_number;

template<typename Scalar>
auto divide(Scalar const& n, Scalar const& d)
-> typename std::enable_if<std::is_floating_point<Scalar>::value, decltype(n / d)>::type {
    return n / d;
}

template<typename NumeratorRep, int NumeratorExponent, typename DenominatorRep, int DenominatorExponent>
auto divide(
        fixed_point<NumeratorRep, NumeratorExponent> const& n,
        fixed_point<DenominatorRep, DenominatorExponent> const& d)
-> decltype(quotient(n, d))
{
    return quotient(n, d);
}

template<
        typename NumeratorNumerator, typename NumeratorDenominator,
        typename DenominatorNumerator, typename DenominatorDenominator>
auto divide(
        fraction<NumeratorNumerator, NumeratorDenominator> const& n,
        fraction<DenominatorNumerator, DenominatorDenominator> const& d)
-> decltype(n / d)
{
    return n / d;
}

// from: https://medium.com/@bellmar/is-cobol-holding-you-hostage-with-math-5498c0eb428b
template<typename Scalar>
auto iterate(Scalar const& y, Scalar const& z)
{
    auto const a = divide(Scalar(1500.), z);
    auto const b = Scalar(815.) - Scalar{a};
    auto const c = divide(b, y);
    auto const d = Scalar(108.) - c;
    return d;
}

template<typename Destination, typename Denominator, typename Numerator>
Destination downscale(fraction<Denominator, Numerator> const& next)
{
    auto too_big = [](fraction<Denominator, Numerator> const& n) {
        return n.numerator>numeric_limits<typename Destination::numerator_type>::max()
                || n.denominator>numeric_limits<typename Destination::denominator_type>::max()
                || n.numerator<numeric_limits<typename Destination::numerator_type>::lowest()
                || n.denominator<numeric_limits<typename Destination::denominator_type>::lowest();
    };

    if (!too_big(next)) {
        return next;
    }

    auto reduced = next;

    auto const numerator_scale{abs(reduced.numerator)/numeric_limits<typename Destination::numerator_type>::max()};
    auto const denominator_scale{abs(reduced.denominator)/numeric_limits<typename Destination::denominator_type>::max()};
    auto const scale{std::max<decltype(numerator_scale|denominator_scale)>(numerator_scale, denominator_scale)};
    auto const scaled{Destination(reduced.denominator/scale, reduced.numerator/scale)};

    CNL_ASSERT(!too_big(scaled));
    return scaled;
}

template<typename Destination, typename Scalar>
Destination downscale(Scalar const& next)
{
    return next;
}


template<typename Scalar>
void test(char const* const title)
{
    using std::cout;
    cout << title << '\n';
    auto print_row = [&](int iteration, Scalar const& value) {
        cout << iteration << '\t' << static_cast<double>(value) << '\t' << value << '\n';
    };

    auto previous{Scalar(4.)};
    print_row(0, previous);

    auto current{Scalar(4.25)};
    print_row(1, current);

    auto divergence_iteration = 0;
    for (auto n = 2; n<max_iterations; ++n) {
        auto const next{iterate(current, previous)};
        if (next==current) {
            break;
        }
        previous = current;
        //previous = reduce(current);

        current = downscale<Scalar>(next);

        if (!divergence_iteration && abs(current-Scalar(5.)) > abs(previous-Scalar(5.))) {
            divergence_iteration = n;
        }

        print_row(n, current);
    }

    cout << "converged until " << divergence_iteration << "\n\n";
}

int main()
{
    test<float>("single precision floating point");
    test<double>("double precision floating point");
    test<long double>("extended precision floating point");
    test<fixed_point<elastic_integer<49>, -38>>("Q11.38");
    test<static_number<42, -30>>("static_number<42, -30>");
    test<static_number<127, -100>>("static_number<42, -30>");

    test<fraction<int64>>("fraction<int64>");
#if defined(CNL_INT128_ENABLED) && defined(__clang__)
    test<fraction<int128>>("fraction<int128>");
#endif
//    test<fraction<static_integer<127>>>("fraction<static_integer<127, -100>>");
//    test<fraction<overflow_integer<elastic_integer<31>, throwing_overflow_tag>>>(
//            "fraction<overflow_integer<elastic_integer<31>, throwing_overflow_tag>>");
}
