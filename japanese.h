#ifndef _JAPANESE_H
#define _JAPANESE_H

//COPYRIGHT AND PERMISSION NOTICE
//
//Copyright (c) 1999,2000,2001 Brian Bennewitz
//
//All rights reserved.
//
//Permission is hereby granted, free of charge, to any person obtaining a
//copy of this software and associated documentation files (the
//"Software"), to deal in the Software without restriction, including
//without limitation the rights to use, copy, modify, merge, publish,
//distribute, and/or sell copies of the Software, and to permit persons
//to whom the Software is furnished to do so, provided that the above
//copyright notice(s) and this permission notice appear in all copies of
//the Software and that both the above copyright notice(s) and this
//permission notice appear in supporting documentation.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
//OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
//HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
//INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
//FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
//NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
//WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
//Except as contained in this notice, the name of a copyright holder
//shall not be used in advertising or otherwise to promote the sale, use
//or other dealings in this Software without prior written authorization
//of the copyright holder.

vector<string> vEucEnc(352);
vector<string> vShiftEnc(352);
vector<string> vRomajiEnc(352);

//33312
int jpfont_shift(long db)
{
	if(db == 33088)
		return 32;
	if(db == 33097)
		return 33;
	if(db == 33102)
		return 34;
	if(db == 33172)
		return 35;
	if(db == 33167)
		return 36;
	if(db == 33171)
		return 37;
	if(db == 33173)
		return 38;
	if(db == 33098)
		return 39;
	if(db == 33129)
		return 40;
	if(db == 33130)
		return 41;
	if(db == 33174)
		return 42;
	if(db == 33147)
		return 43;
	if(db == 33091)
		return 44;
	if(db == 33092)
		return 46;
	if(db == 33090)
		return 46;
	if(db == 33118)
		return 47;
	if(db == 33359)
		return 48;
	if(db == 33360)
		return 49;
	if(db == 33361)
		return 50;
	if(db == 33362)
		return 51;
	if(db == 33363)
		return 52;
	if(db == 33364)
		return 53;
	if(db == 33365)
		return 54;
	if(db == 33366)
		return 55;
	if(db == 33367)
		return 56;
	if(db == 33368)
		return 57;
	if(db == 33099)
		return 58;
	if(db == 33106)
		return 59;
	if(db == 33155)
		return 60;
	if(db == 33153)
		return 61;
	if(db == 33156)
		return 62;
	if(db == 33096)
		return 63;
	if(db == 33175)
		return 64;
	if(db == 33376)
		return 65;
	if(db == 33377)
		return 66;
	if(db == 33378)
		return 67;
	if(db == 33379)
		return 68;
	if(db == 33380)
		return 69;
	if(db == 33381)
		return 70;
	if(db == 33382)
		return 71;
	if(db == 33383)
		return 72;
	if(db == 33384)
		return 73;
	if(db == 33385)
		return 74;
	if(db == 33386)
		return 75;
	if(db == 33387)
		return 76;
	if(db == 33388)
		return 77;
	if(db == 33389)
		return 78;
	if(db == 33390)
		return 79;
	if(db == 33391)
		return 80;
	if(db == 33392)
		return 81;
	if(db == 33393)
		return 82;
	if(db == 33394)
		return 83;
	if(db == 33395)
		return 84;
	if(db == 33396)
		return 85;
	if(db == 33397)
		return 86;
	if(db == 33398)
		return 87;
	if(db == 33399)
		return 88;
	if(db == 33400)
		return 89;
	if(db == 33401)
		return 90;
	if(db == 33141)
		return 91;
	if(db == 33119)
		return 92;
	if(db == 33142)
		return 93;
	if(db == 33103)
		return 94;
	if(db == 33105)
		return 95;
	if(db == 33409)
		return 97;
	if(db == 33410)
		return 98;
	if(db == 33411)
		return 99;
	if(db == 33412)
		return 100;
	if(db == 33413)
		return 101;
	if(db == 33414)
		return 102;
	if(db == 33415)
		return 103;
	if(db == 33416)
		return 104;
	if(db == 33417)
		return 105;
	if(db == 33418)
		return 106;
	if(db == 33419)
		return 107;
	if(db == 33420)
		return 108;
	if(db == 33421)
		return 109;
	if(db == 33422)
		return 110;
	if(db == 33423)
		return 111;
	if(db == 33424)
		return 112;
	if(db == 33425)
		return 113;
	if(db == 33426)
		return 114;
	if(db == 33427)
		return 115;
	if(db == 33428)
		return 116;
	if(db == 33429)
		return 117;
	if(db == 33430)
		return 118;
	if(db == 33431)
		return 119;
	if(db == 33432)
		return 120;
	if(db == 33433)
		return 121;
	if(db == 33434)
		return 122;
	if(db == 33135)
		return 123;
	if(db == 33116)
		return 124;
	if(db == 33136)
		return 125;
	if(db == 33120)
		return 126;
	if(db == 33439)
		return 161;
	if(db == 33440)
		return 161;
	if(db == 33441)
		return 162;
	if(db == 33442)
		return 162;
	if(db == 33443)
		return 163;
	if(db == 33444)
		return 163;
	if(db == 33445)
		return 164;
	if(db == 33446)
		return 164;
	if(db == 33447)
		return 165;
	if(db == 33448)
		return 165;
	if(db == 33449)
		return 166;
	if(db == 33450)
		return 166;
	if(db == 33451)
		return 167;
	if(db == 33452)
		return 167;
	if(db == 33453)
		return 168;
	if(db == 33454)
		return 168;
	if(db == 33455)
		return 169;
	if(db == 33456)
		return 169;
	if(db == 33457)
		return 170;
	if(db == 33458)
		return 170;
	if(db == 33459)
		return 171;
	if(db == 33460)
		return 171;
	if(db == 33461)
		return 172;
	if(db == 33462)
		return 172;
	if(db == 33463)
		return 174;
	if(db == 33464)
		return 174;
	if(db == 33465)
		return 176;
	if(db == 33466)
		return 176;
	if(db == 33467)
		return 177;
	if(db == 33468)
		return 177;
	if(db == 33469)
		return 178;
	if(db == 33470)
		return 178;
	if(db == 33471)
		return 179;
	if(db == 33472)
		return 179;
	if(db == 33473)
		return 180;
	if(db == 33474)
		return 180;
	if(db == 33475)
		return 180;
	if(db == 33476)
		return 182;
	if(db == 33477)
		return 182;
	if(db == 33478)
		return 184;
	if(db == 33479)
		return 184;
	if(db == 33480)
		return 185;
	if(db == 33481)
		return 186;
	if(db == 33482)
		return 187;
	if(db == 33483)
		return 187;
	if(db == 33484)
		return 188;
	if(db == 33485)
		return 189;
	if(db == 33486)
		return 189;
	if(db == 33487)
		return 189;
	if(db == 33488)
		return 190;
	if(db == 33489)
		return 190;
	if(db == 33490)
		return 190;
	if(db == 33491)
		return 191;
	if(db == 33492)
		return 191;
	if(db == 33493)
		return 191;
	if(db == 33494)
		return 192;
	if(db == 33495)
		return 192;
	if(db == 33496)
		return 192;
	if(db == 33497)
		return 193;
	if(db == 33498)
		return 193;
	if(db == 33499)
		return 193;
	if(db == 33500)
		return 194;
	if(db == 33501)
		return 195;
	if(db == 33502)
		return 196;
	if(db == 33503)
		return 197;
	if(db == 33504)
		return 198;
	if(db == 33505)
		return 199;
	if(db == 33506)
		return 199;
	if(db == 33507)
		return 200;
	if(db == 33508)
		return 200;
	if(db == 33509)
		return 201;
	if(db == 33510)
		return 201;
	if(db == 33511)
		return 202;
	if(db == 33512)
		return 203;
	if(db == 33513)
		return 204;
	if(db == 33514)
		return 205;
	if(db == 33515)
		return 206;
	if(db == 33516)
		return 207;
	if(db == 33517)
		return 207;
	if(db == 33520)
		return 208;
	if(db == 33521)
		return 209;
	if(db == 33600)
		return 210;
	if(db == 33601)
		return 210;
	if(db == 33602)
		return 211;
	if(db == 33603)
		return 211;
	if(db == 33604)
		return 212;
	if(db == 33605)
		return 212;
	if(db == 33606)
		return 213;
	if(db == 33607)
		return 213;
	if(db == 33608)
		return 214;
	if(db == 33609)
		return 214;
	if(db == 33610)
		return 215;
	if(db == 33611)
		return 215;
	if(db == 33612)
		return 216;
	if(db == 33613)
		return 216;
	if(db == 33614)
		return 217;
	if(db == 33615)
		return 217;
	if(db == 33616)
		return 218;
	if(db == 33617)
		return 218;
	if(db == 33618)
		return 219;
	if(db == 33619)
		return 219;
	if(db == 33620)
		return 220;
	if(db == 33621)
		return 220;
	if(db == 33622)
		return 221;
	if(db == 33623)
		return 221;
	if(db == 33624)
		return 222;
	if(db == 33625)
		return 222;
	if(db == 33626)
		return 223;
	if(db == 33627)
		return 223;
	if(db == 33628)
		return 224;
	if(db == 33629)
		return 224;
	if(db == 33630)
		return 225;
	if(db == 33631)
		return 225;
	if(db == 33632)
		return 226;
	if(db == 33633)
		return 226;
	if(db == 33634)
		return 227;
	if(db == 33635)
		return 227;
	if(db == 33636)
		return 227;
	if(db == 33637)
		return 228;
	if(db == 33638)
		return 228;
	if(db == 33639)
		return 229;
	if(db == 33640)
		return 229;
	if(db == 33641)
		return 230;
	if(db == 33642)
		return 231;
	if(db == 33643)
		return 232;
	if(db == 33644)
		return 233;
	if(db == 33645)
		return 234;
	if(db == 33646)
		return 235;
	if(db == 33647)
		return 235;
	if(db == 33648)
		return 235;
	if(db == 33649)
		return 236;
	if(db == 33650)
		return 236;
	if(db == 33651)
		return 236;
	if(db == 33652)
		return 237;
	if(db == 33653)
		return 237;
	if(db == 33654)
		return 237;
	if(db == 33655)
		return 238;
	if(db == 33656)
		return 238;
	if(db == 33657)
		return 238;
	if(db == 33658)
		return 239;
	if(db == 33659)
		return 239;
	if(db == 33660)
		return 239;
	if(db == 33661)
		return 240;
	if(db == 33662)
		return 241;
	if(db == 33664)
		return 242;
	if(db == 33665)
		return 243;
	if(db == 33666)
		return 244;
	if(db == 33667)
		return 245;
	if(db == 33668)
		return 245;
	if(db == 33669)
		return 246;
	if(db == 33670)
		return 246;
	if(db == 33671)
		return 247;
	if(db == 33672)
		return 247;
	if(db == 33673)
		return 248;
	if(db == 33674)
		return 249;
	if(db == 33675)
		return 250;
	if(db == 33676)
		return 251;
	if(db == 33677)
		return 252;
	if(db == 33678)
		return 253;
	if(db == 33679)
		return 254;
	if(db == 33683)
		return 255;
	if(db <= 255)
		return  db;
	else
		return  -1;
}

int jpfont_euc(long db)
{
	if(db == 41377)
		return 32;
	if(db == 41386)
		return 33;
	if(db == 41391)
		return 34;
	if(db == 41460)
		return 35;
	if(db == 41455)
		return 36;
	if(db == 41459)
		return 37;
	if(db == 41461)
		return 38;
	if(db == 41387)
		return 39;
	if(db == 41418)
		return 40;
	if(db == 41419)
		return 41;
	if(db == 41462)
		return 42;
	if(db == 41436)
		return 43;
	if(db == 41380)
		return 44;
	if(db == 41381)
		return 46;
	if(db == 41379)
		return 46;
	if(db == 41407)
		return 47;
	if(db == 41904)
		return 48;
	if(db == 41905)
		return 49;
	if(db == 41906)
		return 50;
	if(db == 41907)
		return 51;
	if(db == 41908)
		return 52;
	if(db == 41909)
		return 53;
	if(db == 41910)
		return 54;
	if(db == 41911)
		return 55;
	if(db == 41912)
		return 56;
	if(db == 41913)
		return 57;
	if(db == 41388)
		return 58;
	if(db == 41395)
		return 59;
	if(db == 41443)
		return 60;
	if(db == 41441)
		return 61;
	if(db == 41444)
		return 62;
	if(db == 41385)
		return 63;
	if(db == 41463)
		return 64;
	if(db == 41921)
		return 65;
	if(db == 41922)
		return 66;
	if(db == 41923)
		return 67;
	if(db == 41924)
		return 68;
	if(db == 41925)
		return 69;
	if(db == 41926)
		return 70;
	if(db == 41927)
		return 71;
	if(db == 41928)
		return 72;
	if(db == 41929)
		return 73;
	if(db == 41930)
		return 74;
	if(db == 41931)
		return 75;
	if(db == 41932)
		return 76;
	if(db == 41933)
		return 77;
	if(db == 41934)
		return 78;
	if(db == 41935)
		return 79;
	if(db == 41936)
		return 80;
	if(db == 41937)
		return 81;
	if(db == 41938)
		return 82;
	if(db == 41939)
		return 83;
	if(db == 41940)
		return 84;
	if(db == 41941)
		return 85;
	if(db == 41942)
		return 86;
	if(db == 41943)
		return 87;
	if(db == 41944)
		return 88;
	if(db == 41945)
		return 89;
	if(db == 41946)
		return 90;
	if(db == 41430)
		return 91;
	if(db == 41408)
		return 92;
	if(db == 41431)
		return 93;
	if(db == 41392)
		return 94;
	if(db == 41394)
		return 95;
	if(db == 41953)
		return 97;
	if(db == 41954)
		return 98;
	if(db == 41955)
		return 99;
	if(db == 41956)
		return 100;
	if(db == 41957)
		return 101;
	if(db == 41958)
		return 102;
	if(db == 41959)
		return 103;
	if(db == 41960)
		return 104;
	if(db == 41961)
		return 105;
	if(db == 41962)
		return 106;
	if(db == 41963)
		return 107;
	if(db == 41964)
		return 108;
	if(db == 41965)
		return 109;
	if(db == 41966)
		return 110;
	if(db == 41967)
		return 111;
	if(db == 41968)
		return 112;
	if(db == 41969)
		return 113;
	if(db == 41970)
		return 114;
	if(db == 41971)
		return 115;
	if(db == 41972)
		return 116;
	if(db == 41973)
		return 117;
	if(db == 41974)
		return 118;
	if(db == 41975)
		return 119;
	if(db == 41976)
		return 120;
	if(db == 41977)
		return 121;
	if(db == 41978)
		return 122;
	if(db == 41424)
		return 123;
	if(db == 41405)
		return 124;
	if(db == 41425)
		return 125;
	if(db == 41409)
		return 126;
	if(db == 42145)
		return 161;
	if(db == 42146)
		return 161;
	if(db == 42147)
		return 162;
	if(db == 42148)
		return 162;
	if(db == 42149)
		return 163;
	if(db == 42150)
		return 163;
	if(db == 42151)
		return 164;
	if(db == 42152)
		return 164;
	if(db == 42153)
		return 165;
	if(db == 42154)
		return 165;
	if(db == 42155)
		return 166;
	if(db == 42156)
		return 166;
	if(db == 42157)
		return 167;
	if(db == 42158)
		return 167;
	if(db == 42159)
		return 168;
	if(db == 42160)
		return 168;
	if(db == 42161)
		return 169;
	if(db == 42162)
		return 169;
	if(db == 42163)
		return 170;
	if(db == 42164)
		return 170;
	if(db == 42165)
		return 171;
	if(db == 42166)
		return 171;
	if(db == 42167)
		return 172;
	if(db == 42168)
		return 172;
	if(db == 42169)
		return 174;
	if(db == 42170)
		return 174;
	if(db == 42171)
		return 176;
	if(db == 42172)
		return 176;
	if(db == 42173)
		return 177;
	if(db == 42174)
		return 177;
	if(db == 42175)
		return 178;
	if(db == 42176)
		return 178;
	if(db == 42177)
		return 179;
	if(db == 42178)
		return 179;
	if(db == 42179)
		return 180;
	if(db == 42180)
		return 180;
	if(db == 42181)
		return 180;
	if(db == 42182)
		return 182;
	if(db == 42183)
		return 182;
	if(db == 42184)
		return 184;
	if(db == 42185)
		return 184;
	if(db == 42186)
		return 185;
	if(db == 42187)
		return 186;
	if(db == 42188)
		return 187;
	if(db == 42189)
		return 187;
	if(db == 42190)
		return 188;
	if(db == 42191)
		return 189;
	if(db == 42192)
		return 189;
	if(db == 42193)
		return 189;
	if(db == 42194)
		return 190;
	if(db == 42195)
		return 190;
	if(db == 42196)
		return 190;
	if(db == 42197)
		return 191;
	if(db == 42198)
		return 191;
	if(db == 42199)
		return 191;
	if(db == 42200)
		return 192;
	if(db == 42201)
		return 192;
	if(db == 42202)
		return 192;
	if(db == 42203)
		return 193;
	if(db == 42204)
		return 193;
	if(db == 42205)
		return 193;
	if(db == 42206)
		return 194;
	if(db == 42207)
		return 195;
	if(db == 42208)
		return 196;
	if(db == 42209)
		return 197;
	if(db == 42210)
		return 198;
	if(db == 42211)
		return 199;
	if(db == 42212)
		return 199;
	if(db == 42213)
		return 200;
	if(db == 42214)
		return 200;
	if(db == 42215)
		return 201;
	if(db == 42216)
		return 201;
	if(db == 42217)
		return 202;
	if(db == 42218)
		return 203;
	if(db == 42219)
		return 204;
	if(db == 42220)
		return 205;
	if(db == 42221)
		return 206;
	if(db == 42222)
		return 207;
	if(db == 42223)
		return 207;
	if(db == 42226)
		return 208;
	if(db == 42227)
		return 209;
	if(db == 42401)
		return 210;
	if(db == 42402)
		return 210;
	if(db == 42403)
		return 211;
	if(db == 42404)
		return 211;
	if(db == 42405)
		return 212;
	if(db == 42406)
		return 212;
	if(db == 42407)
		return 213;
	if(db == 42408)
		return 213;
	if(db == 42409)
		return 214;
	if(db == 42410)
		return 214;
	if(db == 42411)
		return 215;
	if(db == 42412)
		return 215;
	if(db == 42413)
		return 216;
	if(db == 42414)
		return 216;
	if(db == 42415)
		return 217;
	if(db == 42416)
		return 217;
	if(db == 42417)
		return 218;
	if(db == 42418)
		return 218;
	if(db == 42419)
		return 219;
	if(db == 42420)
		return 219;
	if(db == 42421)
		return 220;
	if(db == 42422)
		return 220;
	if(db == 42423)
		return 221;
	if(db == 42424)
		return 221;
	if(db == 42425)
		return 222;
	if(db == 42426)
		return 222;
	if(db == 42427)
		return 223;
	if(db == 42428)
		return 223;
	if(db == 42429)
		return 224;
	if(db == 42430)
		return 224;
	if(db == 42431)
		return 225;
	if(db == 42432)
		return 225;
	if(db == 42433)
		return 226;
	if(db == 42434)
		return 226;
	if(db == 42435)
		return 227;
	if(db == 42436)
		return 227;
	if(db == 42437)
		return 227;
	if(db == 42438)
		return 228;
	if(db == 42439)
		return 228;
	if(db == 42440)
		return 229;
	if(db == 42441)
		return 229;
	if(db == 42442)
		return 230;
	if(db == 42443)
		return 231;
	if(db == 42444)
		return 232;
	if(db == 42445)
		return 233;
	if(db == 42446)
		return 234;
	if(db == 42447)
		return 235;
	if(db == 42448)
		return 235;
	if(db == 42449)
		return 235;
	if(db == 42450)
		return 236;
	if(db == 42451)
		return 236;
	if(db == 42452)
		return 236;
	if(db == 42453)
		return 237;
	if(db == 42454)
		return 237;
	if(db == 42455)
		return 237;
	if(db == 42456)
		return 238;
	if(db == 42457)
		return 238;
	if(db == 42458)
		return 238;
	if(db == 42459)
		return 239;
	if(db == 42460)
		return 239;
	if(db == 42461)
		return 239;
	if(db == 42462)
		return 240;
	if(db == 42463)
		return 241;
	if(db == 42464)
		return 242;
	if(db == 42465)
		return 243;
	if(db == 42466)
		return 244;
	if(db == 42467)
		return 245;
	if(db == 42468)
		return 245;
	if(db == 42469)
		return 246;
	if(db == 42470)
		return 246;
	if(db == 42471)
		return 247;
	if(db == 42472)
		return 247;
	if(db == 42473)
		return 248;
	if(db == 42474)
		return 249;
	if(db == 42475)
		return 250;
	if(db == 42476)
		return 251;
	if(db == 42477)
		return 252;
	if(db == 42478)
		return 253;
	if(db == 42479)
		return 254;
	if(db == 42483)
		return 255;
	if(db <= 255)
		return  db;
	else
		return  -1;
}

void LoadEncoding()
{
	vShiftEnc[0] = "Ç†";
	vShiftEnc[1] = "Ç¢";
	vShiftEnc[2] = "Ç§";
	vShiftEnc[3] = "Ç¶";
	vShiftEnc[4] = "Ç®";
	vShiftEnc[5] = "Ç©";
	vShiftEnc[6] = "Ç´";
	vShiftEnc[7] = "Ç≠";
	vShiftEnc[8] = "ÇØ";
	vShiftEnc[9] = "Ç±";
	vShiftEnc[10] = "Ç≥";
	vShiftEnc[11] = "Çµ";
	vShiftEnc[12] = "Ç∑";
	vShiftEnc[13] = "Çπ";
	vShiftEnc[14] = "Çª";
	vShiftEnc[15] = "ÇΩ";
	vShiftEnc[16] = "Çø";
	vShiftEnc[17] = "Ç¬";
	vShiftEnc[18] = "Çƒ";
	vShiftEnc[19] = "Ç∆";
	vShiftEnc[20] = "Ç»";
	vShiftEnc[21] = "Ç…";
	vShiftEnc[22] = "Ç ";
	vShiftEnc[23] = "ÇÀ";
	vShiftEnc[24] = "ÇÃ";
	vShiftEnc[25] = "ÇÕ";
	vShiftEnc[26] = "Ç–";
	vShiftEnc[27] = "Ç”";
	vShiftEnc[28] = "Ç÷";
	vShiftEnc[29] = "ÇŸ";
	vShiftEnc[30] = "Ç‹";
	vShiftEnc[31] = "Ç›";
	vShiftEnc[32] = "Çﬁ";
	vShiftEnc[33] = "Çﬂ";
	vShiftEnc[34] = "Ç‡";
	vShiftEnc[35] = "Ç‚";
	vShiftEnc[36] = "Ç‰";
	vShiftEnc[37] = "ÇÊ";
	vShiftEnc[38] = "ÇÁ";
	vShiftEnc[39] = "ÇË";
	vShiftEnc[40] = "ÇÈ";
	vShiftEnc[41] = "ÇÍ";
	vShiftEnc[42] = "ÇÎ";
	vShiftEnc[43] = "ÇÌ";
	vShiftEnc[44] = "ÇÓ";
	vShiftEnc[45] = "ÇÔ";
	vShiftEnc[46] = "Ç";
	vShiftEnc[47] = "Ç™";
	vShiftEnc[48] = "Ç¨";
	vShiftEnc[49] = "ÇÆ";
	vShiftEnc[50] = "Ç∞";
	vShiftEnc[51] = "Ç≤";
	vShiftEnc[52] = "Ç¥";
	vShiftEnc[53] = "Ç∂";
	vShiftEnc[54] = "Ç∏";
	vShiftEnc[55] = "Ç∫";
	vShiftEnc[56] = "Çº";
	vShiftEnc[57] = "Çæ";
	vShiftEnc[58] = "Ç¿";
	vShiftEnc[59] = "Ç√";
	vShiftEnc[60] = "Ç≈";
	vShiftEnc[61] = "Ç«";
	vShiftEnc[62] = "ÇŒ";
	vShiftEnc[63] = "Ç—";
	vShiftEnc[64] = "Ç‘";
	vShiftEnc[65] = "Ç◊";
	vShiftEnc[66] = "Ç⁄";
	vShiftEnc[67] = "Çœ";
	vShiftEnc[68] = "Ç“";
	vShiftEnc[69] = "Ç’";
	vShiftEnc[70] = "Çÿ";
	vShiftEnc[71] = "Ç€";
	vShiftEnc[72] = "ÉîÉ@";
	vShiftEnc[73] = "ÉîÉB";
	vShiftEnc[74] = "Éî";
	vShiftEnc[75] = "ÉîÉF";
	vShiftEnc[76] = "ÉîÉH";
	vShiftEnc[77] = "Çü";
	vShiftEnc[78] = "Ç°";
	vShiftEnc[79] = "Ç£";
	vShiftEnc[80] = "Ç•";
	vShiftEnc[81] = "Çß";
	vShiftEnc[82] = "Éï";
	vShiftEnc[83] = "Éñ";
	vShiftEnc[84] = "Ç·";
	vShiftEnc[85] = "Ç„";
	vShiftEnc[86] = "ÇÂ";
	vShiftEnc[87] = "ÇÒ";
	vShiftEnc[88] = "Ç¡";
	vShiftEnc[89] = "ÇÏ";
	vShiftEnc[90] = "Ç´Ç·";
	vShiftEnc[91] = "Ç´Ç„";
	vShiftEnc[92] = "Ç´Ç•";
	vShiftEnc[93] = "Ç´ÇÂ";
	vShiftEnc[94] = "ÇµÇ·";
	vShiftEnc[95] = "ÇµÇ„";
	vShiftEnc[96] = "ÇµÇ•";
	vShiftEnc[97] = "ÇµÇÂ";
	vShiftEnc[98] = "ÇøÇ·";
	vShiftEnc[99] = "ÇøÇ„";
	vShiftEnc[100] = "ÇøÇ•";
	vShiftEnc[101] = "ÇøÇÂ";
	vShiftEnc[102] = "Ç…Ç·";
	vShiftEnc[103] = "Ç…Ç„";
	vShiftEnc[104] = "Ç…Ç•";
	vShiftEnc[105] = "Ç…ÇÂ";
	vShiftEnc[106] = "Ç–Ç·";
	vShiftEnc[107] = "Ç–Ç„";
	vShiftEnc[108] = "Ç–Ç•";
	vShiftEnc[109] = "Ç–ÇÂ";
	vShiftEnc[110] = "Ç›Ç·";
	vShiftEnc[111] = "Ç›Ç„";
	vShiftEnc[112] = "Ç›Ç•";
	vShiftEnc[113] = "Ç›ÇÂ";
	vShiftEnc[114] = "ÇËÇ·";
	vShiftEnc[115] = "ÇËÇ„";
	vShiftEnc[116] = "ÇËÇ•";
	vShiftEnc[117] = "ÇËÇÂ";
	vShiftEnc[118] = "Ç¨Ç·";
	vShiftEnc[119] = "Ç¨Ç„";
	vShiftEnc[120] = "Ç¨Ç•";
	vShiftEnc[121] = "Ç¨ÇÂ";
	vShiftEnc[122] = "Ç∂Ç·";
	vShiftEnc[123] = "Ç∂Ç„";
	vShiftEnc[124] = "Ç∂Ç•";
	vShiftEnc[125] = "Ç∂ÇÂ";
	vShiftEnc[126] = "Ç¿Ç·";
	vShiftEnc[127] = "Ç¿Ç„";
	vShiftEnc[128] = "Ç¿Ç•";
	vShiftEnc[129] = "Ç¿ÇÂ";
	vShiftEnc[130] = "Ç—Ç·";
	vShiftEnc[131] = "Ç—Ç„";
	vShiftEnc[132] = "Ç—Ç•";
	vShiftEnc[133] = "Ç—ÇÂ";
	vShiftEnc[134] = "Ç“Ç·";
	vShiftEnc[135] = "Ç“Ç„";
	vShiftEnc[136] = "Ç“Ç•";
	vShiftEnc[137] = "Ç“ÇÂ";
	vShiftEnc[138] = "Ç¬Çü";
	vShiftEnc[139] = "Ç¬Ç°";
	vShiftEnc[140] = "Ç¬Ç•";
	vShiftEnc[141] = "Ç¬Çß";
	vShiftEnc[142] = "Ç∏Ç°";
	vShiftEnc[143] = "Ç≈Ç°";
	vShiftEnc[144] = "ÉA";
	vShiftEnc[145] = "ÉC";
	vShiftEnc[146] = "ÉE";
	vShiftEnc[147] = "ÉG";
	vShiftEnc[148] = "ÉI";
	vShiftEnc[149] = "ÉJ";
	vShiftEnc[150] = "ÉL";
	vShiftEnc[151] = "ÉN";
	vShiftEnc[152] = "ÉP";
	vShiftEnc[153] = "ÉR";
	vShiftEnc[154] = "ÉT";
	vShiftEnc[155] = "ÉV";
	vShiftEnc[156] = "ÉX";
	vShiftEnc[157] = "ÉZ";
	vShiftEnc[158] = "É\\";
	vShiftEnc[159] = "É^";
	vShiftEnc[160] = "É`";
	vShiftEnc[161] = "Éc";
	vShiftEnc[162] = "Ée";
	vShiftEnc[163] = "Ég";
	vShiftEnc[164] = "Éi";
	vShiftEnc[165] = "Éj";
	vShiftEnc[166] = "Ék";
	vShiftEnc[167] = "Él";
	vShiftEnc[168] = "Ém";
	vShiftEnc[169] = "Én";
	vShiftEnc[170] = "Éq";
	vShiftEnc[171] = "Ét";
	vShiftEnc[172] = "Éw";
	vShiftEnc[173] = "Éz";
	vShiftEnc[174] = "É}";
	vShiftEnc[175] = "É~";
	vShiftEnc[176] = "ÉÄ";
	vShiftEnc[177] = "ÉÅ";
	vShiftEnc[178] = "ÉÇ";
	vShiftEnc[179] = "ÉÑ";
	vShiftEnc[180] = "ÉÜ";
	vShiftEnc[181] = "Éà";
	vShiftEnc[182] = "Éâ";
	vShiftEnc[183] = "Éä";
	vShiftEnc[184] = "Éã";
	vShiftEnc[185] = "Éå";
	vShiftEnc[186] = "Éç";
	vShiftEnc[187] = "Éè";
	vShiftEnc[188] = "Éí";
	vShiftEnc[189] = "Éì";
	vShiftEnc[190] = "Çü";
	vShiftEnc[191] = "Ç°";
	vShiftEnc[192] = "Ç£";
	vShiftEnc[193] = "Ç•";
	vShiftEnc[194] = "Çß";
	vShiftEnc[195] = "Ç·";
	vShiftEnc[196] = "Ç„";
	vShiftEnc[197] = "ÇÂ";
	vShiftEnc[198] = "Ç¡";
	vShiftEnc[199] = "É@";
	vShiftEnc[200] = "ÉB";
	vShiftEnc[201] = "ÉD";
	vShiftEnc[202] = "ÉF";
	vShiftEnc[203] = "ÉH";
	vShiftEnc[204] = "ÉÉ";
	vShiftEnc[205] = "ÉÖ";
	vShiftEnc[206] = "Éá";
	vShiftEnc[207] = "Éb";
	vShiftEnc[208] = "Ç`";
	vShiftEnc[209] = "Ça";
	vShiftEnc[210] = "Çb";
	vShiftEnc[211] = "Çc";
	vShiftEnc[212] = "Çd";
	vShiftEnc[213] = "Çe";
	vShiftEnc[214] = "Çf";
	vShiftEnc[215] = "Çg";
	vShiftEnc[216] = "Çh";
	vShiftEnc[217] = "Çi";
	vShiftEnc[218] = "Çj";
	vShiftEnc[219] = "Çk";
	vShiftEnc[220] = "Çl";
	vShiftEnc[221] = "Çm";
	vShiftEnc[222] = "Çn";
	vShiftEnc[223] = "Ço";
	vShiftEnc[224] = "Çp";
	vShiftEnc[225] = "Çq";
	vShiftEnc[226] = "Çr";
	vShiftEnc[227] = "Çs";
	vShiftEnc[228] = "Çt";
	vShiftEnc[229] = "Çu";
	vShiftEnc[230] = "Çv";
	vShiftEnc[231] = "Çw";
	vShiftEnc[232] = "Çx";
	vShiftEnc[233] = "Çy";
	vShiftEnc[234] = "ÇÅ";
	vShiftEnc[235] = "ÇÇ";
	vShiftEnc[236] = "ÇÉ";
	vShiftEnc[237] = "ÇÑ";
	vShiftEnc[238] = "ÇÖ";
	vShiftEnc[239] = "ÇÜ";
	vShiftEnc[240] = "Çá";
	vShiftEnc[241] = "Çà";
	vShiftEnc[242] = "Çâ";
	vShiftEnc[243] = "Çä";
	vShiftEnc[244] = "Çã";
	vShiftEnc[245] = "Çå";
	vShiftEnc[246] = "Çç";
	vShiftEnc[247] = "Çé";
	vShiftEnc[248] = "Çè";
	vShiftEnc[249] = "Çê";
	vShiftEnc[250] = "Çë";
	vShiftEnc[251] = "Çí";
	vShiftEnc[252] = "Çì";
	vShiftEnc[253] = "Çî";
	vShiftEnc[254] = "Çï";
	vShiftEnc[255] = "Çñ";
	vShiftEnc[256] = "Çó";
	vShiftEnc[257] = "Çò";
	vShiftEnc[258] = "Çô";
	vShiftEnc[259] = "Çö";
	vShiftEnc[260] = "ÇP";
	vShiftEnc[261] = "ÇQ";
	vShiftEnc[262] = "ÇR";
	vShiftEnc[263] = "ÇS";
	vShiftEnc[264] = "ÇT";
	vShiftEnc[265] = "ÇU";
	vShiftEnc[266] = "ÇV";
	vShiftEnc[267] = "ÇW";
	vShiftEnc[268] = "ÇX";
	vShiftEnc[269] = "ÇO";
	vShiftEnc[270] = "Çû";
	vShiftEnc[271] = "Ç@";
	vShiftEnc[272] = "Ç_";
	vShiftEnc[273] = "ÇB";
	vShiftEnc[274] = "ÇC";
	vShiftEnc[275] = "ÇD";
	vShiftEnc[276] = "Ç}";
	vShiftEnc[277] = "ÇE";
	vShiftEnc[278] = "ÇI";
	vShiftEnc[279] = "ÇG";
	vShiftEnc[280] = "ÇH";
	vShiftEnc[281] = "ÇL";
	vShiftEnc[282] = "ÇJ";
	vShiftEnc[283] = "Ç\\";
	vShiftEnc[284] = "Çõ";
	vShiftEnc[285] = "Çù";
	vShiftEnc[286] = "Çz";
	vShiftEnc[287] = "Ç|";
	vShiftEnc[288] = "Çú";
	vShiftEnc[289] = "Ç{";
	vShiftEnc[290] = "ÇY";
	vShiftEnc[291] = "ÇZ";
	vShiftEnc[292] = "ÇA";
	vShiftEnc[293] = "Ç[";
	vShiftEnc[294] = "Ç]";
	vShiftEnc[295] = "ÇK";
	vShiftEnc[296] = "ÇM";
	vShiftEnc[297] = "ÇN";
	vShiftEnc[298] = "Ç^";
	vShiftEnc[299] = "Å@";
	vShiftEnc[300] = "ÅA";
	vShiftEnc[301] = "ÅB";
	vShiftEnc[302] = "ÅC";
	vShiftEnc[303] = "ÅD";
	vShiftEnc[304] = "ÅE";
	vShiftEnc[305] = "ÅF";
	vShiftEnc[306] = "ÅG";
	vShiftEnc[307] = "ÅH";
	vShiftEnc[308] = "ÅI";
	vShiftEnc[309] = "ÅJ";
	vShiftEnc[310] = "ÅK";
	vShiftEnc[311] = "ÅL";
	vShiftEnc[312] = "ÅM";
	vShiftEnc[313] = "ÅN";
	vShiftEnc[314] = "ÅO";
	vShiftEnc[315] = "ÅP";
	vShiftEnc[316] = "ÅQ";
	vShiftEnc[317] = "ÅR";
	vShiftEnc[318] = "ÅS";
	vShiftEnc[319] = "ÅT";
	vShiftEnc[320] = "ÅU";
	vShiftEnc[321] = "ÅV";
	vShiftEnc[322] = "ÅW";
	vShiftEnc[323] = "ÅX";
	vShiftEnc[324] = "ÅY";
	vShiftEnc[325] = "Å^";
	vShiftEnc[326] = "Å_";
	vShiftEnc[327] = "Å`";
	vShiftEnc[328] = "Åe";
	vShiftEnc[329] = "Åf";
	vShiftEnc[330] = "Åg";
	vShiftEnc[331] = "Åh";
	vShiftEnc[332] = "Åi";
	vShiftEnc[333] = "Åj";
	vShiftEnc[334] = "Åm";
	vShiftEnc[335] = "Ån";
	vShiftEnc[336] = "Åo";
	vShiftEnc[337] = "Åp";
	vShiftEnc[338] = "Åu";
	vShiftEnc[339] = "Åv";
	vShiftEnc[340] = "Åw";
	vShiftEnc[341] = "Åx";
	vShiftEnc[342] = "Åã";
	vShiftEnc[343] = "Åå";
	vShiftEnc[344] = "Åç";
	vShiftEnc[345] = "Åè";
	vShiftEnc[346] = "Åê";
	vShiftEnc[347] = "Åì";
	vShiftEnc[348] = "Åî";
	vShiftEnc[349] = "Åï";
	vShiftEnc[350] = "Åñ";
	vShiftEnc[351] = "Åó";
	vEucEnc[0] = "§¢";
	vEucEnc[1] = "§§";
	vEucEnc[2] = "§¶";
	vEucEnc[3] = "§®";
	vEucEnc[4] = "§™";
	vEucEnc[5] = "§´";
	vEucEnc[6] = "§≠";
	vEucEnc[7] = "§Ø";
	vEucEnc[8] = "§±";
	vEucEnc[9] = "§≥";
	vEucEnc[10] = "§µ";
	vEucEnc[11] = "§∑";
	vEucEnc[12] = "§π";
	vEucEnc[13] = "§ª";
	vEucEnc[14] = "§Ω";
	vEucEnc[15] = "§ø";
	vEucEnc[16] = "§¡";
	vEucEnc[17] = "§ƒ";
	vEucEnc[18] = "§∆";
	vEucEnc[19] = "§»";
	vEucEnc[20] = "§ ";
	vEucEnc[21] = "§À";
	vEucEnc[22] = "§Ã";
	vEucEnc[23] = "§Õ";
	vEucEnc[24] = "§Œ";
	vEucEnc[25] = "§œ";
	vEucEnc[26] = "§“";
	vEucEnc[27] = "§’";
	vEucEnc[28] = "§ÿ";
	vEucEnc[29] = "§€";
	vEucEnc[30] = "§ﬁ";
	vEucEnc[31] = "§ﬂ";
	vEucEnc[32] = "§‡";
	vEucEnc[33] = "§·";
	vEucEnc[34] = "§‚";
	vEucEnc[35] = "§‰";
	vEucEnc[36] = "§Ê";
	vEucEnc[37] = "§Ë";
	vEucEnc[38] = "§È";
	vEucEnc[39] = "§Í";
	vEucEnc[40] = "§Î";
	vEucEnc[41] = "§Ï";
	vEucEnc[42] = "§Ì";
	vEucEnc[43] = "§Ô";
	vEucEnc[44] = "§";
	vEucEnc[45] = "§Ò";
	vEucEnc[46] = "§Ú";
	vEucEnc[47] = "§¨";
	vEucEnc[48] = "§Æ";
	vEucEnc[49] = "§∞";
	vEucEnc[50] = "§≤";
	vEucEnc[51] = "§¥";
	vEucEnc[52] = "§∂";
	vEucEnc[53] = "§∏";
	vEucEnc[54] = "§∫";
	vEucEnc[55] = "§º";
	vEucEnc[56] = "§æ";
	vEucEnc[57] = "§¿";
	vEucEnc[58] = "§¬";
	vEucEnc[59] = "§≈";
	vEucEnc[60] = "§«";
	vEucEnc[61] = "§…";
	vEucEnc[62] = "§–";
	vEucEnc[63] = "§”";
	vEucEnc[64] = "§÷";
	vEucEnc[65] = "§Ÿ";
	vEucEnc[66] = "§‹";
	vEucEnc[67] = "§—";
	vEucEnc[68] = "§‘";
	vEucEnc[69] = "§◊";
	vEucEnc[70] = "§⁄";
	vEucEnc[71] = "§›";
	vEucEnc[72] = "•Ù•°";
	vEucEnc[73] = "•Ù•£";
	vEucEnc[74] = "•Ù";
	vEucEnc[75] = "•Ù•ß";
	vEucEnc[76] = "•Ù•©";
	vEucEnc[77] = "§°";
	vEucEnc[78] = "§£";
	vEucEnc[79] = "§•";
	vEucEnc[80] = "§ß";
	vEucEnc[81] = "§©";
	vEucEnc[82] = "•ı";
	vEucEnc[83] = "•ˆ";
	vEucEnc[84] = "§„";
	vEucEnc[85] = "§Â";
	vEucEnc[86] = "§Á";
	vEucEnc[87] = "§Û";
	vEucEnc[88] = "§√";
	vEucEnc[89] = "§Ó";
	vEucEnc[90] = "§≠§„";
	vEucEnc[91] = "§≠§Â";
	vEucEnc[92] = "§≠§ß";
	vEucEnc[93] = "§≠§Á";
	vEucEnc[94] = "§∑§„";
	vEucEnc[95] = "§∑§Â";
	vEucEnc[96] = "§∑§ß";
	vEucEnc[97] = "§∑§Á";
	vEucEnc[98] = "§¡§„";
	vEucEnc[99] = "§¡§Â";
	vEucEnc[100] = "§¡§ß";
	vEucEnc[101] = "§¡§Á";
	vEucEnc[102] = "§À§„";
	vEucEnc[103] = "§À§Â";
	vEucEnc[104] = "§À§ß";
	vEucEnc[105] = "§À§Á";
	vEucEnc[106] = "§“§„";
	vEucEnc[107] = "§“§Â";
	vEucEnc[108] = "§“§ß";
	vEucEnc[109] = "§“§Á";
	vEucEnc[110] = "§ﬂ§„";
	vEucEnc[111] = "§ﬂ§Â";
	vEucEnc[112] = "§ﬂ§ß";
	vEucEnc[113] = "§ﬂ§Á";
	vEucEnc[114] = "§Í§„";
	vEucEnc[115] = "§Í§Â";
	vEucEnc[116] = "§Í§ß";
	vEucEnc[117] = "§Í§Á";
	vEucEnc[118] = "§Æ§„";
	vEucEnc[119] = "§Æ§Â";
	vEucEnc[120] = "§Æ§ß";
	vEucEnc[121] = "§Æ§Á";
	vEucEnc[122] = "§∏§„";
	vEucEnc[123] = "§∏§Â";
	vEucEnc[124] = "§∏§ß";
	vEucEnc[125] = "§∏§Á";
	vEucEnc[126] = "§¬§„";
	vEucEnc[127] = "§¬§Â";
	vEucEnc[128] = "§¬§ß";
	vEucEnc[129] = "§¬§Á";
	vEucEnc[130] = "§”§„";
	vEucEnc[131] = "§”§Â";
	vEucEnc[132] = "§”§ß";
	vEucEnc[133] = "§”§Á";
	vEucEnc[134] = "§‘§„";
	vEucEnc[135] = "§‘§Â";
	vEucEnc[136] = "§‘§ß";
	vEucEnc[137] = "§‘§Á";
	vEucEnc[138] = "§ƒ§°";
	vEucEnc[139] = "§ƒ§£";
	vEucEnc[140] = "§ƒ§ß";
	vEucEnc[141] = "§ƒ§©";
	vEucEnc[142] = "§∫§£";
	vEucEnc[143] = "§«§£";
	vEucEnc[144] = "•¢";
	vEucEnc[145] = "•§";
	vEucEnc[146] = "•¶";
	vEucEnc[147] = "•®";
	vEucEnc[148] = "•™";
	vEucEnc[149] = "•´";
	vEucEnc[150] = "•≠";
	vEucEnc[151] = "•Ø";
	vEucEnc[152] = "•±";
	vEucEnc[153] = "•≥";
	vEucEnc[154] = "•µ";
	vEucEnc[155] = "•∑";
	vEucEnc[156] = "•π";
	vEucEnc[157] = "•ª";
	vEucEnc[158] = "•Ω";
	vEucEnc[159] = "•ø";
	vEucEnc[160] = "•¡";
	vEucEnc[161] = "•ƒ";
	vEucEnc[162] = "•∆";
	vEucEnc[163] = "•»";
	vEucEnc[164] = "• ";
	vEucEnc[165] = "•À";
	vEucEnc[166] = "•Ã";
	vEucEnc[167] = "•Õ";
	vEucEnc[168] = "•Œ";
	vEucEnc[169] = "•œ";
	vEucEnc[170] = "•“";
	vEucEnc[171] = "•’";
	vEucEnc[172] = "•ÿ";
	vEucEnc[173] = "•€";
	vEucEnc[174] = "•ﬁ";
	vEucEnc[175] = "•ﬂ";
	vEucEnc[176] = "•‡";
	vEucEnc[177] = "•·";
	vEucEnc[178] = "•‚";
	vEucEnc[179] = "•‰";
	vEucEnc[180] = "•Ê";
	vEucEnc[181] = "•Ë";
	vEucEnc[182] = "•È";
	vEucEnc[183] = "•Í";
	vEucEnc[184] = "•Î";
	vEucEnc[185] = "•Ï";
	vEucEnc[186] = "•Ì";
	vEucEnc[187] = "•Ô";
	vEucEnc[188] = "•Ú";
	vEucEnc[189] = "•Û";
	vEucEnc[190] = "§°";
	vEucEnc[191] = "§£";
	vEucEnc[192] = "§•";
	vEucEnc[193] = "§ß";
	vEucEnc[194] = "§©";
	vEucEnc[195] = "§„";
	vEucEnc[196] = "§Â";
	vEucEnc[197] = "§Á";
	vEucEnc[198] = "§√";
	vEucEnc[199] = "•°";
	vEucEnc[200] = "•£";
	vEucEnc[201] = "••";
	vEucEnc[202] = "•ß";
	vEucEnc[203] = "•©";
	vEucEnc[204] = "•„";
	vEucEnc[205] = "•Â";
	vEucEnc[206] = "•Á";
	vEucEnc[207] = "•√";
	vEucEnc[208] = "£¡";
	vEucEnc[209] = "£¬";
	vEucEnc[210] = "£√";
	vEucEnc[211] = "£ƒ";
	vEucEnc[212] = "£≈";
	vEucEnc[213] = "£∆";
	vEucEnc[214] = "£«";
	vEucEnc[215] = "£»";
	vEucEnc[216] = "£…";
	vEucEnc[217] = "£ ";
	vEucEnc[218] = "£À";
	vEucEnc[219] = "£Ã";
	vEucEnc[220] = "£Õ";
	vEucEnc[221] = "£Œ";
	vEucEnc[222] = "£œ";
	vEucEnc[223] = "£–";
	vEucEnc[224] = "£—";
	vEucEnc[225] = "£“";
	vEucEnc[226] = "£”";
	vEucEnc[227] = "£‘";
	vEucEnc[228] = "£’";
	vEucEnc[229] = "£÷";
	vEucEnc[230] = "£◊";
	vEucEnc[231] = "£ÿ";
	vEucEnc[232] = "£Ÿ";
	vEucEnc[233] = "£⁄";
	vEucEnc[234] = "£·";
	vEucEnc[235] = "£‚";
	vEucEnc[236] = "£„";
	vEucEnc[237] = "£‰";
	vEucEnc[238] = "£Â";
	vEucEnc[239] = "£Ê";
	vEucEnc[240] = "£Á";
	vEucEnc[241] = "£Ë";
	vEucEnc[242] = "£È";
	vEucEnc[243] = "£Í";
	vEucEnc[244] = "£Î";
	vEucEnc[245] = "£Ï";
	vEucEnc[246] = "£Ì";
	vEucEnc[247] = "£Ó";
	vEucEnc[248] = "£Ô";
	vEucEnc[249] = "£";
	vEucEnc[250] = "£Ò";
	vEucEnc[251] = "£Ú";
	vEucEnc[252] = "£Û";
	vEucEnc[253] = "£Ù";
	vEucEnc[254] = "£ı";
	vEucEnc[255] = "£ˆ";
	vEucEnc[256] = "£˜";
	vEucEnc[257] = "£¯";
	vEucEnc[258] = "£˘";
	vEucEnc[259] = "£˙";
	vEucEnc[260] = "£±";
	vEucEnc[261] = "£≤";
	vEucEnc[262] = "£≥";
	vEucEnc[263] = "£¥";
	vEucEnc[264] = "£µ";
	vEucEnc[265] = "£∂";
	vEucEnc[266] = "£∑";
	vEucEnc[267] = "£∏";
	vEucEnc[268] = "£π";
	vEucEnc[269] = "£∞";
	vEucEnc[270] = "£˛";
	vEucEnc[271] = "£°";
	vEucEnc[272] = "£¿";
	vEucEnc[273] = "££";
	vEucEnc[274] = "£§";
	vEucEnc[275] = "£•";
	vEucEnc[276] = "£ﬁ";
	vEucEnc[277] = "£¶";
	vEucEnc[278] = "£™";
	vEucEnc[279] = "£®";
	vEucEnc[280] = "£©";
	vEucEnc[281] = "£≠";
	vEucEnc[282] = "£´";
	vEucEnc[283] = "£Ω";
	vEucEnc[284] = "£˚";
	vEucEnc[285] = "£˝";
	vEucEnc[286] = "£€";
	vEucEnc[287] = "£›";
	vEucEnc[288] = "£¸";
	vEucEnc[289] = "£‹";
	vEucEnc[290] = "£∫";
	vEucEnc[291] = "£ª";
	vEucEnc[292] = "£¢";
	vEucEnc[293] = "£º";
	vEucEnc[294] = "£æ";
	vEucEnc[295] = "£¨";
	vEucEnc[296] = "£Æ";
	vEucEnc[297] = "£Ø";
	vEucEnc[298] = "£ø";
	vEucEnc[299] = "°°";
	vEucEnc[300] = "°¢";
	vEucEnc[301] = "°£";
	vEucEnc[302] = "°§";
	vEucEnc[303] = "°•";
	vEucEnc[304] = "°¶";
	vEucEnc[305] = "°ß";
	vEucEnc[306] = "°®";
	vEucEnc[307] = "°©";
	vEucEnc[308] = "°™";
	vEucEnc[309] = "°´";
	vEucEnc[310] = "°¨";
	vEucEnc[311] = "°≠";
	vEucEnc[312] = "°Æ";
	vEucEnc[313] = "°Ø";
	vEucEnc[314] = "°∞";
	vEucEnc[315] = "°±";
	vEucEnc[316] = "°≤";
	vEucEnc[317] = "°≥";
	vEucEnc[318] = "°¥";
	vEucEnc[319] = "°µ";
	vEucEnc[320] = "°∂";
	vEucEnc[321] = "°∑";
	vEucEnc[322] = "°∏";
	vEucEnc[323] = "°π";
	vEucEnc[324] = "°∫";
	vEucEnc[325] = "°ø";
	vEucEnc[326] = "°¿";
	vEucEnc[327] = "°¡";
	vEucEnc[328] = "°∆";
	vEucEnc[329] = "°«";
	vEucEnc[330] = "°»";
	vEucEnc[331] = "°…";
	vEucEnc[332] = "° ";
	vEucEnc[333] = "°À";
	vEucEnc[334] = "°Œ";
	vEucEnc[335] = "°œ";
	vEucEnc[336] = "°–";
	vEucEnc[337] = "°—";
	vEucEnc[338] = "°÷";
	vEucEnc[339] = "°◊";
	vEucEnc[340] = "°ÿ";
	vEucEnc[341] = "°Ÿ";
	vEucEnc[342] = "°Î";
	vEucEnc[343] = "°Ï";
	vEucEnc[344] = "°Ì";
	vEucEnc[345] = "°Ô";
	vEucEnc[346] = "°";
	vEucEnc[347] = "°Û";
	vEucEnc[348] = "°Ù";
	vEucEnc[349] = "°ı";
	vEucEnc[350] = "°ˆ";
	vEucEnc[351] = "°˜";
	vRomajiEnc[0] = "a";
	vRomajiEnc[1] = "i";
	vRomajiEnc[2] = "u";
	vRomajiEnc[3] = "e";
	vRomajiEnc[4] = "o";
	vRomajiEnc[5] = "ka";
	vRomajiEnc[6] = "ki";
	vRomajiEnc[7] = "ku";
	vRomajiEnc[8] = "ke";
	vRomajiEnc[9] = "ko";
	vRomajiEnc[10] = "sa";
	vRomajiEnc[11] = "si";
	vRomajiEnc[12] = "su";
	vRomajiEnc[13] = "se";
	vRomajiEnc[14] = "so";
	vRomajiEnc[15] = "ta";
	vRomajiEnc[16] = "ti";
	vRomajiEnc[17] = "tu";
	vRomajiEnc[18] = "te";
	vRomajiEnc[19] = "to";
	vRomajiEnc[20] = "na";
	vRomajiEnc[21] = "ni";
	vRomajiEnc[22] = "nu";
	vRomajiEnc[23] = "ne";
	vRomajiEnc[24] = "no";
	vRomajiEnc[25] = "ha";
	vRomajiEnc[26] = "hi";
	vRomajiEnc[27] = "hu";
	vRomajiEnc[28] = "he";
	vRomajiEnc[29] = "ho";
	vRomajiEnc[30] = "ma";
	vRomajiEnc[31] = "mi";
	vRomajiEnc[32] = "mu";
	vRomajiEnc[33] = "me";
	vRomajiEnc[34] = "mo";
	vRomajiEnc[35] = "ya";
	vRomajiEnc[36] = "yu";
	vRomajiEnc[37] = "yo";
	vRomajiEnc[38] = "ra";
	vRomajiEnc[39] = "ri";
	vRomajiEnc[40] = "ru";
	vRomajiEnc[41] = "re";
	vRomajiEnc[42] = "ro";
	vRomajiEnc[43] = "wa";
	vRomajiEnc[44] = "wi";
	vRomajiEnc[45] = "we";
	vRomajiEnc[46] = "wo";
	vRomajiEnc[47] = "ga";
	vRomajiEnc[48] = "gi";
	vRomajiEnc[49] = "gu";
	vRomajiEnc[50] = "ge";
	vRomajiEnc[51] = "go";
	vRomajiEnc[52] = "za";
	vRomajiEnc[53] = "ji";
	vRomajiEnc[54] = "zu";
	vRomajiEnc[55] = "ze";
	vRomajiEnc[56] = "zo";
	vRomajiEnc[57] = "da";
	vRomajiEnc[58] = "dji";
	vRomajiEnc[59] = "du";
	vRomajiEnc[60] = "de";
	vRomajiEnc[61] = "do";
	vRomajiEnc[62] = "ba";
	vRomajiEnc[63] = "bi";
	vRomajiEnc[64] = "bu";
	vRomajiEnc[65] = "be";
	vRomajiEnc[66] = "bo";
	vRomajiEnc[67] = "pa";
	vRomajiEnc[68] = "pi";
	vRomajiEnc[69] = "pu";
	vRomajiEnc[70] = "pe";
	vRomajiEnc[71] = "po";
	vRomajiEnc[72] = "va";
	vRomajiEnc[73] = "vi";
	vRomajiEnc[74] = "vu";
	vRomajiEnc[75] = "ve";
	vRomajiEnc[76] = "vo";
	vRomajiEnc[77] = "xa";
	vRomajiEnc[78] = "xi";
	vRomajiEnc[79] = "xu";
	vRomajiEnc[80] = "xe";
	vRomajiEnc[81] = "xo";
	vRomajiEnc[82] = "xka";
	vRomajiEnc[83] = "xke";
	vRomajiEnc[84] = "xya";
	vRomajiEnc[85] = "xyu";
	vRomajiEnc[86] = "xyo";
	vRomajiEnc[87] = "n";
	vRomajiEnc[88] = "tt";
	vRomajiEnc[89] = "xwa";
	vRomajiEnc[90] = "kya";
	vRomajiEnc[91] = "kyu";
	vRomajiEnc[92] = "kye";
	vRomajiEnc[93] = "kyo";
	vRomajiEnc[94] = "sha";
	vRomajiEnc[95] = "shu";
	vRomajiEnc[96] = "she";
	vRomajiEnc[97] = "sho";
	vRomajiEnc[98] = "cha";
	vRomajiEnc[99] = "chu";
	vRomajiEnc[100] = "che";
	vRomajiEnc[101] = "cho";
	vRomajiEnc[102] = "nya";
	vRomajiEnc[103] = "nyu";
	vRomajiEnc[104] = "nye";
	vRomajiEnc[105] = "nyo";
	vRomajiEnc[106] = "hya";
	vRomajiEnc[107] = "hyu";
	vRomajiEnc[108] = "hye";
	vRomajiEnc[109] = "hyo";
	vRomajiEnc[110] = "mya";
	vRomajiEnc[111] = "myu";
	vRomajiEnc[112] = "mye";
	vRomajiEnc[113] = "myo";
	vRomajiEnc[114] = "rya";
	vRomajiEnc[115] = "ryu";
	vRomajiEnc[116] = "rye";
	vRomajiEnc[117] = "ryo";
	vRomajiEnc[118] = "gya";
	vRomajiEnc[119] = "gyu";
	vRomajiEnc[120] = "gye";
	vRomajiEnc[121] = "gyo";
	vRomajiEnc[122] = "ja";
	vRomajiEnc[123] = "ju";
	vRomajiEnc[124] = "je";
	vRomajiEnc[125] = "jo";
	vRomajiEnc[126] = "dja";
	vRomajiEnc[127] = "dju";
	vRomajiEnc[128] = "dje";
	vRomajiEnc[129] = "djo";
	vRomajiEnc[130] = "bya";
	vRomajiEnc[131] = "byu";
	vRomajiEnc[132] = "bye";
	vRomajiEnc[133] = "byo";
	vRomajiEnc[134] = "pya";
	vRomajiEnc[135] = "pyu";
	vRomajiEnc[136] = "pye";
	vRomajiEnc[137] = "pyo";
	vRomajiEnc[138] = "tsa";
	vRomajiEnc[139] = "tsi";
	vRomajiEnc[140] = "tse";
	vRomajiEnc[141] = "tso";
	vRomajiEnc[142] = "zi";
	vRomajiEnc[143] = "di";
	vRomajiEnc[144] = "<a>";
	vRomajiEnc[145] = "<i>";
	vRomajiEnc[146] = "<u>";
	vRomajiEnc[147] = "<e>";
	vRomajiEnc[148] = "<o>";
	vRomajiEnc[149] = "<ka>";
	vRomajiEnc[150] = "<ki>";
	vRomajiEnc[151] = "<ku>";
	vRomajiEnc[152] = "<ke>";
	vRomajiEnc[153] = "<ko>";
	vRomajiEnc[154] = "<sa>";
	vRomajiEnc[155] = "<si>";
	vRomajiEnc[156] = "<su>";
	vRomajiEnc[157] = "<se>";
	vRomajiEnc[158] = "<so>";
	vRomajiEnc[159] = "<ta>";
	vRomajiEnc[160] = "<ti>";
	vRomajiEnc[161] = "<tu>";
	vRomajiEnc[162] = "<te>";
	vRomajiEnc[163] = "<to>";
	vRomajiEnc[164] = "<na>";
	vRomajiEnc[165] = "<ni>";
	vRomajiEnc[166] = "<nu>";
	vRomajiEnc[167] = "<ne>";
	vRomajiEnc[168] = "<no>";
	vRomajiEnc[169] = "<ha>";
	vRomajiEnc[170] = "<hi>";
	vRomajiEnc[171] = "<hu>";
	vRomajiEnc[172] = "<he>";
	vRomajiEnc[173] = "<ho>";
	vRomajiEnc[174] = "<ma>";
	vRomajiEnc[175] = "<mi>";
	vRomajiEnc[176] = "<mu>";
	vRomajiEnc[177] = "<me>";
	vRomajiEnc[178] = "<mo>";
	vRomajiEnc[179] = "<ya>";
	vRomajiEnc[180] = "<yu>";
	vRomajiEnc[181] = "<yo>";
	vRomajiEnc[182] = "<ra>";
	vRomajiEnc[183] = "<ri>";
	vRomajiEnc[184] = "<ru>";
	vRomajiEnc[185] = "<re>";
	vRomajiEnc[186] = "<ro>";
	vRomajiEnc[187] = "<wa>";
	vRomajiEnc[188] = "<wo>";
	vRomajiEnc[189] = "<n>";
	vRomajiEnc[190] = "<<a>>";
	vRomajiEnc[191] = "<<i>>";
	vRomajiEnc[192] = "<<u>>";
	vRomajiEnc[193] = "<<e>>";
	vRomajiEnc[194] = "<<o>>";
	vRomajiEnc[195] = "<<ya>>";
	vRomajiEnc[196] = "<<yu>>";
	vRomajiEnc[197] = "<<yo>>";
	vRomajiEnc[198] = "<<tsu>>";
	vRomajiEnc[199] = "<<<a>>>";
	vRomajiEnc[200] = "<<<i>>>";
	vRomajiEnc[201] = "<<<u>>>";
	vRomajiEnc[202] = "<<<e>>>";
	vRomajiEnc[203] = "<<<o>>>";
	vRomajiEnc[204] = "<<<ya>>>";
	vRomajiEnc[205] = "<<<yu>>>";
	vRomajiEnc[206] = "<<<yo>>>";
	vRomajiEnc[207] = "<<<tsu>>>";
	vRomajiEnc[208] = "<A>";
	vRomajiEnc[209] = "<B>";
	vRomajiEnc[210] = "<C>";
	vRomajiEnc[211] = "<D>";
	vRomajiEnc[212] = "<E>";
	vRomajiEnc[213] = "<F>";
	vRomajiEnc[214] = "<G>";
	vRomajiEnc[215] = "<H>";
	vRomajiEnc[216] = "<I>";
	vRomajiEnc[217] = "<J>";
	vRomajiEnc[218] = "<K>";
	vRomajiEnc[219] = "<L>";
	vRomajiEnc[220] = "<M>";
	vRomajiEnc[221] = "<N>";
	vRomajiEnc[222] = "<O>";
	vRomajiEnc[223] = "<P>";
	vRomajiEnc[224] = "<Q>";
	vRomajiEnc[225] = "<R>";
	vRomajiEnc[226] = "<S>";
	vRomajiEnc[227] = "<T>";
	vRomajiEnc[228] = "<U>";
	vRomajiEnc[229] = "<V>";
	vRomajiEnc[230] = "<W>";
	vRomajiEnc[231] = "<X>";
	vRomajiEnc[232] = "<Y>";
	vRomajiEnc[233] = "<Z>";
	vRomajiEnc[234] = "<a>";
	vRomajiEnc[235] = "<b>";
	vRomajiEnc[236] = "<c>";
	vRomajiEnc[237] = "<d>";
	vRomajiEnc[238] = "<e>";
	vRomajiEnc[239] = "<f>";
	vRomajiEnc[240] = "<g>";
	vRomajiEnc[241] = "<h>";
	vRomajiEnc[242] = "<i>";
	vRomajiEnc[243] = "<j>";
	vRomajiEnc[244] = "<k>";
	vRomajiEnc[245] = "<l>";
	vRomajiEnc[246] = "<m>";
	vRomajiEnc[247] = "<n>";
	vRomajiEnc[248] = "<o>";
	vRomajiEnc[249] = "<p>";
	vRomajiEnc[250] = "<q>";
	vRomajiEnc[251] = "<r>";
	vRomajiEnc[252] = "<s>";
	vRomajiEnc[253] = "<t>";
	vRomajiEnc[254] = "<u>";
	vRomajiEnc[255] = "<v>";
	vRomajiEnc[256] = "<w>";
	vRomajiEnc[257] = "<x>";
	vRomajiEnc[258] = "<y>";
	vRomajiEnc[259] = "<z>";
	vRomajiEnc[260] = "<1>";
	vRomajiEnc[261] = "<2>";
	vRomajiEnc[262] = "<3>";
	vRomajiEnc[263] = "<4>";
	vRomajiEnc[264] = "<5>";
	vRomajiEnc[265] = "<6>";
	vRomajiEnc[266] = "<7>";
	vRomajiEnc[267] = "<8>";
	vRomajiEnc[268] = "<9>";
	vRomajiEnc[269] = "<0>";
	vRomajiEnc[270] = "<~>";
	vRomajiEnc[271] = "<!>";
	vRomajiEnc[272] = "<@>";
	vRomajiEnc[273] = "<#>";
	vRomajiEnc[274] = "<$>";
	vRomajiEnc[275] = "<%>";
	vRomajiEnc[276] = "<^>";
	vRomajiEnc[277] = "<&>";
	vRomajiEnc[278] = "<*>";
	vRomajiEnc[279] = "<(>";
	vRomajiEnc[280] = "<)>";
	vRomajiEnc[281] = "<->";
	vRomajiEnc[282] = "<+>";
	vRomajiEnc[283] = "<=>";
	vRomajiEnc[284] = "<{>";
	vRomajiEnc[285] = "<}>";
	vRomajiEnc[286] = "<[>";
	vRomajiEnc[287] = "<]>";
	vRomajiEnc[288] = "<|>";
	vRomajiEnc[289] = "<\\>";
	vRomajiEnc[290] = "<:>";
	vRomajiEnc[291] = "<;>";
	vRomajiEnc[292] = "<\">";
	vRomajiEnc[293] = "<<>";
	vRomajiEnc[294] = "<>>";
	vRomajiEnc[295] = "<,>";
	vRomajiEnc[296] = "<.>";
	vRomajiEnc[297] = "</>";
	vRomajiEnc[298] = "<?>";
	vRomajiEnc[299] = " ";
	vRomajiEnc[300] = "ÅA";
	vRomajiEnc[301] = "ÅB";
	vRomajiEnc[302] = ",";
	vRomajiEnc[303] = ".";
	vRomajiEnc[304] = "ÅE";
	vRomajiEnc[305] = ":";
	vRomajiEnc[306] = ";";
	vRomajiEnc[307] = "?";
	vRomajiEnc[308] = "!";
	vRomajiEnc[309] = "ÅJ";
	vRomajiEnc[310] = "ÅK";
	vRomajiEnc[311] = "ÅL";
	vRomajiEnc[312] = "ÅM";
	vRomajiEnc[313] = "ÅN";
	vRomajiEnc[314] = "^";
	vRomajiEnc[315] = "ÅP";
	vRomajiEnc[316] = "ÅQ";
	vRomajiEnc[317] = "ÅR";
	vRomajiEnc[318] = "ÅS";
	vRomajiEnc[319] = "ÅT";
	vRomajiEnc[320] = "ÅU";
	vRomajiEnc[321] = "ÅV";
	vRomajiEnc[322] = "ÅW";
	vRomajiEnc[323] = "ÅX";
	vRomajiEnc[324] = "ÅY";
	vRomajiEnc[325] = "/";
	vRomajiEnc[326] = "\\";
	vRomajiEnc[327] = "~";
	vRomajiEnc[328] = "Åe";
	vRomajiEnc[329] = "Åf";
	vRomajiEnc[330] = "Åg";
	vRomajiEnc[331] = "Åh";
	vRomajiEnc[332] = "(";
	vRomajiEnc[333] = ")";
	vRomajiEnc[334] = "[";
	vRomajiEnc[335] = "]";
	vRomajiEnc[336] = "{";
	vRomajiEnc[337] = "}";
	vRomajiEnc[338] = "Åu";
	vRomajiEnc[339] = "Åv";
	vRomajiEnc[340] = "Åw";
	vRomajiEnc[341] = "Åx";
	vRomajiEnc[342] = "Åã";
	vRomajiEnc[343] = "Åå";
	vRomajiEnc[344] = "Åç";
	vRomajiEnc[345] = "Åè";
	vRomajiEnc[346] = "$";
	vRomajiEnc[347] = "%";
	vRomajiEnc[348] = "$";
	vRomajiEnc[349] = "&";
	vRomajiEnc[350] = "*";
	vRomajiEnc[351] = "@";
}

#endif