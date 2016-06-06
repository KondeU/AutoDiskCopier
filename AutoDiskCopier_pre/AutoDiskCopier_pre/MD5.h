#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include<fstream>

namespace MD5
{
	typedef unsigned int  uint;
	typedef unsigned char uint8;
	typedef unsigned long uint32;
	typedef unsigned long long uint64;


	class CMD5
	{

	public:

		CMD5(
			uint32 ui32MD5ChainingA = 0x67452301ul, //标准幻数A：(01234567)hex
			uint32 ui32MD5ChainingB = 0xEFCDAB89ul, //标准幻数B：(89ABCDEF)hex
			uint32 ui32MD5ChainingC = 0x98BADCFEul, //标准幻数C：(FEDCBA98)hex
			uint32 ui32MD5ChainingD = 0x10325476ul  //标准幻数D：(76543210)hex
			);

		//重置数据
		inline void Reset();
		
		//获取MD5值
		inline char * GetMD5(char * szMD5) const;

		//MD5：字节流
		void CalcMD5(const void * pvDataStream, uint32 ui32DataBytes);
		//MD5：字符串
		void CalcMD5(const char * szString);
		//MD5：文件
		void CalcMD5(const char * szFilePath, bool bIsBinary);

	private:
		
		static const uint   msk_uiMD5TransformShiftMatrix[4][4];  //MD5变换移位矩阵
		static const uint32 msk_ui32MD5TransformAddMatrix[4][16]; //MD5变换增量矩阵

		static const uint8  msk_ui8MD5PaddingArray[64]; //MD5拓展长度数组

		uint32 m_ui32MD5ChainingA; //MD5链接变量A
		uint32 m_ui32MD5ChainingB; //MD5链接变量B
		uint32 m_ui32MD5ChainingC; //MD5链接变量C
		uint32 m_ui32MD5ChainingD; //MD5链接变量D

		uint32 m_ui32State[4];       //MD5算法当前计算状态值

		uint32 m_ui32ByteCounts[2];  //数据总字节数

		uint8  m_ui8InputBuffer[64]; //输入缓冲区

		char   m_szMD5[33];          //MD5计算结果

		bool   m_bResetFlag;         //复位标志

		inline void Uint32to8(uint8 * pui8Output, const uint32 * pui32Input, uint uiInputByteCounts) const;
		inline void Uint8to32(uint32 * pui32Output, const uint8 * pui8Input, uint uiInputByteCounts) const;

		inline uint32 F(const uint32 x, const uint32 y, const uint32 z) const;
		inline uint32 G(const uint32 x, const uint32 y, const uint32 z) const;
		inline uint32 H(const uint32 x, const uint32 y, const uint32 z) const;
		inline uint32 I(const uint32 x, const uint32 y, const uint32 z) const;
		
		inline uint32 RotateLeft(const uint32 x, const uint uiShiftCount) const;

		inline void FF(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint s, uint32 ac);
		inline void GG(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint s, uint32 ac);
		inline void HH(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint s, uint32 ac);
		inline void II(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint s, uint32 ac);

		void Transform(const uint8 ui8Block[64]);

		void Update(const uint8 * pui8Input, uint32 ui32InputBytes);
		
		void Calculate();

	};
	
	//初始化MD5变换移位矩阵
	const uint CMD5::msk_uiMD5TransformShiftMatrix[4][4] = 
	{
		{7, 12, 17, 22},
		{5,  9, 14, 20},
		{4, 11, 16, 23},
		{6, 10, 15, 21}
	};

	//初始化MD5变换增量矩阵
	const uint32 CMD5::msk_ui32MD5TransformAddMatrix[4][16] = 
	{
		/*
			loop i=1..64 : 4294967296 * abs(sin(i))
			4294967296 = 2^32
		*/

		{
			0xD76AA478, //0xd76aa478
			0xE8C7B756, //0xe8c7b756
			0x242070DB, //0x242070db
			0xC1BDCEEE, //0xc1bdceee
			0xF57C0FAF, //0xf57c0faf
			0x4787C62A, //0x4787c62a
			0xA8304613, //0xa8304613
			0xFD469501, //0xfd469501
			0x698098D8, //0x698098d8
			0x8B44F7AF, //0x8b44f7af
			0xFFFF5BB1, //0xffff5bb1
			0x895CD7BE, //0x895cd7be
			0x6B901122, //0x6b901122
			0xFD987193, //0xfd987193
			0xA679438E, //0xa679438e
			0x49B40821  //0x49b40821
		},

		{
			0xF61E2562, //0xf61e2562
			0xC040B340,	//0xc040b340
			0x265E5A51,	//0x265e5a51
			0xE9B6C7AA,	//0xe9b6c7aa
			0xD62F105D,	//0xd62f105d
			0x02441453,	//0x02441453
			0xD8A1E681,	//0xd8a1e681
			0xE7D3FBC8,	//0xe7d3fbc8
			0x21E1CDE6,	//0x21e1cde6
			0xC33707D6,	//0xc33707d6
			0xF4D50D87,	//0xf4d50d87
			0x455A14ED,	//0x455a14ed
			0xA9E3E905,	//0xa9e3e905
			0xFCEFA3F8,	//0xfcefa3f8
			0x676F02D9,	//0x676f02d9
			0x8D2A4C8A  //0x8d2a4c8a
		},

		{
			0xFFFA3942, //0xfffa3942
			0x8771F681, //0x8771f681
			0x6D9D6122, //0x6d9d6122
			0xFDE5380C, //0xfde5380c
			0xA4BEEA44, //0xa4beea44
			0x4BDECFA9, //0x4bdecfa9
			0xF6BB4B60, //0xf6bb4b60
			0xBEBFBC70, //0xbebfbc70
			0x289B7EC6, //0x289b7ec6
			0xEAA127FA, //0xeaa127fa
			0xD4EF3085, //0xd4ef3085
			0x04881D05, //0x04881d05
			0xD9D4D039, //0xd9d4d039
			0xE6DB99E5, //0xe6db99e5
			0x1FA27CF8, //0x1fa27cf8
			0xC4AC5665  //0xc4ac5665
		},

		{
			0xF4292244, //0xf4292244
			0x432AFF97, //0x432aff97
			0xAB9423A7, //0xab9423a7
			0xFC93A039, //0xfc93a039
			0x655B59C3, //0x655b59c3
			0x8F0CCC92, //0x8f0ccc92
			0xFFEFF47D, //0xffeff47d
			0x85845DD1, //0x85845dd1
			0x6FA87E4F, //0x6fa87e4f
			0xFE2CE6E0, //0xfe2ce6e0
			0xA3014314, //0xa3014314
			0x4E0811A1, //0x4e0811a1
			0xF7537E82, //0xf7537e82
			0xBD3AF235, //0xbd3af235
			0x2AD7D2BB, //0x2ad7d2bb
			0xEB86D391  //0xeb86d391
		}
	};

	//初始化MD5拓展长度数组
	const uint8 CMD5::msk_ui8MD5PaddingArray[64] = { 0x80 };


	//CMD5实现：

	CMD5::CMD5(uint32 ui32MD5ChainingA, uint32 ui32MD5ChainingB, uint32 ui32MD5ChainingC, uint32 ui32MD5ChainingD)
	{
		m_ui32MD5ChainingA = ui32MD5ChainingA;
		m_ui32MD5ChainingB = ui32MD5ChainingB;
		m_ui32MD5ChainingC = ui32MD5ChainingC;
		m_ui32MD5ChainingD = ui32MD5ChainingD;

		this->Reset();
	}

	inline void CMD5::Reset()
	{
		m_bResetFlag = true;

		m_ui32State[0] = m_ui32MD5ChainingA;
		m_ui32State[1] = m_ui32MD5ChainingB;
		m_ui32State[2] = m_ui32MD5ChainingC;
		m_ui32State[3] = m_ui32MD5ChainingD;

		memset(m_ui32ByteCounts, 0, sizeof(uint32) * 2);

		memset(m_ui8InputBuffer, 0, sizeof(uint8) * 64);

		memset(m_szMD5, 0, sizeof(char) * 33);

		return;
	}

	inline char * CMD5::GetMD5(char * szMD5) const
	{
		strcpy(szMD5, m_szMD5);
		return szMD5;
	}

	void CMD5::CalcMD5(const void * pvDataStream, uint32 ui32DataBytes)
	{
		if (false == m_bResetFlag)
		{
			return;
		}
		else
		{
			m_bResetFlag = false;
		}

		this->Update((uint8 *)(pvDataStream), ui32DataBytes);
		this->Calculate();

		return;
	}

	void CMD5::CalcMD5(const char * szString)
	{
		if (false == m_bResetFlag)
		{
			return;
		}
		else
		{
			m_bResetFlag = false;
		}

		this->Update((uint8 *)(szString), strlen(szString));
		this->Calculate();

		return;
	}

	void CMD5::CalcMD5(const char * szFilePath, bool bIsBinary)
	{
		if (false == m_bResetFlag)
		{
			return;
		}
		else
		{
			m_bResetFlag = false;
		}

		std::ifstream fin(szFilePath, bIsBinary ? (std::ios::binary) : (std::ios_base::in));

		if (!fin)
		{
			return;
		}

		std::streamsize ReadBytes = 0;
		char cReadBuffer[1024] = { 0 };

		while (!fin.eof())
		{
			fin.read(cReadBuffer, 1024);
			ReadBytes = fin.gcount();
			if (ReadBytes > 0)
			{
				this->Update((uint8 *)cReadBuffer, ReadBytes);
				//streamsize-->uint32可能丢失数据，限制缓冲区大小为1024bytes
			}
		}

		fin.close();

		this->Calculate();

		return;
	}

	inline void CMD5::Uint32to8(uint8 * pui8Output, const uint32 * pui32Input, uint uiInputByteCounts) const
	{
		for (uint uiInputPos = 0, uiOutputPos = 0; uiOutputPos < uiInputByteCounts; uiInputPos++)
		{
			pui8Output[uiOutputPos++] = (uint8)((pui32Input[uiInputPos]) & 0xFF);
			pui8Output[uiOutputPos++] = (uint8)((pui32Input[uiInputPos] >> 8)  & 0xFF);
			pui8Output[uiOutputPos++] = (uint8)((pui32Input[uiInputPos] >> 16) & 0xFF);
			pui8Output[uiOutputPos++] = (uint8)((pui32Input[uiInputPos] >> 24) & 0xFF);
		}

		return;
	}

	inline void CMD5::Uint8to32(uint32 * pui32Output, const uint8 * pui8Input, uint uiInputByteCounts) const
	{
		for (uint uiInputPos = 0, uiOutputPos = 0; uiInputPos < uiInputByteCounts; uiOutputPos++)
		{
			pui32Output[uiOutputPos]  = (uint32)(pui8Input[uiInputPos++]);
			pui32Output[uiOutputPos] |= (uint32)(pui8Input[uiInputPos++]) << 8;
			pui32Output[uiOutputPos] |= (uint32)(pui8Input[uiInputPos++]) << 16;
			pui32Output[uiOutputPos] |= (uint32)(pui8Input[uiInputPos++]) << 24;
		}

		return;
	}

	inline uint32 CMD5::F(const uint32 x, const uint32 y, const uint32 z) const
	{
		return ((x & y) | ((~x) & z));
	}

	inline uint32 CMD5::G(const uint32 x, const uint32 y, const uint32 z) const
	{
		return ((x & z) | (y & (~z)));
	}

	inline uint32 CMD5::H(const uint32 x, const uint32 y, const uint32 z) const
	{
		return x^y^z;
	}

	inline uint32 CMD5::I(const uint32 x, const uint32 y, const uint32 z) const
	{
		return (y ^ (x | (~z)));
	}

	inline uint32 CMD5::RotateLeft(const uint32 x, const uint uiShiftCount) const
	{
		return ((x << uiShiftCount) | (x >> (32 - uiShiftCount)));
	}

	inline void CMD5::FF(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint s, uint32 ac)
	{
		a += F(b, c, d) + x + ac;
		a = RotateLeft(a, s);
		a += b;
	}

	inline void CMD5::GG(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint s, uint32 ac)
	{
		a += G(b, c, d) + x + ac;
		a = RotateLeft(a, s);
		a += b;
	}

	inline void CMD5::HH(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint s, uint32 ac)
	{
		a += H(b, c, d) + x + ac;
		a = RotateLeft(a, s);
		a += b;
	}

	inline void CMD5::II(uint32 & a, uint32 b, uint32 c, uint32 d, uint32 x, uint s, uint32 ac)
	{
		a += I(b, c, d) + x + ac;
		a = RotateLeft(a, s);
		a += b;
	}

	void CMD5::Transform(const uint8 ui8Block[64])
	{
		register uint32 a = m_ui32State[0];
		register uint32 b = m_ui32State[1];
		register uint32 c = m_ui32State[2];
		register uint32 d = m_ui32State[3];

		uint32 ui32Block[16];

		this->Uint8to32(ui32Block, ui8Block, 64);

		/* Round 1 */
		FF(a, b, c, d, ui32Block[ 0], msk_uiMD5TransformShiftMatrix[0][0], msk_ui32MD5TransformAddMatrix[0][ 0]); /*  1 */
		FF(d, a, b, c, ui32Block[ 1], msk_uiMD5TransformShiftMatrix[0][1], msk_ui32MD5TransformAddMatrix[0][ 1]); /*  2 */
		FF(c, d, a, b, ui32Block[ 2], msk_uiMD5TransformShiftMatrix[0][2], msk_ui32MD5TransformAddMatrix[0][ 2]); /*  3 */
		FF(b, c, d, a, ui32Block[ 3], msk_uiMD5TransformShiftMatrix[0][3], msk_ui32MD5TransformAddMatrix[0][ 3]); /*  4 */
		FF(a, b, c, d, ui32Block[ 4], msk_uiMD5TransformShiftMatrix[0][0], msk_ui32MD5TransformAddMatrix[0][ 4]); /*  5 */
		FF(d, a, b, c, ui32Block[ 5], msk_uiMD5TransformShiftMatrix[0][1], msk_ui32MD5TransformAddMatrix[0][ 5]); /*  6 */
		FF(c, d, a, b, ui32Block[ 6], msk_uiMD5TransformShiftMatrix[0][2], msk_ui32MD5TransformAddMatrix[0][ 6]); /*  7 */
		FF(b, c, d, a, ui32Block[ 7], msk_uiMD5TransformShiftMatrix[0][3], msk_ui32MD5TransformAddMatrix[0][ 7]); /*  8 */
		FF(a, b, c, d, ui32Block[ 8], msk_uiMD5TransformShiftMatrix[0][0], msk_ui32MD5TransformAddMatrix[0][ 8]); /*  9 */
		FF(d, a, b, c, ui32Block[ 9], msk_uiMD5TransformShiftMatrix[0][1], msk_ui32MD5TransformAddMatrix[0][ 9]); /* 10 */
		FF(c, d, a, b, ui32Block[10], msk_uiMD5TransformShiftMatrix[0][2], msk_ui32MD5TransformAddMatrix[0][10]); /* 11 */
		FF(b, c, d, a, ui32Block[11], msk_uiMD5TransformShiftMatrix[0][3], msk_ui32MD5TransformAddMatrix[0][11]); /* 12 */
		FF(a, b, c, d, ui32Block[12], msk_uiMD5TransformShiftMatrix[0][0], msk_ui32MD5TransformAddMatrix[0][12]); /* 13 */
		FF(d, a, b, c, ui32Block[13], msk_uiMD5TransformShiftMatrix[0][1], msk_ui32MD5TransformAddMatrix[0][13]); /* 14 */
		FF(c, d, a, b, ui32Block[14], msk_uiMD5TransformShiftMatrix[0][2], msk_ui32MD5TransformAddMatrix[0][14]); /* 15 */
		FF(b, c, d, a, ui32Block[15], msk_uiMD5TransformShiftMatrix[0][3], msk_ui32MD5TransformAddMatrix[0][15]); /* 16 */

		/* Round 2 */
		GG(a, b, c, d, ui32Block[ 1], msk_uiMD5TransformShiftMatrix[1][0], msk_ui32MD5TransformAddMatrix[1][ 0]); /* 17 */
		GG(d, a, b, c, ui32Block[ 6], msk_uiMD5TransformShiftMatrix[1][1], msk_ui32MD5TransformAddMatrix[1][ 1]); /* 18 */
		GG(c, d, a, b, ui32Block[11], msk_uiMD5TransformShiftMatrix[1][2], msk_ui32MD5TransformAddMatrix[1][ 2]); /* 19 */
		GG(b, c, d, a, ui32Block[ 0], msk_uiMD5TransformShiftMatrix[1][3], msk_ui32MD5TransformAddMatrix[1][ 3]); /* 20 */
		GG(a, b, c, d, ui32Block[ 5], msk_uiMD5TransformShiftMatrix[1][0], msk_ui32MD5TransformAddMatrix[1][ 4]); /* 21 */
		GG(d, a, b, c, ui32Block[10], msk_uiMD5TransformShiftMatrix[1][1], msk_ui32MD5TransformAddMatrix[1][ 5]); /* 22 */
		GG(c, d, a, b, ui32Block[15], msk_uiMD5TransformShiftMatrix[1][2], msk_ui32MD5TransformAddMatrix[1][ 6]); /* 23 */
		GG(b, c, d, a, ui32Block[ 4], msk_uiMD5TransformShiftMatrix[1][3], msk_ui32MD5TransformAddMatrix[1][ 7]); /* 24 */
		GG(a, b, c, d, ui32Block[ 9], msk_uiMD5TransformShiftMatrix[1][0], msk_ui32MD5TransformAddMatrix[1][ 8]); /* 25 */
		GG(d, a, b, c, ui32Block[14], msk_uiMD5TransformShiftMatrix[1][1], msk_ui32MD5TransformAddMatrix[1][ 9]); /* 26 */
		GG(c, d, a, b, ui32Block[ 3], msk_uiMD5TransformShiftMatrix[1][2], msk_ui32MD5TransformAddMatrix[1][10]); /* 27 */
		GG(b, c, d, a, ui32Block[ 8], msk_uiMD5TransformShiftMatrix[1][3], msk_ui32MD5TransformAddMatrix[1][11]); /* 28 */
		GG(a, b, c, d, ui32Block[13], msk_uiMD5TransformShiftMatrix[1][0], msk_ui32MD5TransformAddMatrix[1][12]); /* 29 */
		GG(d, a, b, c, ui32Block[ 2], msk_uiMD5TransformShiftMatrix[1][1], msk_ui32MD5TransformAddMatrix[1][13]); /* 30 */
		GG(c, d, a, b, ui32Block[ 7], msk_uiMD5TransformShiftMatrix[1][2], msk_ui32MD5TransformAddMatrix[1][14]); /* 31 */
		GG(b, c, d, a, ui32Block[12], msk_uiMD5TransformShiftMatrix[1][3], msk_ui32MD5TransformAddMatrix[1][15]); /* 32 */

		/* Round 3 */
		HH(a, b, c, d, ui32Block[ 5], msk_uiMD5TransformShiftMatrix[2][0], msk_ui32MD5TransformAddMatrix[2][ 0]); /* 33 */
		HH(d, a, b, c, ui32Block[ 8], msk_uiMD5TransformShiftMatrix[2][1], msk_ui32MD5TransformAddMatrix[2][ 1]); /* 34 */
		HH(c, d, a, b, ui32Block[11], msk_uiMD5TransformShiftMatrix[2][2], msk_ui32MD5TransformAddMatrix[2][ 2]); /* 35 */
		HH(b, c, d, a, ui32Block[14], msk_uiMD5TransformShiftMatrix[2][3], msk_ui32MD5TransformAddMatrix[2][ 3]); /* 36 */
		HH(a, b, c, d, ui32Block[ 1], msk_uiMD5TransformShiftMatrix[2][0], msk_ui32MD5TransformAddMatrix[2][ 4]); /* 37 */
		HH(d, a, b, c, ui32Block[ 4], msk_uiMD5TransformShiftMatrix[2][1], msk_ui32MD5TransformAddMatrix[2][ 5]); /* 38 */
		HH(c, d, a, b, ui32Block[ 7], msk_uiMD5TransformShiftMatrix[2][2], msk_ui32MD5TransformAddMatrix[2][ 6]); /* 39 */
		HH(b, c, d, a, ui32Block[10], msk_uiMD5TransformShiftMatrix[2][3], msk_ui32MD5TransformAddMatrix[2][ 7]); /* 40 */
		HH(a, b, c, d, ui32Block[13], msk_uiMD5TransformShiftMatrix[2][0], msk_ui32MD5TransformAddMatrix[2][ 8]); /* 41 */
		HH(d, a, b, c, ui32Block[ 0], msk_uiMD5TransformShiftMatrix[2][1], msk_ui32MD5TransformAddMatrix[2][ 9]); /* 42 */
		HH(c, d, a, b, ui32Block[ 3], msk_uiMD5TransformShiftMatrix[2][2], msk_ui32MD5TransformAddMatrix[2][10]); /* 43 */
		HH(b, c, d, a, ui32Block[ 6], msk_uiMD5TransformShiftMatrix[2][3], msk_ui32MD5TransformAddMatrix[2][11]); /* 44 */
		HH(a, b, c, d, ui32Block[ 9], msk_uiMD5TransformShiftMatrix[2][0], msk_ui32MD5TransformAddMatrix[2][12]); /* 45 */
		HH(d, a, b, c, ui32Block[12], msk_uiMD5TransformShiftMatrix[2][1], msk_ui32MD5TransformAddMatrix[2][13]); /* 46 */
		HH(c, d, a, b, ui32Block[15], msk_uiMD5TransformShiftMatrix[2][2], msk_ui32MD5TransformAddMatrix[2][14]); /* 47 */
		HH(b, c, d, a, ui32Block[ 2], msk_uiMD5TransformShiftMatrix[2][3], msk_ui32MD5TransformAddMatrix[2][15]); /* 48 */

		/* Round 4 */
		II(a, b, c, d, ui32Block[ 0], msk_uiMD5TransformShiftMatrix[3][0], msk_ui32MD5TransformAddMatrix[3][ 0]); /* 49 */
		II(d, a, b, c, ui32Block[ 7], msk_uiMD5TransformShiftMatrix[3][1], msk_ui32MD5TransformAddMatrix[3][ 1]); /* 50 */
		II(c, d, a, b, ui32Block[14], msk_uiMD5TransformShiftMatrix[3][2], msk_ui32MD5TransformAddMatrix[3][ 2]); /* 51 */
		II(b, c, d, a, ui32Block[ 5], msk_uiMD5TransformShiftMatrix[3][3], msk_ui32MD5TransformAddMatrix[3][ 3]); /* 52 */
		II(a, b, c, d, ui32Block[12], msk_uiMD5TransformShiftMatrix[3][0], msk_ui32MD5TransformAddMatrix[3][ 4]); /* 53 */
		II(d, a, b, c, ui32Block[ 3], msk_uiMD5TransformShiftMatrix[3][1], msk_ui32MD5TransformAddMatrix[3][ 5]); /* 54 */
		II(c, d, a, b, ui32Block[10], msk_uiMD5TransformShiftMatrix[3][2], msk_ui32MD5TransformAddMatrix[3][ 6]); /* 55 */
		II(b, c, d, a, ui32Block[ 1], msk_uiMD5TransformShiftMatrix[3][3], msk_ui32MD5TransformAddMatrix[3][ 7]); /* 56 */
		II(a, b, c, d, ui32Block[ 8], msk_uiMD5TransformShiftMatrix[3][0], msk_ui32MD5TransformAddMatrix[3][ 8]); /* 57 */
		II(d, a, b, c, ui32Block[15], msk_uiMD5TransformShiftMatrix[3][1], msk_ui32MD5TransformAddMatrix[3][ 9]); /* 58 */
		II(c, d, a, b, ui32Block[ 6], msk_uiMD5TransformShiftMatrix[3][2], msk_ui32MD5TransformAddMatrix[3][10]); /* 59 */
		II(b, c, d, a, ui32Block[13], msk_uiMD5TransformShiftMatrix[3][3], msk_ui32MD5TransformAddMatrix[3][11]); /* 60 */
		II(a, b, c, d, ui32Block[ 4], msk_uiMD5TransformShiftMatrix[3][0], msk_ui32MD5TransformAddMatrix[3][12]); /* 61 */
		II(d, a, b, c, ui32Block[11], msk_uiMD5TransformShiftMatrix[3][1], msk_ui32MD5TransformAddMatrix[3][13]); /* 62 */
		II(c, d, a, b, ui32Block[ 2], msk_uiMD5TransformShiftMatrix[3][2], msk_ui32MD5TransformAddMatrix[3][14]); /* 63 */
		II(b, c, d, a, ui32Block[ 9], msk_uiMD5TransformShiftMatrix[3][3], msk_ui32MD5TransformAddMatrix[3][15]); /* 64 */

		m_ui32State[0] += a;
		m_ui32State[1] += b;
		m_ui32State[2] += c;
		m_ui32State[3] += d;

		return;
	}

	void CMD5::Update(const uint8 * pui8Input, uint32 ui32InputBytes)
	{
		//计算m_ui32ByteCounts[0]对64的余数，即：uiPos = m_ui32ByteCounts[0] mod 64
		uint uiMod = (uint)((m_ui32ByteCounts[0] >> 3) & 0x3F); /* (0x3F)hex = (63)dec = (0011,1111)bin */

		//更新字节数
		if ((m_ui32ByteCounts[0] += (ui32InputBytes << 3))  <  (ui32InputBytes << 3))
		{
			m_ui32ByteCounts[1]++;
		}
		m_ui32ByteCounts[1] += (ui32InputBytes >> 29);

		//尽可能多的进行MD5变换
		uint32 ui32 = 0;
		uint   uiPartBytes = 64 - uiMod;
		if (ui32InputBytes >= uiPartBytes)
		{
			memcpy(&m_ui8InputBuffer[uiMod], pui8Input, uiPartBytes);
			this->Transform(m_ui8InputBuffer);
			for (ui32 = uiPartBytes; (ui32 + 63) < ui32InputBytes; ui32 += 64)
			{
				this->Transform(&pui8Input[ui32]);
			}
		}

		//输入区内剩余的数据
		memcpy(&m_ui8InputBuffer[uiMod], &pui8Input[ui32], ui32InputBytes - ui32);

		return;
	}

	void CMD5::Calculate()
	{
		//整个数据流的字节数
		uint8 ui8TotalBytes[8] = { 0 };

		//转换uint类型
		this->Uint32to8(ui8TotalBytes, m_ui32ByteCounts, 8); //2 * sizeof(uint32)

		//拓展字节到 56 mod 64
		uint uiMod = (uint)((m_ui32ByteCounts[0] >> 3) & 0x3f);
		uint uiPad = (uiMod < 56) ? (56 - uiMod) : (120 - uiMod);
		this->Update(msk_ui8MD5PaddingArray, uiPad);

		//附加数据流的字节大小的数据
		this->Update(ui8TotalBytes, 8);

		//处理MD5字符串
		uint8 ui8MD5Str[16] = { 0 };
		this->Uint32to8(ui8MD5Str, m_ui32State, 16); //4 * sizeof(uint32)
		for (uint ui = 0; ui < 16; ui++)
		{
			if (ui8MD5Str[ui] == 0)
			{
				strcat(m_szMD5, "00");
			}
			else if (ui8MD5Str[ui] <= 15)
			{
				sprintf(m_szMD5, "%s0%X", m_szMD5, ui8MD5Str[ui]);
			}
			else
			{
				sprintf(m_szMD5, "%s%X", m_szMD5, ui8MD5Str[ui]);
			}
		}

		return;
	}

}