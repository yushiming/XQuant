//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  KeyCodes.h
//
//   Description:  ��������
//
//       Version:  1.0
//       Created:  2023��1��2�� 13ʱ12��22��
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================


#pragma once

namespace XQuant
{
	using KeyCode = uint16_t;

	namespace Key
	{
		enum : KeyCode
		{
			// From glfw3.h
			eSpace = 32,
			eApostrophe = 39, /* ' */
			eComma = 44, /* , */
			eMinus = 45, /* - */
			ePeriod = 46, /* . */
			eSlash = 47, /* / */

			eD0 = 48, /* 0 */
			eD1 = 49, /* 1 */
			eD2 = 50, /* 2 */
			eD3 = 51, /* 3 */
			eD4 = 52, /* 4 */
			eD5 = 53, /* 5 */
			eD6 = 54, /* 6 */
			eD7 = 55, /* 7 */
			eD8 = 56, /* 8 */
			eD9 = 57, /* 9 */

			eSemicolon = 59, /* ; */
			eEqual = 61, /* = */

			eA = 65,
			eB = 66,
			eC = 67,
			eD = 68,
			eE = 69,
			eF = 70,
			eG = 71,
			eH = 72,
			eI = 73,
			eJ = 74,
			eK = 75,
			eL = 76,
			eM = 77,
			eN = 78,
			eO = 79,
			eP = 80,
			eQ = 81,
			eR = 82,
			eS = 83,
			eT = 84,
			eU = 85,
			eV = 86,
			eW = 87,
			eX = 88,
			eY = 89,
			eZ = 90,

			eLeftBracket = 91,  /* [ */
			eBackslash = 92,  /* \ */
			eRightBracket = 93,  /* ] */
			eGraveAccent = 96,  /* ` */

			eWorld1 = 161, /* non-US #1 */
			eWorld2 = 162, /* non-US #2 */

			/* Function keys */
			eEscape = 256,
			eEnter = 257,
			eTab = 258,
			eBackspace = 259,
			eInsert = 260,
			eDelete = 261,
			eRight = 262,
			eLeft = 263,
			eDown = 264,
			eUp = 265,
			ePageUp = 266,
			ePageDown = 267,
			eHome = 268,
			eEnd = 269,
			eCapsLock = 280,
			eScrollLock = 281,
			eNumLock = 282,
			ePrintScreen = 283,
			ePause = 284,
			eF1 = 290,
			eF2 = 291,
			eF3 = 292,
			eF4 = 293,
			eF5 = 294,
			eF6 = 295,
			eF7 = 296,
			eF8 = 297,
			eF9 = 298,
			eF10 = 299,
			eF11 = 300,
			eF12 = 301,
			eF13 = 302,
			eF14 = 303,
			eF15 = 304,
			eF16 = 305,
			eF17 = 306,
			eF18 = 307,
			eF19 = 308,
			eF20 = 309,
			eF21 = 310,
			eF22 = 311,
			eF23 = 312,
			eF24 = 313,
			eF25 = 314,

			/* Keypad */
			eKP0 = 320,
			eKP1 = 321,
			eKP2 = 322,
			eKP3 = 323,
			eKP4 = 324,
			eKP5 = 325,
			eKP6 = 326,
			eKP7 = 327,
			eKP8 = 328,
			eKP9 = 329,
			eKPDecimal = 330,
			eKPDivide = 331,
			eKPMultiply = 332,
			eKPSubtract = 333,
			eKPAdd = 334,
			eKPEnter = 335,
			eKPEqual = 336,

			eLeftShift = 340,
			eLeftControl = 341,
			eLeftAlt = 342,
			eLeftSuper = 343,
			eRightShift = 344,
			eRightControl = 345,
			eRightAlt = 346,
			eRightSuper = 347,
			eMenu = 348
		};
	}
}