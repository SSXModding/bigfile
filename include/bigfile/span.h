#pragma once
#include <cstdint>

namespace bigfile {

	/**
	 * A non-owning wrapper object over a raw buffer sequence,
	 * that allows it to encode size in the same space.
	 * 
	 * \tparam T Type of the underlying memory.
	 */
	template<typename T>
	struct Span {
		constexpr Span() noexcept
			: memory_(nullptr),
			  size_(0) {
		}

		/**
		 * Construct a span with a valid memory address and size.
		 * 
		 * \param[in] memory The memory to wrap.
		 * \param[in] size The size of the buffer sequence.
		 */
		constexpr Span(T* memory, std::size_t size) noexcept
			: memory_(memory),
			  size_(size) {
		}

		/**
		 * Get the pointer stored in the span. 
		 */
		constexpr T* get() noexcept {
			return memory_;
		}

		/**
		 * Get the size of the buffer sequence stored in the span.
		 */
		constexpr std::size_t size() noexcept {
			return size_;
		}

		
		/**
		 * Get the raw size of the buffer sequence in the span.
		 */
		constexpr std::size_t buffer_size() noexcept {
			return size_ * sizeof(T);
		}

	   private:
		T* memory_;
		std::size_t size_;
	};

	/**
	 * Constexpr function to make spans.
	 * 
	 * \tparam T The type of the span to create
	 */
	template<typename T>
	constexpr static Span<T> MakeSpan(T* buffer, std::size_t size) noexcept {
		return Span<T> { buffer, size };
	}

} // namespace bigfile