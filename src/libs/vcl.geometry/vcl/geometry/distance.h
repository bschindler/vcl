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

// C++ standard library
#include <array>

// VCL 
#include <vcl/math/math.h>
#include <vcl/core/contract.h>

namespace Vcl { namespace Geometry
{
	/*!
	 *	Point - Triangle distance computation refactored from Eberly's Geometric tools
	 *	to support SIMD execution
	 *	http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf
	 */
	namespace detail
	{
		using namespace Vcl::Mathematics;

		template<typename Real>
		VCL_STRONG_INLINE Eigen::Matrix<Real, 3, 1> computeDistanceRegion0(Real s, Real t, Real det, Real a, Real b, Real c, Real d, Real e, Real f)
		{
			Eigen::Matrix<Real, 3, 1> dist;

			Real inv_det = rcp(det);
			s *= inv_det;
			t *= inv_det;
			dist(0) = s*(a*s + b*t + ((Real)2.0)*d) +
				      t*(b*s + c*t + ((Real)2.0)*e) + f;
			dist(1) = s;
			dist(2) = t;

			return dist;
		}

		template<typename Real>
		VCL_STRONG_INLINE Eigen::Matrix<Real, 3, 1> computeDistanceRegion1(Real det, Real a, Real b, Real c, Real d, Real e, Real f)
		{
			VCL_UNREFERENCED_PARAMETER(det);

			Eigen::Matrix<Real, 3, 1> dist;

			Real numer = c + e - b - d;
			Real denom = a - b*2 + c;

			Real s_a = 0;
			Real s_b = 1;
			Real s_c = numer * rcp(denom);

			Real t_a = 1;
			Real t_b = 0;
			Real t_c = (Real)1.0 - s_c;

			Real d_a = c + ((Real)2.0)*e + f;
			Real d_b = a + ((Real)2.0)*d + f;
			Real d_c = s_c*(a*s_c + b*t_c + ((Real)2.0)*d) +
					   t_c*(b*s_c + c*t_c + ((Real)2.0)*e) + f;

			dist(0) = select
			(
				numer <= (Real)0.0,
				d_a,
				select
				(
					numer >= denom,
					d_b,
					d_c
				)
			);
			dist(1) = select
			(
				numer <= (Real)0.0,
				s_a,
				select
				(
					numer >= denom,
					s_b,
					s_c
				)
			);
			dist(2) = select
			(
				numer <= (Real)0.0,
				t_a,
				select
				(
					numer >= denom,
					t_b,
					t_c
				)
			);

			return dist;
		}

		template<typename Real>
		VCL_STRONG_INLINE Eigen::Matrix<Real, 3, 1> computeDistanceRegion2(Real det, Real a, Real b, Real c, Real d, Real e, Real f)
		{
			VCL_UNREFERENCED_PARAMETER(det);

			Eigen::Matrix<Real, 3, 1> dist;

			Real tmp0 = b + d;
			Real tmp1 = c + e;
			Real numer = tmp1 - tmp0;
			Real denom = a - b*2 + c;

			Real s_a = 1;
			Real s_b = numer * rcp(denom);
			Real s_c = 0;
			Real s_d = 0;
			Real s_e = 0;

			Real t_a = 0;
			Real t_b = (Real)1.0 - s_b;
			Real t_c = 1;
			Real t_d = 0;
			Real t_e = -e * rcp(c);

			Real d_a = a + ((Real)2.0)*d + f;
			Real d_b = s_b*(a*s_b + b*t_b + d*2) +
				       t_b*(b*s_b + c*t_b + ((Real)2.0)*e) + f;
			Real d_c = c + ((Real)2.0)*e + f;
			Real d_d = f;
			Real d_e = e*t_e + f;

			dist(0) = select(tmp1 > tmp0,
				select(numer >= denom, d_a, d_b),
				select(tmp1 <= (Real)0.0, d_c,
					select(e >= (Real)0.0, d_d, d_e)));

			dist(1) = select(tmp1 > tmp0,
				select(numer >= denom, s_a, s_b),
				select(tmp1 <= (Real)0.0, s_c,
					select(e >= (Real)0.0, s_d, s_e)));

			dist(2) = select(tmp1 > tmp0,
				select(numer >= denom, t_a, t_b),
				select(tmp1 <= (Real)0.0, t_c,
					select(e >= (Real)0.0, t_d, t_e)));
			return dist;
		}

		template<typename Real>
		VCL_STRONG_INLINE Eigen::Matrix<Real, 3, 1> computeDistanceRegion3(Real det, Real a, Real b, Real c, Real d, Real e, Real f)
		{
			VCL_UNREFERENCED_PARAMETER(det);
			VCL_UNREFERENCED_PARAMETER(a);
			VCL_UNREFERENCED_PARAMETER(b);
			VCL_UNREFERENCED_PARAMETER(d);

			Eigen::Matrix<Real, 3, 1> dist;

			Real t_a = 0;
			Real t_b = 1;
			Real t_c = -e * rcp(c);

			Real sq_d_a = f;
			Real sq_d_b = c + ((Real)2.0)*e + f;
			Real sq_d_c = e*t_c + f;

			dist(0) = select
			(
				e >= (Real)0.0,
				sq_d_a,
				select
				(
					-e >= c,
					sq_d_b,
					sq_d_c
				)
			);
			dist(1) = 0;
			dist(2) = select
			(
				e >= (Real)0.0,
				t_a,
				select
				(
					-e >= c,
					t_b,
					t_c
				)
			);

			return dist;
		}

		template<typename Real>
		VCL_STRONG_INLINE Eigen::Matrix<Real, 3, 1> computeDistanceRegion4(Real det, Real a, Real b, Real c, Real d, Real e, Real f)
		{
			VCL_UNREFERENCED_PARAMETER(det);
			VCL_UNREFERENCED_PARAMETER(b);

			Eigen::Matrix<Real, 3, 1> dist;

			Real s_a = 1;
			Real s_b = -d * rcp(a);
			Real s_c = 0;
			Real s_d = 0;
			Real s_e = 0;
					   
			Real t_a = 0;
			Real t_b = 0;
			Real t_c = 0;
			Real t_d = 1;
			Real t_e = -e * rcp(c);
					   
			Real d_a = a + ((Real)2.0)*d + f;
			Real d_b = d*s_b + f;
			Real d_c = f;
			Real d_d = c + ((Real)2.0)*e + f;
			Real d_e = e*t_e + f;

			dist(0) = select(d < (Real)0.0, 
				select(-d >= a, d_a, d_b),
				select(e >= (Real)0.0, d_c,
					select(-e >= c, d_d, d_e)));
					
			dist(1) = select(d < (Real)0.0, 
				select(-d >= a, s_a, s_b),
				select(e >= (Real)0.0, s_c,
					select(-e >= c, s_d, s_e)));
					
			dist(2) = select(d < (Real)0.0, 
				select(-d >= a, t_a, t_b),
				select(e >= (Real)0.0, t_c,
					select(-e >= c, t_d, t_e)));

			return dist;
		}

		template<typename Real>
		VCL_STRONG_INLINE Eigen::Matrix<Real, 3, 1> computeDistanceRegion5(Real det, Real a, Real b, Real c, Real d, Real e, Real f)
		{
			VCL_UNREFERENCED_PARAMETER(det);
			VCL_UNREFERENCED_PARAMETER(b);
			VCL_UNREFERENCED_PARAMETER(c);
			VCL_UNREFERENCED_PARAMETER(e);

			Eigen::Matrix<Real, 3, 1> dist;

			Real s_a = 0;
			Real s_b = 1;
			Real s_c = -d * rcp(a);

			Real d_a = f;
			Real d_b = a + d*2 + f;
			Real d_c = d*s_c + f;

			dist(0) = select(d >= 0,
				d_a,
				select(-d >= a, d_b, d_c));
			dist(1) = select(d >= 0, 
				s_a,
				select(-d >= a, s_b, s_c));
			dist(2) = 0;
			return dist;
		}

		template<typename Real>
		VCL_STRONG_INLINE Eigen::Matrix<Real, 3, 1> computeDistanceRegion6(Real det, Real a, Real b, Real c, Real d, Real e, Real f)
		{
			VCL_UNREFERENCED_PARAMETER(det);

			Eigen::Matrix<Real, 3, 1> dist;

			Real tmp0 = b + e;
			Real tmp1 = a + d;
			Real numer = tmp1 - tmp0;
			Real denom = a - ((Real)2.0)*b + c;

			Real t_a = 1;
			Real t_b = numer * rcp(denom);
			Real t_c = 0;
			Real t_d = 0;
			Real t_e = 0;

			Real s_a = 0;
			Real s_b = (Real)1.0 - t_b;
			Real s_c = 1;
			Real s_d = 0;
			Real s_e = -d * rcp(a);
					   
			Real d_a = c + ((Real)2.0)*e + f;
			Real d_b = s_b*(a*s_b + b*t_b + ((Real)2.0)*d) +
					   t_b*(b*s_b + c*t_b + ((Real)2.0)*e) + f;
			Real d_c = a + ((Real)2.0)*d + f;
			Real d_d = f;
			Real d_e = d*s_e + f;

			dist(0) = select(tmp1 > tmp0,
				select(numer >= denom, d_a, d_b),
				select(tmp1 <= (Real)0.0, d_c,
					select(d >= (Real)0.0, d_d, d_e)));
			
			dist(1) = select(tmp1 > tmp0,
				select(numer >= denom, s_a, s_b),
				select(tmp1 <= (Real)0.0, s_c,
					select(d >= (Real)0.0, s_d, s_e)));

			dist(2) = select(tmp1 > tmp0,
				select(numer >= denom, t_a, t_b),
				select(tmp1 <= (Real)0.0, t_c,
					select(d >= (Real)0.0, t_d, t_e)));

			return dist;
		}
	}

	template<typename Real>
	Real distance
	(
		const Eigen::Matrix<Real, 3, 1>& v0,
		const Eigen::Matrix<Real, 3, 1>& v1,
		const Eigen::Matrix<Real, 3, 1>& v2,
		const Eigen::Matrix<Real, 3, 1>& p,
		int* r = nullptr
	)
	{
		using namespace Vcl::Mathematics;

		Eigen::Matrix<Real, 3, 1> P = p;
		Eigen::Matrix<Real, 3, 1> B = v0;
		Eigen::Matrix<Real, 3, 1> E0 = v1 - v0;
		Eigen::Matrix<Real, 3, 1> E1 = v2 - v0;
		Real a = E0.squaredNorm();
		Real b = E0.dot(E1);
		Real c = E1.squaredNorm();
		Real d = (B - P).dot(E0);
		Real e = (B - P).dot(E1);
		Real f = (B - P).squaredNorm();
		Real det = abs(a*c-b*b);
		Real s = b*e-c*d;
		Real t = b*d-a*e;

		// Compute the results for all the regions
		//Eigen::Matrix<Real, 3, 1> dist[7];
		Real dist[7];
		dist[0] = detail::computeDistanceRegion0(s, t, det, a, b, c, d, e, f)[0];
		dist[1] = detail::computeDistanceRegion1(det, a, b, c, d, e, f)[0];
		dist[2] = detail::computeDistanceRegion2(det, a, b, c, d, e, f)[0];
		dist[3] = detail::computeDistanceRegion3(det, a, b, c, d, e, f)[0];
		dist[4] = detail::computeDistanceRegion4(det, a, b, c, d, e, f)[0];
		dist[5] = detail::computeDistanceRegion5(det, a, b, c, d, e, f)[0];
		dist[6] = detail::computeDistanceRegion6(det, a, b, c, d, e, f)[0];

		Real sq_dist = select
		(
			s + t <= det, 
			select
			(
				s < (Real)0.0,
				select(t < (Real)0.0, dist[4], dist[3]), 
				select(t < (Real)0.0, dist[5], dist[0])
			),
			select
			(
				s < (Real)0.0,
				dist[2],
				select(t < (Real)0.0, dist[6], dist[1])
			)
		);

		//int region = select
		//(
		//	s + t <= det, 
		//	select
		//	(
		//		s < (Real)0.0,
		//		select(t < (Real)0.0, 4, 3), 
		//		select(t < (Real)0.0, 5, 0)
		//	),
		//	select
		//	(
		//		s < (Real)0.0,
		//		2,
		//		select(t < (Real)0.0, 6, 1)
		//	)
		//);

		// Account for numerical round-off error
		//dist[region](0) = std::max((Real) 0, dist[region](0));
		sq_dist = max((Real) 0, sq_dist);

		//if (r != nullptr)
		//	*r = region;
		if (r != nullptr)
			*r = -1;

		/*m_kClosestPoint0 = P;
		m_kClosestPoint1 = B + s*E0+ t*E1;
		m_afTriangleBary[1] = s;
		m_afTriangleBary[2] = t;
		m_afTriangleBary[0] = (Real)1.0 - s - t;*/
		return sqrt(sq_dist);
	}
}}