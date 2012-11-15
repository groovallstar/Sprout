#ifndef SPROUT_COMPOST_FORMATS_AS_PCM_WAVE_HPP
#define SPROUT_COMPOST_FORMATS_AS_PCM_WAVE_HPP

#include <cstdint>
#include <limits>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/algorithm/clamp.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/type_traits/is_int.hpp>
#include <sprout/type_traits/is_uint.hpp>
#include <sprout/range/adaptor/transformed.hpp>

namespace sprout {
	namespace compost {
		namespace detail {
			template<typename IntType, typename = void>
			struct normalized_to_pcm_wave;
			template<typename IntType>
			struct normalized_to_pcm_wave<
				IntType,
				typename std::enable_if<sprout::is_int<IntType>::value>::type
			> {
			public:
				typedef IntType result_type;
			public:
				template<typename T>
				SPROUT_CONSTEXPR result_type operator()(T const& x) const {
					return static_cast<result_type>(sprout::clamp(x, -1, 1) * std::numeric_limits<result_type>::max());
				}
			};
			template<typename IntType>
			struct normalized_to_pcm_wave<
				IntType,
				typename std::enable_if<sprout::is_uint<IntType>::value>::type
			> {
			public:
				typedef IntType result_type;
			public:
				template<typename T>
				SPROUT_CONSTEXPR result_type operator()(T const& x) const {
					return static_cast<result_type>(sprout::clamp((x + 1) / 2, 0, 1) * std::numeric_limits<result_type>::max());
				}
			};
		}	// namespace detail
		//
		// normalized_to_pcm_wave
		//
		template<typename IntType>
		struct normalized_to_pcm_wave
			: public sprout::compost::detail::normalized_to_pcm_wave<IntType>
		{};

		namespace formats {
			//
			// as_pcm_wave_forwarder
			//
			template<typename IntType>
			class as_pcm_wave_forwarder {};

			//
			// as_pcm_wave
			//
			template<typename IntType>
			inline SPROUT_CONSTEXPR sprout::compost::formats::as_pcm_wave_forwarder<IntType>
			as_pcm_wave() {
				return sprout::compost::formats::as_pcm_wave_forwarder<IntType>();
			}

			//
			// operator|
			//
			template<typename Range, typename IntType>
			inline SPROUT_CONSTEXPR auto
			operator|(Range&& lhs, sprout::compost::formats::as_pcm_wave_forwarder<IntType> const& rhs)
			-> decltype(
				sprout::forward<Range>(lhs)
					| sprout::adaptors::transformed(sprout::compost::normalized_to_pcm_wave<IntType>())
				)
			{
				return sprout::forward<Range>(lhs)
					| sprout::adaptors::transformed(sprout::compost::normalized_to_pcm_wave<IntType>())
					;
			}

			//
			// as_pcm_wave8
			// as_pcm_wave16
			//
			namespace {
				SPROUT_STATIC_CONSTEXPR sprout::compost::formats::as_pcm_wave_forwarder<std::uint8_t> as_pcm_wave8{};
				SPROUT_STATIC_CONSTEXPR sprout::compost::formats::as_pcm_wave_forwarder<std::int16_t> as_pcm_wave16{};
			}	// anonymous-namespace
		}	// namespace formats
	}	// namespace compost
}	// namespace sprout

#endif	// #ifndef SPROUT_COMPOST_FORMATS_AS_PCM_WAVE_HPP
