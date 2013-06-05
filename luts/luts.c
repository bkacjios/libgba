const fixed_p fp_sin[360] = {
0x0000,0x0004,0x0008,0x000d,0x0011,0x0016,0x001a,0x001f,
0x0023,0x0028,0x002c,0x0030,0x0035,0x0039,0x003d,0x0042,
0x0046,0x004a,0x004f,0x0053,0x0057,0x005b,0x005f,0x0064,
0x0068,0x006c,0x0070,0x0074,0x0078,0x007c,0x007f,0x0083,
0x0087,0x008b,0x008f,0x0092,0x0096,0x009a,0x009d,0x00a1,
0x00a4,0x00a7,0x00ab,0x00ae,0x00b1,0x00b5,0x00b8,0x00bb,
0x00be,0x00c1,0x00c4,0x00c6,0x00c9,0x00cc,0x00cf,0x00d1,
0x00d4,0x00d6,0x00d9,0x00db,0x00dd,0x00df,0x00e2,0x00e4,
0x00e6,0x00e8,0x00e9,0x00eb,0x00ed,0x00ee,0x00f0,0x00f2,
0x00f3,0x00f4,0x00f6,0x00f7,0x00f8,0x00f9,0x00fa,0x00fb,
0x00fc,0x00fc,0x00fd,0x00fe,0x00fe,0x00ff,0x00ff,0x00ff,
0x00ff,0x00ff,0x0100,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
0x00fe,0x00fe,0x00fd,0x00fc,0x00fc,0x00fb,0x00fa,0x00f9,
0x00f8,0x00f7,0x00f6,0x00f4,0x00f3,0x00f2,0x00f0,0x00ee,
0x00ed,0x00eb,0x00e9,0x00e8,0x00e6,0x00e4,0x00e2,0x00df,
0x00dd,0x00db,0x00d9,0x00d6,0x00d4,0x00d1,0x00cf,0x00cc,
0x00c9,0x00c6,0x00c4,0x00c1,0x00be,0x00bb,0x00b8,0x00b5,
0x00b1,0x00ae,0x00ab,0x00a7,0x00a4,0x00a1,0x009d,0x009a,
0x0096,0x0092,0x008f,0x008b,0x0087,0x0083,0x007f,0x007c,
0x0078,0x0074,0x0070,0x006c,0x0068,0x0064,0x005f,0x005b,
0x0057,0x0053,0x004f,0x004a,0x0046,0x0042,0x003d,0x0039,
0x0035,0x0030,0x002c,0x0028,0x0023,0x001f,0x001a,0x0016,
0x0011,0x000d,0x0008,0x0004,0x0000,0xfffc,0xfff8,0xfff3,
0xffef,0xffea,0xffe6,0xffe1,0xffdd,0xffd8,0xffd4,0xffd0,
0xffcb,0xffc7,0xffc3,0xffbe,0xffba,0xffb6,0xffb1,0xffad,
0xffa9,0xffa5,0xffa1,0xff9c,0xff98,0xff94,0xff90,0xff8c,
0xff88,0xff84,0xff80,0xff7d,0xff79,0xff75,0xff71,0xff6e,
0xff6a,0xff66,0xff63,0xff5f,0xff5c,0xff59,0xff55,0xff52,
0xff4f,0xff4b,0xff48,0xff45,0xff42,0xff3f,0xff3c,0xff3a,
0xff37,0xff34,0xff31,0xff2f,0xff2c,0xff2a,0xff27,0xff25,
0xff23,0xff21,0xff1e,0xff1c,0xff1a,0xff18,0xff17,0xff15,
0xff13,0xff12,0xff10,0xff0e,0xff0d,0xff0c,0xff0a,0xff09,
0xff08,0xff07,0xff06,0xff05,0xff04,0xff04,0xff03,0xff02,
0xff02,0xff01,0xff01,0xff01,0xff01,0xff01,0xff00,0xff01,
0xff01,0xff01,0xff01,0xff01,0xff02,0xff02,0xff03,0xff04,
0xff04,0xff05,0xff06,0xff07,0xff08,0xff09,0xff0a,0xff0c,
0xff0d,0xff0e,0xff10,0xff12,0xff13,0xff15,0xff17,0xff18,
0xff1a,0xff1c,0xff1e,0xff21,0xff23,0xff25,0xff27,0xff2a,
0xff2c,0xff2f,0xff31,0xff34,0xff37,0xff3a,0xff3c,0xff3f,
0xff42,0xff45,0xff48,0xff4b,0xff4f,0xff52,0xff55,0xff59,
0xff5c,0xff5f,0xff63,0xff66,0xff6a,0xff6e,0xff71,0xff75,
0xff79,0xff7d,0xff80,0xff84,0xff88,0xff8c,0xff90,0xff94,
0xff98,0xff9c,0xffa1,0xffa5,0xffa9,0xffad,0xffb1,0xffb6,
0xffba,0xffbe,0xffc3,0xffc7,0xffcb,0xffd0,0xffd4,0xffd8,
0xffdd,0xffe1,0xffe6,0xffea,0xffef,0xfff3,0xfff8,0xfffc
};

const fixed_p fp_cos[360] = {
0x0100,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00fe,0x00fe,
0x00fd,0x00fc,0x00fc,0x00fb,0x00fa,0x00f9,0x00f8,0x00f7,
0x00f6,0x00f4,0x00f3,0x00f2,0x00f0,0x00ee,0x00ed,0x00eb,
0x00e9,0x00e8,0x00e6,0x00e4,0x00e2,0x00df,0x00dd,0x00db,
0x00d9,0x00d6,0x00d4,0x00d1,0x00cf,0x00cc,0x00c9,0x00c6,
0x00c4,0x00c1,0x00be,0x00bb,0x00b8,0x00b5,0x00b1,0x00ae,
0x00ab,0x00a7,0x00a4,0x00a1,0x009d,0x009a,0x0096,0x0092,
0x008f,0x008b,0x0087,0x0083,0x0080,0x007c,0x0078,0x0074,
0x0070,0x006c,0x0068,0x0064,0x005f,0x005b,0x0057,0x0053,
0x004f,0x004a,0x0046,0x0042,0x003d,0x0039,0x0035,0x0030,
0x002c,0x0028,0x0023,0x001f,0x001a,0x0016,0x0011,0x000d,
0x0008,0x0004,0x0000,0xfffc,0xfff8,0xfff3,0xffef,0xffea,
0xffe6,0xffe1,0xffdd,0xffd8,0xffd4,0xffd0,0xffcb,0xffc7,
0xffc3,0xffbe,0xffba,0xffb6,0xffb1,0xffad,0xffa9,0xffa5,
0xffa1,0xff9c,0xff98,0xff94,0xff90,0xff8c,0xff88,0xff84,
0xff81,0xff7d,0xff79,0xff75,0xff71,0xff6e,0xff6a,0xff66,
0xff63,0xff5f,0xff5c,0xff59,0xff55,0xff52,0xff4f,0xff4b,
0xff48,0xff45,0xff42,0xff3f,0xff3c,0xff3a,0xff37,0xff34,
0xff31,0xff2f,0xff2c,0xff2a,0xff27,0xff25,0xff23,0xff21,
0xff1e,0xff1c,0xff1a,0xff18,0xff17,0xff15,0xff13,0xff12,
0xff10,0xff0e,0xff0d,0xff0c,0xff0a,0xff09,0xff08,0xff07,
0xff06,0xff05,0xff04,0xff04,0xff03,0xff02,0xff02,0xff01,
0xff01,0xff01,0xff01,0xff01,0xff00,0xff01,0xff01,0xff01,
0xff01,0xff01,0xff02,0xff02,0xff03,0xff04,0xff04,0xff05,
0xff06,0xff07,0xff08,0xff09,0xff0a,0xff0c,0xff0d,0xff0e,
0xff10,0xff12,0xff13,0xff15,0xff17,0xff18,0xff1a,0xff1c,
0xff1e,0xff21,0xff23,0xff25,0xff27,0xff2a,0xff2c,0xff2f,
0xff31,0xff34,0xff37,0xff3a,0xff3c,0xff3f,0xff42,0xff45,
0xff48,0xff4b,0xff4f,0xff52,0xff55,0xff59,0xff5c,0xff5f,
0xff63,0xff66,0xff6a,0xff6e,0xff71,0xff75,0xff79,0xff7d,
0xff80,0xff84,0xff88,0xff8c,0xff90,0xff94,0xff98,0xff9c,
0xffa1,0xffa5,0xffa9,0xffad,0xffb1,0xffb6,0xffba,0xffbe,
0xffc3,0xffc7,0xffcb,0xffd0,0xffd4,0xffd8,0xffdd,0xffe1,
0xffe6,0xffea,0xffef,0xfff3,0xfff8,0xfffc,0x0000,0x0004,
0x0008,0x000d,0x0011,0x0016,0x001a,0x001f,0x0023,0x0028,
0x002c,0x0030,0x0035,0x0039,0x003d,0x0042,0x0046,0x004a,
0x004f,0x0053,0x0057,0x005b,0x005f,0x0064,0x0068,0x006c,
0x0070,0x0074,0x0078,0x007c,0x0080,0x0083,0x0087,0x008b,
0x008f,0x0092,0x0096,0x009a,0x009d,0x00a1,0x00a4,0x00a7,
0x00ab,0x00ae,0x00b1,0x00b5,0x00b8,0x00bb,0x00be,0x00c1,
0x00c4,0x00c6,0x00c9,0x00cc,0x00cf,0x00d1,0x00d4,0x00d6,
0x00d9,0x00db,0x00dd,0x00df,0x00e2,0x00e4,0x00e6,0x00e8,
0x00e9,0x00eb,0x00ed,0x00ee,0x00f0,0x00f2,0x00f3,0x00f4,
0x00f6,0x00f7,0x00f8,0x00f9,0x00fa,0x00fb,0x00fc,0x00fc,
0x00fd,0x00fe,0x00fe,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff
};

