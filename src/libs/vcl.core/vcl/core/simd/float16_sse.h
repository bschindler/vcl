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
#include <vcl/core/simd/bool16_sse.h>
#include <vcl/core/simd/vectorscalar.h>
#include <vcl/core/simd/intrinsics_sse.h>

namespace Vcl
{
	template<>
	class VectorScalar<float, 16>
	{
	public:
		VCL_STRONG_INLINE VectorScalar() = default;
		VCL_STRONG_INLINE VectorScalar(float s)
		{
			mF4[0] = _mm_set1_ps(s);
			mF4[1] = _mm_set1_ps(s);
			mF4[2] = _mm_set1_ps(s);
			mF4[3] = _mm_set1_ps(s);
		}
		explicit VCL_STRONG_INLINE VectorScalar(const __m128& F4_0, const __m128& F4_1, const __m128& F4_2, const __m128& F4_3)
		{
			mF4[0] = F4_0;
			mF4[1] = F4_1;
			mF4[2] = F4_2;
			mF4[3] = F4_3;
		}

	public:
		VectorScalar<float, 16>& operator= (const VectorScalar<float, 16>& rhs)
		{
			mF4[0] = rhs.mF4[0];
			mF4[1] = rhs.mF4[1];
			mF4[2] = rhs.mF4[2];
			mF4[3] = rhs.mF4[3];
			
			return *this;
		}

	public:
		float& operator[] (int idx)
		{
			Require(0 <= idx && idx < 16, "Access is in range.");

			return mF4[idx / 4].m128_f32[idx % 4];
		}

		float operator[] (int idx) const
		{
			Require(0 <= idx && idx < 16, "Access is in range.");

			return mF4[idx / 4].m128_f32[idx % 4];
		}

		__m128 get(int i) const
		{
			Require(0 <= i && i < 4, "Access is in range.");

			return mF4[i];
		}

	public:
		VectorScalar<float, 16> operator- () const
		{
			return (*this) * VectorScalar<float, 16>(-1);
		}

	public:
		VectorScalar<float, 16> operator+ (const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<float, 16>
			(
				_mm_add_ps(mF4[0], rhs.mF4[0]),
				_mm_add_ps(mF4[1], rhs.mF4[1]),
				_mm_add_ps(mF4[2], rhs.mF4[2]),
				_mm_add_ps(mF4[3], rhs.mF4[3])
			);
		}

		VectorScalar<float, 16> operator- (const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<float, 16>
			(
				_mm_sub_ps(mF4[0], rhs.mF4[0]),
				_mm_sub_ps(mF4[1], rhs.mF4[1]),
				_mm_sub_ps(mF4[2], rhs.mF4[2]),
				_mm_sub_ps(mF4[3], rhs.mF4[3])
			);
		}

		VectorScalar<float, 16> operator* (const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<float, 16>
			(
				_mm_mul_ps(mF4[0], rhs.mF4[0]),
				_mm_mul_ps(mF4[1], rhs.mF4[1]),
				_mm_mul_ps(mF4[2], rhs.mF4[2]),
				_mm_mul_ps(mF4[3], rhs.mF4[3])
			);
		}

		VectorScalar<float, 16> operator/ (const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<float, 16>
			(
				_mm_div_ps(mF4[0], rhs.mF4[0]),
				_mm_div_ps(mF4[1], rhs.mF4[1]),
				_mm_div_ps(mF4[2], rhs.mF4[2]),
				_mm_div_ps(mF4[3], rhs.mF4[3])
			);
		}

	public:
		VectorScalar<float, 16>& operator += (const VectorScalar<float, 16>& rhs)
		{
			mF4[0] = _mm_add_ps(mF4[0], rhs.mF4[0]);
			mF4[1] = _mm_add_ps(mF4[1], rhs.mF4[1]);
			mF4[2] = _mm_add_ps(mF4[2], rhs.mF4[2]);
			mF4[3] = _mm_add_ps(mF4[3], rhs.mF4[3]);
			return *this;
		}
		VectorScalar<float, 16>& operator -= (const VectorScalar<float, 16>& rhs)
		{
			mF4[0] = _mm_sub_ps(mF4[0], rhs.mF4[0]);
			mF4[1] = _mm_sub_ps(mF4[1], rhs.mF4[1]);
			mF4[2] = _mm_sub_ps(mF4[2], rhs.mF4[2]);
			mF4[3] = _mm_sub_ps(mF4[3], rhs.mF4[3]);
			return *this;
		}
		VectorScalar<float, 16>& operator *= (const VectorScalar<float, 16>& rhs)
		{
			mF4[0] = _mm_mul_ps(mF4[0], rhs.mF4[0]);
			mF4[1] = _mm_mul_ps(mF4[1], rhs.mF4[1]);
			mF4[2] = _mm_mul_ps(mF4[2], rhs.mF4[2]);
			mF4[3] = _mm_mul_ps(mF4[3], rhs.mF4[3]);
			return *this;
		}
		VectorScalar<float, 16>& operator /= (const VectorScalar<float, 16>& rhs)
		{
			mF4[0] = _mm_div_ps(mF4[0], rhs.mF4[0]);
			mF4[1] = _mm_div_ps(mF4[1], rhs.mF4[1]);
			mF4[2] = _mm_div_ps(mF4[2], rhs.mF4[2]);
			mF4[3] = _mm_div_ps(mF4[3], rhs.mF4[3]);
			return *this;
		}

	public:
		VectorScalar<bool, 16> operator== (const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<bool, 16>
			(
				_mm_cmpeq_ps(mF4[0], rhs.mF4[0]),
				_mm_cmpeq_ps(mF4[1], rhs.mF4[1]),
				_mm_cmpeq_ps(mF4[2], rhs.mF4[2]),
				_mm_cmpeq_ps(mF4[3], rhs.mF4[3])
			);
		}

		VectorScalar<bool, 16> operator< (const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<bool, 16>
			(
				_mm_cmplt_ps(mF4[0], rhs.mF4[0]),
				_mm_cmplt_ps(mF4[1], rhs.mF4[1]),
				_mm_cmplt_ps(mF4[2], rhs.mF4[2]),
				_mm_cmplt_ps(mF4[3], rhs.mF4[3])
			);
		}
		VectorScalar<bool, 16> operator<= (const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<bool, 16>
			(
				_mm_cmple_ps(mF4[0], rhs.mF4[0]),
				_mm_cmple_ps(mF4[1], rhs.mF4[1]),
				_mm_cmple_ps(mF4[2], rhs.mF4[2]),
				_mm_cmple_ps(mF4[3], rhs.mF4[3])
			);
		}
		VectorScalar<bool, 16> operator> (const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<bool, 16>
			(
				_mm_cmpgt_ps(mF4[0], rhs.mF4[0]),
				_mm_cmpgt_ps(mF4[1], rhs.mF4[1]),
				_mm_cmpgt_ps(mF4[2], rhs.mF4[2]),
				_mm_cmpgt_ps(mF4[3], rhs.mF4[3])
			);
		}
		VectorScalar<bool, 16> operator>= (const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<bool, 16>
			(
				_mm_cmpge_ps(mF4[0], rhs.mF4[0]),
				_mm_cmpge_ps(mF4[1], rhs.mF4[1]),
				_mm_cmpge_ps(mF4[2], rhs.mF4[2]),
				_mm_cmpge_ps(mF4[3], rhs.mF4[3])
			);
		}

	public:
		VCL_STRONG_INLINE VectorScalar<float, 16> abs()  const { return VectorScalar<float, 16>(_mm_abs_ps (mF4[0]), _mm_abs_ps (mF4[1]), _mm_abs_ps (mF4[2]), _mm_abs_ps (mF4[3])); }
		VCL_STRONG_INLINE VectorScalar<float, 16> sin()  const { return VectorScalar<float, 16>(_mm_sin_ps (mF4[0]), _mm_sin_ps (mF4[1]), _mm_sin_ps (mF4[2]), _mm_sin_ps (mF4[3])); }
		VCL_STRONG_INLINE VectorScalar<float, 16> cos()  const { return VectorScalar<float, 16>(_mm_cos_ps (mF4[0]), _mm_cos_ps (mF4[1]), _mm_cos_ps (mF4[2]), _mm_cos_ps (mF4[3])); }
		VCL_STRONG_INLINE VectorScalar<float, 16> exp()  const { return VectorScalar<float, 16>(_mm_exp_ps (mF4[0]), _mm_exp_ps (mF4[1]), _mm_exp_ps (mF4[2]), _mm_exp_ps (mF4[3])); }
		VCL_STRONG_INLINE VectorScalar<float, 16> log()  const { return VectorScalar<float, 16>(_mm_log_ps (mF4[0]), _mm_log_ps (mF4[1]), _mm_log_ps (mF4[2]), _mm_log_ps (mF4[3])); }
		VCL_STRONG_INLINE VectorScalar<float, 16> sgn()  const { return VectorScalar<float, 16>(_mm_sgn_ps (mF4[0]), _mm_sgn_ps (mF4[1]), _mm_sgn_ps (mF4[2]), _mm_sgn_ps (mF4[3])); }
		VCL_STRONG_INLINE VectorScalar<float, 16> sqrt() const { return VectorScalar<float, 16>(_mm_sqrt_ps(mF4[0]), _mm_sqrt_ps(mF4[1]), _mm_sqrt_ps(mF4[2]), _mm_sqrt_ps(mF4[3])); }
		VCL_STRONG_INLINE VectorScalar<float, 16> rcp()  const { return VectorScalar<float, 16>(_mm_rcp_ps (mF4[0]), _mm_rcp_ps (mF4[1]), _mm_rcp_ps (mF4[2]), _mm_rcp_ps (mF4[3])); }

		VCL_STRONG_INLINE VectorScalar<float, 16> acos() const { return VectorScalar<float, 16>(_mm_acos_ps(mF4[0]), _mm_acos_ps(mF4[1]), _mm_acos_ps(mF4[2]), _mm_acos_ps(mF4[3])); }

	public:
		VCL_STRONG_INLINE VectorScalar<float, 16> min(const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<float, 16>
			(
				_mm_min_ps(mF4[0], rhs.mF4[0]),
				_mm_min_ps(mF4[1], rhs.mF4[1]),
				_mm_min_ps(mF4[2], rhs.mF4[2]),
				_mm_min_ps(mF4[3], rhs.mF4[3])
			);
		}
		VCL_STRONG_INLINE VectorScalar<float, 16> max(const VectorScalar<float, 16>& rhs) const
		{
			return VectorScalar<float, 16>
			(
				_mm_max_ps(mF4[0], rhs.mF4[0]),
				_mm_max_ps(mF4[1], rhs.mF4[1]),
				_mm_max_ps(mF4[2], rhs.mF4[2]),
				_mm_max_ps(mF4[3], rhs.mF4[3])
			);
		}

	public:
		friend std::ostream& operator<< (std::ostream &s, const VectorScalar<float, 16>& rhs);
		friend VectorScalar<float, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<float, 16>& a, const VectorScalar<float, 16>& b);

	private:
		std::array<__m128, 4> mF4;
	};

	VCL_STRONG_INLINE std::ostream& operator<< (std::ostream &s, const VectorScalar<float, 16>& rhs)
	{
		float VCL_ALIGN(16) vars[16];
		_mm_store_ps(vars +  0, rhs.mF4[0]);
		_mm_store_ps(vars +  4, rhs.mF4[1]);
		_mm_store_ps(vars +  8, rhs.mF4[2]);
		_mm_store_ps(vars + 12, rhs.mF4[3]);

		s << "'" << vars[ 0] << "," << vars[ 1] << "," << vars[ 2] << "," << vars[ 3]
		         << vars[ 4] << "," << vars[ 5] << "," << vars[ 6] << "," << vars[ 7]
				 << vars[ 8] << "," << vars[ 9] << "," << vars[10] << "," << vars[11]
				 << vars[12] << "," << vars[13] << "," << vars[14] << "," << vars[15] << "'";

		return s;
	}

	VCL_STRONG_INLINE VectorScalar<float, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<float, 16>& a, const VectorScalar<float, 16>& b)
	{
		// (((b ^ a) & mask)^b)
		return VectorScalar<float, 16>
		(
			_mm_xor_ps(b.mF4[0], _mm_and_ps(mask.mF4[0], _mm_xor_ps(b.mF4[0], a.mF4[0]))),
			_mm_xor_ps(b.mF4[1], _mm_and_ps(mask.mF4[1], _mm_xor_ps(b.mF4[1], a.mF4[1]))),
			_mm_xor_ps(b.mF4[2], _mm_and_ps(mask.mF4[2], _mm_xor_ps(b.mF4[2], a.mF4[2]))),
			_mm_xor_ps(b.mF4[3], _mm_and_ps(mask.mF4[3], _mm_xor_ps(b.mF4[3], a.mF4[3])))
		);
	}
}
