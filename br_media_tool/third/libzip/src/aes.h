// Aes.h: interface for the CAes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ANYCHAT_AES_INCLUDE_H_)
#define _ANYCHAT_AES_INCLUDE_H_

#include "osdef.h"

#pragma once




class CAes
{
public:
	CAes();
	virtual ~CAes();

	// 密钥长度
	enum {
		Bits128 = 16,
		Bits192 = 24,
	    Bits256 = 32
	};  // key size, in bits, for construtor


	CAes(INT keySize, BYTE* keyBytes);
	//转换16进制字符串到整数
	static INT htoi(CHAR s[3]);

	BYTE State[4][4];  // State matrix
	VOID Cipher(BYTE* input, BYTE* output);  // encipher 16-bit input
	VOID InvCipher(BYTE * input, BYTE * output);  // decipher 16-bit input

private:
	INT Nb;         // block size in 32-bit words.  Always 4 for AES.  (128 bits).
	INT Nk;         // key size in 32-bit words.  4, 6, 8.  (128, 192, 256 bits).
	INT Nr;         // number of rounds. 10, 12, 14.

	BYTE key[32];     // the seed key. size will be 4 * keySize from ctor.
	//char* Sbox;   // Substitution box
	//char* iSbox;  // inverse Substitution box
	BYTE w[16*15];      // key schedule array. (Nb*(Nr+1))*4
	//char* Rcon;   // Round constants.
	VOID SetNbNkNr(INT keyS);
	VOID AddRoundKey(INT round);
	VOID SubBytes();
	VOID InvSubBytes();
	VOID ShiftRows();
	VOID InvShiftRows();
	VOID MixColumns();
	VOID InvMixColumns();
	BYTE gfmultby01(BYTE b);
	BYTE gfmultby02(BYTE b);
	BYTE gfmultby03(BYTE b);
	BYTE gfmultby09(BYTE b);
	BYTE gfmultby0b(BYTE b);
	BYTE gfmultby0d(BYTE b);
	BYTE gfmultby0e(BYTE b);
	VOID KeyExpansion();
	VOID SubWord(BYTE * word,BYTE* result);
	VOID RotWord(BYTE * word,BYTE* result);

};

#endif // !defined(_ANYCHAT_AES_INCLUDE_H_)
