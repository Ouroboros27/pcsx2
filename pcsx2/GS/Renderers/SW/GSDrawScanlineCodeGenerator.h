/*
 *	Copyright (C) 2007-2009 Gabest
 *	http://www.gabest.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#pragma once

#include "GSScanlineEnvironment.h"
#include "../Common/GSFunctionMap.h"
#include "../../GSUtil.h"

#if defined(_M_AMD64) || defined(_WIN64)
#define RegLong Xbyak::Reg64
#else
#define RegLong Xbyak::Reg32
#endif

class GSDrawScanlineCodeGenerator : public GSCodeGenerator
{
	void operator=(const GSDrawScanlineCodeGenerator&);

	GSScanlineSelector m_sel;
	GSScanlineLocalData& m_local;
	bool m_rip;

	void Generate();

#if _M_SSE >= 0x501

	void Init();
	void Step();
	void TestZ(const Ymm& temp1, const Ymm& temp2);
	void SampleTexture();
	void Wrap(const Ymm& uv0);
	void Wrap(const Ymm& uv0, const Ymm& uv1);
	void SampleTextureLOD();
	void WrapLOD(const Ymm& uv0);
	void WrapLOD(const Ymm& uv0, const Ymm& uv1);
	void AlphaTFX();
	void ReadMask();
	void TestAlpha();
	void ColorTFX();
	void Fog();
	void ReadFrame();
	void TestDestAlpha();
	void WriteMask();
	void WriteZBuf();
	void AlphaBlend();
	void WriteFrame();
	void ReadPixel(const Ymm& dst, const Ymm& temp, const RegLong& addr);
	void WritePixel(const Ymm& src, const Ymm& temp, const RegLong& addr, const Reg32& mask, bool fast, int psm, int fz);
	void WritePixel(const Xbyak::Xmm& src, const RegLong& addr, uint8 i, uint8 j, int psm);
	void ReadTexel(int pixels, int mip_offset = 0);
	void ReadTexel(const Ymm& dst, const Ymm& addr, uint8 i);

#else

	void Generate_SSE();
	void Init_SSE();
	void Step_SSE();
	void TestZ_SSE(const Xbyak::Xmm& temp1, const Xbyak::Xmm& temp2);
	void SampleTexture_SSE();
	void Wrap_SSE(const Xbyak::Xmm& uv0);
	void Wrap_SSE(const Xbyak::Xmm& uv0, const Xbyak::Xmm& uv1);
	void SampleTextureLOD_SSE();
	void WrapLOD_SSE(const Xbyak::Xmm& uv0);
	void WrapLOD_SSE(const Xbyak::Xmm& uv0, const Xbyak::Xmm& uv1);
	void AlphaTFX_SSE();
	void ReadMask_SSE();
	void TestAlpha_SSE();
	void ColorTFX_SSE();
	void Fog_SSE();
	void ReadFrame_SSE();
	void TestDestAlpha_SSE();
	void WriteMask_SSE();
	void WriteZBuf_SSE();
	void AlphaBlend_SSE();
	void WriteFrame_SSE();
	void ReadPixel_SSE(const Xbyak::Xmm& dst, const RegLong& addr);
	void WritePixel_SSE(const Xbyak::Xmm& src, const RegLong& addr, const Xbyak::Reg8& mask, bool fast, int psm, int fz);
	void WritePixel_SSE(const Xbyak::Xmm& src, const RegLong& addr, uint8 i, int psm);
	void ReadTexel_SSE(int pixels, int mip_offset = 0);
	void ReadTexel_SSE(const Xbyak::Xmm& dst, const Xbyak::Xmm& addr, uint8 i);

	void Generate_AVX();
	void Init_AVX();
	void Step_AVX();
	void TestZ_AVX(const Xbyak::Xmm& temp1, const Xbyak::Xmm& temp2);
	void SampleTexture_AVX();
	void Wrap_AVX(const Xbyak::Xmm& uv0);
	void Wrap_AVX(const Xbyak::Xmm& uv0, const Xbyak::Xmm& uv1);
	void SampleTextureLOD_AVX();
	void WrapLOD_AVX(const Xbyak::Xmm& uv0);
	void WrapLOD_AVX(const Xbyak::Xmm& uv0, const Xbyak::Xmm& uv1);
	void AlphaTFX_AVX();
	void ReadMask_AVX();
	void TestAlpha_AVX();
	void ColorTFX_AVX();
	void Fog_AVX();
	void ReadFrame_AVX();
	void TestDestAlpha_AVX();
	void WriteMask_AVX();
	void WriteZBuf_AVX();
	void AlphaBlend_AVX();
	void WriteFrame_AVX();
	void ReadPixel_AVX(const Xbyak::Xmm& dst, const RegLong& addr);
	void WritePixel_AVX(const Xbyak::Xmm& src, const RegLong& addr, const Xbyak::Reg8& mask, bool fast, int psm, int fz);
	void WritePixel_AVX(const Xbyak::Xmm& src, const RegLong& addr, uint8 i, int psm);
	void ReadTexel_AVX(int pixels, int mip_offset = 0);
	void ReadTexel_AVX(const Xbyak::Xmm& dst, const Xbyak::Xmm& addr, uint8 i);

#endif

	void modulate16(const Xbyak::Xmm& a, const Xbyak::Operand& f, uint8 shift);
	void lerp16(const Xbyak::Xmm& a, const Xbyak::Xmm& b, const Xbyak::Xmm& f, uint8 shift);
	void lerp16_4(const Xbyak::Xmm& a, const Xbyak::Xmm& b, const Xbyak::Xmm& f);
	void mix16(const Xbyak::Xmm& a, const Xbyak::Xmm& b, const Xbyak::Xmm& temp);
	void clamp16(const Xbyak::Xmm& a, const Xbyak::Xmm& temp);
	void alltrue(const Xbyak::Xmm& test);
	void blend(const Xbyak::Xmm& a, const Xbyak::Xmm& b, const Xbyak::Xmm& mask);
	void blendr(const Xbyak::Xmm& b, const Xbyak::Xmm& a, const Xbyak::Xmm& mask);
	void blend8(const Xbyak::Xmm& a, const Xbyak::Xmm& b);
	void blend8r(const Xbyak::Xmm& b, const Xbyak::Xmm& a);
	void split16_2x8(const Xbyak::Xmm& l, const Xbyak::Xmm& h, const Xbyak::Xmm& src);

public:
	GSDrawScanlineCodeGenerator(void* param, uint64 key, void* code, size_t maxsize);
};
