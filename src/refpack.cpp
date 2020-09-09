#include <bigfile/refpack.h>
#include <bigfile/endian_detection.h>

namespace bigfile {
	namespace refpack {

		// helper variable to not hardcode 3u all over the place
		constexpr auto uint24_size = 3u;

		// TODO: add a function to perform RefPack compression

		/**
		 * inline function to verify refpack magic of 0xFB
		 */
		inline bool CheckMagic(uint16 sig) { 
			byte val;

			// set value depending on endian
			if constexpr(current_endian == endian::little) {
				val = *(((byte*)&sig));
			} else if constexpr(current_endian == endian::big) {
				val = (((byte*)&sig)[1]);
			}

			return val == 0xFB;
		}

		std::vector<byte> Decompress(Span<byte> compressed) {
			const byte* in = compressed.get();

			// Command variables
			byte first;
			byte second;
			byte third;
			byte fourth;

			// output buffer
			std::vector<byte> out;

			uint32 proc_len;
			uint32 ref_run;
			byte* ref_ptr;

			// perform null & 0 size check
			if(!in || compressed.size() == 0)
				return {};

			uint16 signature = ((in[0] << 8) | in[1]);

			// a bit of robustness that was integrated into SSX Tricky's RefPack mechanism
			// but not integrated into TSO (which probably came from the EAC library directly like C&C)
			// is to check if the 0xFB magic exists before trying to decompress what could be 
			// malformed data.
			//
			// We do that. It's probably a good idea(TM).
			if(!CheckMagic(signature))
				return {};

			in += sizeof(uint16);

			// skip uint24 compressed size field
			if(signature & 0x0100)
				in += uint24_size;

			// read the uint24 decompressed size
			uint32 decompressed_size = ((in[0] << 16) | (in[1] << 8) | in[2]);
			in += uint24_size;

			// then resize output buffer

			out.resize(decompressed_size);
			byte* outptr = out.data();

			while(true) {
				// Retrive the first command byte
				first = *in++;

				if(!(first & 0x80)) {
					// 2-byte command: 0DDRRRPP DDDDDDDD
					second = *in++;

					proc_len = first & 0x03;

					for(uint32 i = 0; i < proc_len; i++)
						*outptr++ = *in++;

					ref_ptr = outptr - ((first & 0x60) << 3) - second - 1;
					ref_run = ((first >> 2) & 0x07) + 3;

					for(uint32 i = 0; i < ref_run; ++i)
						*outptr++ = *ref_ptr++;

				} else if(!(first & 0x40)) {
					// 3-byte command: 10RRRRRR PPDDDDDD DDDDDDDD
					second = *in++;
					third = *in++;

					proc_len = second >> 6;

					for(uint32 i = 0; i < proc_len; ++i)
						*outptr++ = *in++;

					ref_ptr = outptr - ((second & 0x3f) << 8) - third - 1;
					ref_run = (first & 0x3f) + 4;

					for(uint32 i = 0; i < ref_run; ++i)
						*outptr++ = *ref_ptr++;

				} else if(!(first & 0x20)) {
					// 4-byte command: 110DRRPP DDDDDDDD DDDDDDDD RRRRRRRR
					second = *in++;
					third = *in++;
					fourth = *in++;

					proc_len = first & 0x03;

					for(uint32 i = 0; i < proc_len; ++i)
						*outptr++ = *in++;

					ref_ptr = outptr - ((first & 0x10) << 12) - (second << 8) - third - 1;
					ref_run = ((first & 0x0c) << 6) + fourth + 5;

					for(uint32 i = 0; i < ref_run; ++i)
						*outptr++ = *ref_ptr++;
				} else {
					// 1-byte command: 111PPPPP

					proc_len = (first & 0x1f) * 4 + 4;

					if(proc_len <= 0x70) {
						// no stop flag

						for(uint32 i = 0; i < proc_len; ++i)
							*outptr++ = *in++;

					} else {
						// has a stop flag
						proc_len = first & 0x3;

						for(uint32 i = 0; i < proc_len; ++i)
							*outptr++ = *in++;

						break;
					}
				}
			}

			return out;
		}

	} // namespace refpack
} // namespace bigfile