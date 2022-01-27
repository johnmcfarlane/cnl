
//          Copyright John McFarlane 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CHARCONV_TO_CHARS_CAPACITY_H)
#define CNL_IMPL_CHARCONV_TO_CHARS_CAPACITY_H

#include "../../integer.h"
#include "../cnl_assert.h"
#include "../numbers/signedness.h"

#include <array>
#include <limits>
#include <numbers>

namespace cnl::_impl {
    [[nodiscard]] constexpr auto num_digits_from_binary(int num_digits, int base)
    {
        CNL_ASSERT(base >= 2);
        CNL_ASSERT(base <= 36);

        // How many characters are needed to print numbers of this base (minus 2), relative to binary?
        constexpr auto integer_chars_factor{std::array<double, 37>{
                std::numeric_limits<double>::quiet_NaN(),
                std::numeric_limits<double>::infinity(),
                1.0000000000000000000000000000000000000000000000000000000,
                0.63092975357145753001475441124057397246360778808593750000,
                0.50000000000000000000000000000000000000000000000000000000,
                0.43067655807339311202497356134699657559394836425781250000,
                0.38685280723454162910002196440473198890686035156250000000,
                0.35620718710802223849754000184475444257259368896484375000,
                0.33333333333333342585191871876304503530263900756835937500,
                0.31546487678572876500737720562028698623180389404296875000,
                0.30102999566398119801746702250966336578130722045898437500,
                0.28906482631788787962534570397110655903816223144531250000,
                0.27894294565112986994392940687248483300209045410156250000,
                0.27023815442731979885593318613246083259582519531250000000,
                0.26264953503719362659651892499823588877916336059570312500,
                0.25595802480981555415695538613363169133663177490234375000,
                0.25000000000000000000000000000000000000000000000000000000,
                0.24465054211822603869030956502683693543076515197753906250,
                0.23981246656813148820930337024037726223468780517578125000,
                0.23540891336663827271280524655594490468502044677734375000,
                0.23137821315975920510865648793696891516447067260742187500,
                0.22767024869695301481087312822637613862752914428710937500,
                0.22424382421757543815132862619066145271062850952148437500,
                0.22106472945750377245843765194877050817012786865234375000,
                0.21810429198553157870144048047222895547747611999511718750,
                0.21533827903669655601248678067349828779697418212890625000,
                0.21274605355336317913383936684112995862960815429687500000,
                0.21030991785715250075305959853722015395760536193847656250,
                0.20801459767650948284867240545281674712896347045898437500,
                0.20584683246043447568673911973746726289391517639160156250,
                0.20379504709050619282173499868804356083273887634277343750,
                0.20184908658209987919462946592830121517181396484375000000,
                0.20000000000000001110223024625156540423631668090820312500,
                0.19823986317056055406204961855110013857483863830566406250,
                0.19656163223282260843483015833044191822409629821777343750,
                0.19495902189378633284633224320714361965656280517578125000,
                0.19342640361727081455001098220236599445343017578125000000}};

        return static_cast<int>(num_digits * integer_chars_factor[base]) + 1;
    }

    // maximum number of characters necessary to represent given Scalar
    // in give base in human-readable text excluding nul terminator,
    // e.g. to_chars_capacity<int8_t>{}() == 4  // ["-128".."127"]
    template<typename Scalar>
    struct to_chars_capacity;

    template<typename Scalar>
    requires integer<Scalar>
    struct to_chars_capacity<Scalar> {
        [[nodiscard]] constexpr auto operator()(int base = 10) const
        {
            static_assert(std::numeric_limits<Scalar>::radix == 2);

            auto const sign_chars = static_cast<int>(cnl::numbers::signedness_v<Scalar>);
            auto const integer_chars = num_digits_from_binary(std::numeric_limits<Scalar>::digits, base);
            return sign_chars + integer_chars;
        }
    };
}

#endif  // CNL_IMPL_CHARCONV_TO_CHARS_CAPACITY_H
