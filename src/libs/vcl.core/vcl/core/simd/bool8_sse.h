/* 
 * This file is part of the Visual Computing Library (VCL) release under the
 * MIT license.
 *
 * Copyright (c) 2014 Basil Fierz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

// VCL configuration
#include <vcl/config/global.h>
#include <vcl/config/eigen.h>

// C++ Standard Library
#include <array>

// VCL
#include <vcl/core/simd/vectorscalar.h>

namespace Vcl
{
	template<>
	class VectorScalar<bool, 8>
	{
	public:
		VCL_STRONG_INLINE VectorScalar() = default;
		explicit VCL_STRONG_INLINE VectorScalar(__m128 F4_0, __m128 F4_1)
		{
			mF4[0] = F4_0;
			mF4[1] = F4_1;
		}
		explicit VCL_STRONG_INLINE VectorScalar(__m128i I4_0, __m128i I4_1)
		{
			mF4[0] = _mm_castsi128_ps(I4_0);
			mF4[1] = _mm_castsi128_ps(I4_1);
		}
		
	public:
		VectorScalar<bool, 8> operator&& (const VectorScalar<bool, 8>& rhs)
		{
			return VectorScalar<bool, 8>(_mm_and_ps(mF4[0], rhs.mF4[0]), _mm_and_ps(mF4[1], rhs.mF4[1]));
		}
		VectorScalar<bool, 8> operator|| (const VectorScalar<bool, 8>& rhs)
		{
			return VectorScalar<bool, 8>(_mm_or_ps(mF4[0], rhs.mF4[0]), _mm_or_ps(mF4[1], rhs.mF4[1]));
		}

	public:
		friend VectorScalar<float, 8> select(const VectorScalar<bool, 8>& mask, const VectorScalar<float, 8>& a, const VectorScalar<float, 8>& b);
		friend VectorScalar<int, 8> select(const VectorScalar<bool, 8>& mask, const VectorScalar<int, 8>& a, const VectorScalar<int, 8>& b);
		friend bool any(const VectorScalar<bool, 8>& b);
		friend bool all(const VectorScalar<bool, 8>& b);

	private:
		__m128 mF4[2];
	};

	VCL_STRONG_INLINE bool any(const VectorScalar<bool, 8>& b)
	{
		//int VCL_ALIGN(16) vars[8];
		//_mm_store_ps((float*) vars + 0, b.mF4[0]);
		//_mm_store_ps((float*) vars + 4, b.mF4[1]);
		//
		//return vars[0] | vars[1] | vars[2] | vars[3] |
		//       vars[4] | vars[5] | vars[6] | vars[7];

		int mask  = _mm_movemask_ps(b.mF4[1]) << 4;
		    mask |= _mm_movemask_ps(b.mF4[0]);

		return mask != 0;
	}

	VCL_STRONG_INLINE bool all(const VectorScalar<bool, 8>& b)
	{
		//int VCL_ALIGN(16) vars[8];
		//_mm_store_ps((float*) vars + 0, b.mF4[0]);
		//_mm_store_ps((float*) vars + 4, b.mF4[1]);
		//
		//return vars[0] & vars[1] & vars[2] & vars[3] &
		//       vars[4] & vars[5] & vars[6] & vars[7];

		int mask  = _mm_movemask_ps(b.mF4[1]) << 4;
		    mask |= _mm_movemask_ps(b.mF4[0]);
			
		return static_cast<unsigned int>(mask) == 0xff;
	}
}
