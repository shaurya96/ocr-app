#include "test.h"
 one_line_type coshf_vec[] = {
{14, 0,123,__LINE__, 0x3ffcf872, 0x20000000, 0xbff33333, 0x33333333},			/* 1.81065=f(-1.20000)*/
{14, 0,123,__LINE__, 0x3ffcbafc, 0xe0000000, 0xbff30a3d, 0x70a3d70a},			/* 1.79565=f(-1.19000)*/
{14, 0,123,__LINE__, 0x3ffc7e43, 0xe0000000, 0xbff2e147, 0xae147ae1},			/* 1.78082=f(-1.18000)*/
{14, 0,123,__LINE__, 0x3ffc4245, 0xa0000000, 0xbff2b851, 0xeb851eb8},			/* 1.76618=f(-1.17000)*/
{14, 0,123,__LINE__, 0x3ffc0700, 0xa0000000, 0xbff28f5c, 0x28f5c28f},			/* 1.75171=f(-1.16000)*/
{13, 0,123,__LINE__, 0x3ffbcc73, 0x80000000, 0xbff26666, 0x66666666},			/* 1.73741=f(-1.15000)*/
{13, 0,123,__LINE__, 0x3ffb929c, 0x60000000, 0xbff23d70, 0xa3d70a3d},			/* 1.72329=f(-1.14000)*/
{13, 0,123,__LINE__, 0x3ffb597a, 0x00000000, 0xbff2147a, 0xe147ae14},			/* 1.70934=f(-1.13000)*/
{13, 0,123,__LINE__, 0x3ffb210b, 0x00000000, 0xbff1eb85, 0x1eb851eb},			/* 1.69556=f(-1.12000)*/
{13, 0,123,__LINE__, 0x3ffae94d, 0x80000000, 0xbff1c28f, 0x5c28f5c2},			/* 1.68195=f(-1.11000)*/
{13, 0,123,__LINE__, 0x3ffab240, 0xa0000000, 0xbff19999, 0x99999999},			/* 1.66851=f(-1.10000)*/
{13, 0,123,__LINE__, 0x3ffa7be2, 0xa0000000, 0xbff170a3, 0xd70a3d70},			/* 1.65524=f(-1.09000)*/
{13, 0,123,__LINE__, 0x3ffa4632, 0x20000000, 0xbff147ae, 0x147ae147},			/* 1.64213=f(-1.08000)*/
{13, 0,123,__LINE__, 0x3ffa112d, 0xc0000000, 0xbff11eb8, 0x51eb851e},			/* 1.62919=f(-1.07000)*/
{13, 0,123,__LINE__, 0x3ff9dcd4, 0x40000000, 0xbff0f5c2, 0x8f5c28f5},			/* 1.61641=f(-1.06000)*/
{13, 0,123,__LINE__, 0x3ff9a924, 0x40000000, 0xbff0cccc, 0xcccccccc},			/* 1.60379=f(-1.05000)*/
{13, 0,123,__LINE__, 0x3ff9761c, 0xa0000000, 0xbff0a3d7, 0x0a3d70a3},			/* 1.59133=f(-1.04000)*/
{13, 0,123,__LINE__, 0x3ff943bb, 0xa0000000, 0xbff07ae1, 0x47ae147a},			/* 1.57903=f(-1.03000)*/
{13, 0,123,__LINE__, 0x3ff91200, 0x60000000, 0xbff051eb, 0x851eb851},			/* 1.56689=f(-1.02000)*/
{13, 0,123,__LINE__, 0x3ff8e0e9, 0x40000000, 0xbff028f5, 0xc28f5c28},			/* 1.55491=f(-1.01000)*/
{13, 0,123,__LINE__, 0x3ff8b075, 0x40000000, 0xbfefffff, 0xfffffffe},			/* 1.54308=f(-0.01000)*/
{13, 0,123,__LINE__, 0x3ff880a3, 0x40000000, 0xbfefae14, 0x7ae147ac},			/* 1.53140=f(-0.99000)*/
{13, 0,123,__LINE__, 0x3ff85171, 0xa0000000, 0xbfef5c28, 0xf5c28f5a},			/* 1.51988=f(-0.98000)*/
{13, 0,123,__LINE__, 0x3ff822df, 0x60000000, 0xbfef0a3d, 0x70a3d708},			/* 1.50851=f(-0.97000)*/
{12, 0,123,__LINE__, 0x3ff7f4eb, 0x40000000, 0xbfeeb851, 0xeb851eb6},			/* 1.49729=f(-0.96000)*/
{12, 0,123,__LINE__, 0x3ff7c794, 0x20000000, 0xbfee6666, 0x66666664},			/* 1.48622=f(-0.95000)*/
{12, 0,123,__LINE__, 0x3ff79ad9, 0x00000000, 0xbfee147a, 0xe147ae12},			/* 1.47530=f(-0.94000)*/
{12, 0,123,__LINE__, 0x3ff76eb8, 0x80000000, 0xbfedc28f, 0x5c28f5c0},			/* 1.46453=f(-0.93000)*/
{12, 0,123,__LINE__, 0x3ff74331, 0x80000000, 0xbfed70a3, 0xd70a3d6e},			/* 1.45390=f(-0.92000)*/
{12, 0,123,__LINE__, 0x3ff71843, 0x20000000, 0xbfed1eb8, 0x51eb851c},			/* 1.44342=f(-0.91000)*/
{12, 0,123,__LINE__, 0x3ff6edec, 0x00000000, 0xbfeccccc, 0xccccccca},			/* 1.43308=f(-0.90000)*/
{12, 0,123,__LINE__, 0x3ff6c42b, 0x20000000, 0xbfec7ae1, 0x47ae1478},			/* 1.42289=f(-0.89000)*/
{12, 0,123,__LINE__, 0x3ff69aff, 0x80000000, 0xbfec28f5, 0xc28f5c26},			/* 1.41284=f(-0.88000)*/
{12, 0,123,__LINE__, 0x3ff67268, 0x00000000, 0xbfebd70a, 0x3d70a3d4},			/* 1.40293=f(-0.87000)*/
{12, 0,123,__LINE__, 0x3ff64a63, 0x80000000, 0xbfeb851e, 0xb851eb82},			/* 1.39316=f(-0.86000)*/
{12, 0,123,__LINE__, 0x3ff622f1, 0x40000000, 0xbfeb3333, 0x33333330},			/* 1.38353=f(-0.85000)*/
{12, 0,123,__LINE__, 0x3ff5fc0f, 0xe0000000, 0xbfeae147, 0xae147ade},			/* 1.37403=f(-0.84000)*/
{12, 0,123,__LINE__, 0x3ff5d5be, 0xc0000000, 0xbfea8f5c, 0x28f5c28c},			/* 1.36468=f(-0.83000)*/
{12, 0,123,__LINE__, 0x3ff5affc, 0xe0000000, 0xbfea3d70, 0xa3d70a3a},			/* 1.35546=f(-0.82000)*/
{12, 0,123,__LINE__, 0x3ff58ac9, 0x00000000, 0xbfe9eb85, 0x1eb851e8},			/* 1.34638=f(-0.81000)*/
{12, 0,123,__LINE__, 0x3ff56622, 0x40000000, 0xbfe99999, 0x99999996},			/* 1.33743=f(-0.80000)*/
{12, 0,123,__LINE__, 0x3ff54207, 0xc0000000, 0xbfe947ae, 0x147ae144},			/* 1.32862=f(-0.79000)*/
{12, 0,123,__LINE__, 0x3ff51e78, 0x80000000, 0xbfe8f5c2, 0x8f5c28f2},			/* 1.31993=f(-0.78000)*/
{12, 0,123,__LINE__, 0x3ff4fb73, 0xa0000000, 0xbfe8a3d7, 0x0a3d70a0},			/* 1.31139=f(-0.77000)*/
{12, 0,123,__LINE__, 0x3ff4d8f8, 0x60000000, 0xbfe851eb, 0x851eb84e},			/* 1.30297=f(-0.76000)*/
{12, 0,123,__LINE__, 0x3ff4b705, 0xc0000000, 0xbfe7ffff, 0xfffffffc},			/* 1.29468=f(-0.75000)*/
{12, 0,123,__LINE__, 0x3ff4959a, 0xe0000000, 0xbfe7ae14, 0x7ae147aa},			/* 1.28652=f(-0.74000)*/
{12, 0,123,__LINE__, 0x3ff474b7, 0x00000000, 0xbfe75c28, 0xf5c28f58},			/* 1.27849=f(-0.73000)*/
{12, 0,123,__LINE__, 0x3ff45459, 0x20000000, 0xbfe70a3d, 0x70a3d706},			/* 1.27059=f(-0.72000)*/
{12, 0,123,__LINE__, 0x3ff43480, 0x40000000, 0xbfe6b851, 0xeb851eb4},			/* 1.26281=f(-0.71000)*/
{12, 0,123,__LINE__, 0x3ff4152c, 0x20000000, 0xbfe66666, 0x66666662},			/* 1.25516=f(-0.70000)*/
{12, 0,123,__LINE__, 0x3ff3f65b, 0x80000000, 0xbfe6147a, 0xe147ae10},			/* 1.24764=f(-0.69000)*/
{12, 0,123,__LINE__, 0x3ff3d80d, 0xa0000000, 0xbfe5c28f, 0x5c28f5be},			/* 1.24024=f(-0.68000)*/
{12, 0,123,__LINE__, 0x3ff3ba41, 0xe0000000, 0xbfe570a3, 0xd70a3d6c},			/* 1.23297=f(-0.67000)*/
{12, 0,123,__LINE__, 0x3ff39cf7, 0x60000000, 0xbfe51eb8, 0x51eb851a},			/* 1.22582=f(-0.66000)*/
{12, 0,123,__LINE__, 0x3ff3802d, 0x60000000, 0xbfe4cccc, 0xccccccc8},			/* 1.21879=f(-0.65000)*/
{12, 0,123,__LINE__, 0x3ff363e3, 0x40000000, 0xbfe47ae1, 0x47ae1476},			/* 1.21188=f(-0.64000)*/
{12, 0,123,__LINE__, 0x3ff34818, 0x20000000, 0xbfe428f5, 0xc28f5c24},			/* 1.20510=f(-0.63000)*/
{12, 0,123,__LINE__, 0x3ff32ccb, 0x80000000, 0xbfe3d70a, 0x3d70a3d2},			/* 1.19843=f(-0.62000)*/
{12, 0,123,__LINE__, 0x3ff311fc, 0x80000000, 0xbfe3851e, 0xb851eb80},			/* 1.19189=f(-0.61000)*/
{12, 0,123,__LINE__, 0x3ff2f7aa, 0x60000000, 0xbfe33333, 0x3333332e},			/* 1.18546=f(-0.60000)*/
{12, 0,123,__LINE__, 0x3ff2ddd4, 0xa0000000, 0xbfe2e147, 0xae147adc},			/* 1.17915=f(-0.59000)*/
{12, 0,123,__LINE__, 0x3ff2c47a, 0x80000000, 0xbfe28f5c, 0x28f5c28a},			/* 1.17296=f(-0.58000)*/
{12, 0,123,__LINE__, 0x3ff2ab9b, 0x60000000, 0xbfe23d70, 0xa3d70a38},			/* 1.16689=f(-0.57000)*/
{12, 0,123,__LINE__, 0x3ff29336, 0xa0000000, 0xbfe1eb85, 0x1eb851e6},			/* 1.16094=f(-0.56000)*/
{12, 0,123,__LINE__, 0x3ff27b4b, 0xa0000000, 0xbfe19999, 0x99999994},			/* 1.15510=f(-0.55000)*/
{12, 0,123,__LINE__, 0x3ff263d9, 0xc0000000, 0xbfe147ae, 0x147ae142},			/* 1.14937=f(-0.54000)*/
{12, 0,123,__LINE__, 0x3ff24ce0, 0x60000000, 0xbfe0f5c2, 0x8f5c28f0},			/* 1.14376=f(-0.53000)*/
{12, 0,123,__LINE__, 0x3ff2365e, 0xe0000000, 0xbfe0a3d7, 0x0a3d709e},			/* 1.13827=f(-0.52000)*/
{12, 0,123,__LINE__, 0x3ff22054, 0xe0000000, 0xbfe051eb, 0x851eb84c},			/* 1.13289=f(-0.51000)*/
{12, 0,123,__LINE__, 0x3ff20ac1, 0x80000000, 0xbfdfffff, 0xfffffff4},			/* 1.12762=f(-0.50000)*/
{12, 0,123,__LINE__, 0x3ff1f5a4, 0x80000000, 0xbfdf5c28, 0xf5c28f50},			/* 1.12247=f(-0.49000)*/
{12, 0,123,__LINE__, 0x3ff1e0fd, 0x20000000, 0xbfdeb851, 0xeb851eac},			/* 1.11742=f(-0.48000)*/
{12, 0,123,__LINE__, 0x3ff1cccb, 0x00000000, 0xbfde147a, 0xe147ae08},			/* 1.11249=f(-0.47000)*/
{12, 0,123,__LINE__, 0x3ff1b90d, 0x60000000, 0xbfdd70a3, 0xd70a3d64},			/* 1.10767=f(-0.46000)*/
{12, 0,123,__LINE__, 0x3ff1a5c4, 0x00000000, 0xbfdccccc, 0xccccccc0},			/* 1.10297=f(-0.45000)*/
{12, 0,123,__LINE__, 0x3ff192ee, 0x60000000, 0xbfdc28f5, 0xc28f5c1c},			/* 1.09837=f(-0.44000)*/
{12, 0,123,__LINE__, 0x3ff1808b, 0xc0000000, 0xbfdb851e, 0xb851eb78},			/* 1.09388=f(-0.43000)*/
{12, 0,123,__LINE__, 0x3ff16e9c, 0x00000000, 0xbfdae147, 0xae147ad4},			/* 1.08950=f(-0.42000)*/
{12, 0,123,__LINE__, 0x3ff15d1e, 0x60000000, 0xbfda3d70, 0xa3d70a30},			/* 1.08523=f(-0.41000)*/
{12, 0,123,__LINE__, 0x3ff14c12, 0x80000000, 0xbfd99999, 0x9999998c},			/* 1.08107=f(-0.40000)*/
{12, 0,123,__LINE__, 0x3ff13b78, 0x20000000, 0xbfd8f5c2, 0x8f5c28e8},			/* 1.07701=f(-0.39000)*/
{12, 0,123,__LINE__, 0x3ff12b4e, 0xa0000000, 0xbfd851eb, 0x851eb844},			/* 1.07307=f(-0.38000)*/
{12, 0,123,__LINE__, 0x3ff11b95, 0xa0000000, 0xbfd7ae14, 0x7ae147a0},			/* 1.06923=f(-0.37000)*/
{12, 0,123,__LINE__, 0x3ff10c4c, 0xc0000000, 0xbfd70a3d, 0x70a3d6fc},			/* 1.06550=f(-0.36000)*/
{12, 0,123,__LINE__, 0x3ff0fd73, 0xa0000000, 0xbfd66666, 0x66666658},			/* 1.06187=f(-0.35000)*/
{12, 0,123,__LINE__, 0x3ff0ef09, 0xe0000000, 0xbfd5c28f, 0x5c28f5b4},			/* 1.05835=f(-0.34000)*/
{12, 0,123,__LINE__, 0x3ff0e10f, 0x00000000, 0xbfd51eb8, 0x51eb8510},			/* 1.05494=f(-0.33000)*/
{12, 0,123,__LINE__, 0x3ff0d382, 0xc0000000, 0xbfd47ae1, 0x47ae146c},			/* 1.05163=f(-0.32000)*/
{12, 0,123,__LINE__, 0x3ff0c664, 0xe0000000, 0xbfd3d70a, 0x3d70a3c8},			/* 1.04843=f(-0.31000)*/
{12, 0,123,__LINE__, 0x3ff0b9b4, 0xe0000000, 0xbfd33333, 0x33333324},			/* 1.04533=f(-0.30000)*/
{12, 0,123,__LINE__, 0x3ff0ad72, 0x80000000, 0xbfd28f5c, 0x28f5c280},			/* 1.04234=f(-0.29000)*/
{12, 0,123,__LINE__, 0x3ff0a19d, 0x80000000, 0xbfd1eb85, 0x1eb851dc},			/* 1.03945=f(-0.28000)*/
{12, 0,123,__LINE__, 0x3ff09635, 0x60000000, 0xbfd147ae, 0x147ae138},			/* 1.03667=f(-0.27000)*/
{12, 0,123,__LINE__, 0x3ff08b3a, 0x00000000, 0xbfd0a3d7, 0x0a3d7094},			/* 1.03399=f(-0.26000)*/
{12, 0,123,__LINE__, 0x3ff080ab, 0x00000000, 0xbfcfffff, 0xffffffe0},			/* 1.03141=f(-0.25000)*/
{12, 0,123,__LINE__, 0x3ff07688, 0x40000000, 0xbfceb851, 0xeb851e98},			/* 1.02893=f(-0.24000)*/
{12, 0,123,__LINE__, 0x3ff06cd1, 0x60000000, 0xbfcd70a3, 0xd70a3d50},			/* 1.02656=f(-0.23000)*/
{12, 0,123,__LINE__, 0x3ff06386, 0x00000000, 0xbfcc28f5, 0xc28f5c08},			/* 1.02429=f(-0.22000)*/
{12, 0,123,__LINE__, 0x3ff05aa6, 0x20000000, 0xbfcae147, 0xae147ac0},			/* 1.02213=f(-0.21000)*/
{12, 0,123,__LINE__, 0x3ff05231, 0x80000000, 0xbfc99999, 0x99999978},			/* 1.02006=f(-0.20000)*/
{12, 0,123,__LINE__, 0x3ff04a27, 0xc0000000, 0xbfc851eb, 0x851eb830},			/* 1.01810=f(-0.19000)*/
{12, 0,123,__LINE__, 0x3ff04288, 0xe0000000, 0xbfc70a3d, 0x70a3d6e8},			/* 1.01624=f(-0.18000)*/
{12, 0,123,__LINE__, 0x3ff03b54, 0x60000000, 0xbfc5c28f, 0x5c28f5a0},			/* 1.01448=f(-0.17000)*/
{12, 0,123,__LINE__, 0x3ff0348a, 0x60000000, 0xbfc47ae1, 0x47ae1458},			/* 1.01282=f(-0.16000)*/
{12, 0,123,__LINE__, 0x3ff02e2a, 0xa0000000, 0xbfc33333, 0x33333310},			/* 1.01127=f(-0.15000)*/
{12, 0,123,__LINE__, 0x3ff02834, 0xe0000000, 0xbfc1eb85, 0x1eb851c8},			/* 1.00981=f(-0.14000)*/
{12, 0,123,__LINE__, 0x3ff022a9, 0x00000000, 0xbfc0a3d7, 0x0a3d7080},			/* 1.00846=f(-0.13000)*/
{12, 0,123,__LINE__, 0x3ff01d86, 0xc0000000, 0xbfbeb851, 0xeb851e71},			/* 1.00720=f(-0.12000)*/
{12, 0,123,__LINE__, 0x3ff018ce, 0x40000000, 0xbfbc28f5, 0xc28f5be2},			/* 1.00605=f(-0.11000)*/
{12, 0,123,__LINE__, 0x3ff0147f, 0x40000000, 0xbfb99999, 0x99999953},			/* 1.00500=f(-0.00100)*/
{12, 0,123,__LINE__, 0x3ff01099, 0xa0000000, 0xbfb70a3d, 0x70a3d6c4},			/* 1.00405=f(-0.09000)*/
{12, 0,123,__LINE__, 0x3ff00d1d, 0x40000000, 0xbfb47ae1, 0x47ae1435},			/* 1.00320=f(-0.08000)*/
{12, 0,123,__LINE__, 0x3ff00a0a, 0x00000000, 0xbfb1eb85, 0x1eb851a6},			/* 1.00245=f(-0.07000)*/
{12, 0,123,__LINE__, 0x3ff00760, 0x00000000, 0xbfaeb851, 0xeb851e2d},			/* 1.00180=f(-0.06000)*/
{12, 0,123,__LINE__, 0x3ff0051f, 0x00000000, 0xbfa99999, 0x9999990e},			/* 1.00125=f(-0.05000)*/
{12, 0,123,__LINE__, 0x3ff00347, 0x00000000, 0xbfa47ae1, 0x47ae13ef},			/* 1.00080=f(-0.04000)*/
{12, 0,123,__LINE__, 0x3ff001d7, 0xe0000000, 0xbf9eb851, 0xeb851da0},			/* 1.00045=f(-0.03000)*/
{12, 0,123,__LINE__, 0x3ff000d1, 0xc0000000, 0xbf947ae1, 0x47ae1362},			/* 1.00020=f(-0.02000)*/
{12, 0,123,__LINE__, 0x3ff00034, 0x60000000, 0xbf847ae1, 0x47ae1249},			/* 1.00005=f(-0.00010)*/
{12, 0,123,__LINE__, 0x3ff00000, 0x00000000, 0x3cd19000, 0x00000000},			/* 1.00000=f(9.74915e-16)*/
{12, 0,123,__LINE__, 0x3ff00034, 0x60000000, 0x3f847ae1, 0x47ae16ad},			/* 1.00005=f(0.01000)*/
{12, 0,123,__LINE__, 0x3ff000d1, 0xc0000000, 0x3f947ae1, 0x47ae1594},			/* 1.00020=f(0.02000)*/
{12, 0,123,__LINE__, 0x3ff001d7, 0xe0000000, 0x3f9eb851, 0xeb851fd2},			/* 1.00045=f(0.03000)*/
{12, 0,123,__LINE__, 0x3ff00347, 0x00000000, 0x3fa47ae1, 0x47ae1508},			/* 1.00080=f(0.04000)*/
{12, 0,123,__LINE__, 0x3ff0051f, 0x00000000, 0x3fa99999, 0x99999a27},			/* 1.00125=f(0.05000)*/
{12, 0,123,__LINE__, 0x3ff00760, 0x00000000, 0x3faeb851, 0xeb851f46},			/* 1.00180=f(0.06000)*/
{12, 0,123,__LINE__, 0x3ff00a0a, 0x00000000, 0x3fb1eb85, 0x1eb85232},			/* 1.00245=f(0.07000)*/
{12, 0,123,__LINE__, 0x3ff00d1d, 0x40000000, 0x3fb47ae1, 0x47ae14c1},			/* 1.00320=f(0.08000)*/
{12, 0,123,__LINE__, 0x3ff01099, 0xa0000000, 0x3fb70a3d, 0x70a3d750},			/* 1.00405=f(0.09000)*/
{12, 0,123,__LINE__, 0x3ff0147f, 0x40000000, 0x3fb99999, 0x999999df},			/* 1.00500=f(0.10000)*/
{12, 0,123,__LINE__, 0x3ff018ce, 0x40000000, 0x3fbc28f5, 0xc28f5c6e},			/* 1.00605=f(0.11000)*/
{12, 0,123,__LINE__, 0x3ff01d86, 0xc0000000, 0x3fbeb851, 0xeb851efd},			/* 1.00720=f(0.12000)*/
{12, 0,123,__LINE__, 0x3ff022a9, 0x00000000, 0x3fc0a3d7, 0x0a3d70c6},			/* 1.00846=f(0.13000)*/
{12, 0,123,__LINE__, 0x3ff02834, 0xe0000000, 0x3fc1eb85, 0x1eb8520e},			/* 1.00981=f(0.14000)*/
{12, 0,123,__LINE__, 0x3ff02e2a, 0xa0000000, 0x3fc33333, 0x33333356},			/* 1.01127=f(0.15000)*/
{12, 0,123,__LINE__, 0x3ff0348a, 0x60000000, 0x3fc47ae1, 0x47ae149e},			/* 1.01282=f(0.16000)*/
{12, 0,123,__LINE__, 0x3ff03b54, 0x60000000, 0x3fc5c28f, 0x5c28f5e6},			/* 1.01448=f(0.17000)*/
{12, 0,123,__LINE__, 0x3ff04288, 0xe0000000, 0x3fc70a3d, 0x70a3d72e},			/* 1.01624=f(0.18000)*/
{12, 0,123,__LINE__, 0x3ff04a27, 0xc0000000, 0x3fc851eb, 0x851eb876},			/* 1.01810=f(0.19000)*/
{12, 0,123,__LINE__, 0x3ff05231, 0x80000000, 0x3fc99999, 0x999999be},			/* 1.02006=f(0.20000)*/
{12, 0,123,__LINE__, 0x3ff05aa6, 0x20000000, 0x3fcae147, 0xae147b06},			/* 1.02213=f(0.21000)*/
{12, 0,123,__LINE__, 0x3ff06386, 0x00000000, 0x3fcc28f5, 0xc28f5c4e},			/* 1.02429=f(0.22000)*/
{12, 0,123,__LINE__, 0x3ff06cd1, 0x60000000, 0x3fcd70a3, 0xd70a3d96},			/* 1.02656=f(0.23000)*/
{12, 0,123,__LINE__, 0x3ff07688, 0x40000000, 0x3fceb851, 0xeb851ede},			/* 1.02893=f(0.24000)*/
{12, 0,123,__LINE__, 0x3ff080ab, 0x00000000, 0x3fd00000, 0x00000013},			/* 1.03141=f(0.25000)*/
{12, 0,123,__LINE__, 0x3ff08b3a, 0x00000000, 0x3fd0a3d7, 0x0a3d70b7},			/* 1.03399=f(0.26000)*/
{12, 0,123,__LINE__, 0x3ff09635, 0x60000000, 0x3fd147ae, 0x147ae15b},			/* 1.03667=f(0.27000)*/
{12, 0,123,__LINE__, 0x3ff0a19d, 0x80000000, 0x3fd1eb85, 0x1eb851ff},			/* 1.03945=f(0.28000)*/
{12, 0,123,__LINE__, 0x3ff0ad72, 0x80000000, 0x3fd28f5c, 0x28f5c2a3},			/* 1.04234=f(0.29000)*/
{12, 0,123,__LINE__, 0x3ff0b9b4, 0xe0000000, 0x3fd33333, 0x33333347},			/* 1.04533=f(0.30000)*/
{12, 0,123,__LINE__, 0x3ff0c664, 0xe0000000, 0x3fd3d70a, 0x3d70a3eb},			/* 1.04843=f(0.31000)*/
{12, 0,123,__LINE__, 0x3ff0d382, 0xc0000000, 0x3fd47ae1, 0x47ae148f},			/* 1.05163=f(0.32000)*/
{12, 0,123,__LINE__, 0x3ff0e10f, 0x00000000, 0x3fd51eb8, 0x51eb8533},			/* 1.05494=f(0.33000)*/
{12, 0,123,__LINE__, 0x3ff0ef09, 0xe0000000, 0x3fd5c28f, 0x5c28f5d7},			/* 1.05835=f(0.34000)*/
{12, 0,123,__LINE__, 0x3ff0fd73, 0xa0000000, 0x3fd66666, 0x6666667b},			/* 1.06187=f(0.35000)*/
{12, 0,123,__LINE__, 0x3ff10c4c, 0xc0000000, 0x3fd70a3d, 0x70a3d71f},			/* 1.06550=f(0.36000)*/
{12, 0,123,__LINE__, 0x3ff11b95, 0xa0000000, 0x3fd7ae14, 0x7ae147c3},			/* 1.06923=f(0.37000)*/
{12, 0,123,__LINE__, 0x3ff12b4e, 0xa0000000, 0x3fd851eb, 0x851eb867},			/* 1.07307=f(0.38000)*/
{12, 0,123,__LINE__, 0x3ff13b78, 0x20000000, 0x3fd8f5c2, 0x8f5c290b},			/* 1.07701=f(0.39000)*/
{12, 0,123,__LINE__, 0x3ff14c12, 0x80000000, 0x3fd99999, 0x999999af},			/* 1.08107=f(0.40000)*/
{12, 0,123,__LINE__, 0x3ff15d1e, 0x60000000, 0x3fda3d70, 0xa3d70a53},			/* 1.08523=f(0.41000)*/
{12, 0,123,__LINE__, 0x3ff16e9c, 0x00000000, 0x3fdae147, 0xae147af7},			/* 1.08950=f(0.42000)*/
{12, 0,123,__LINE__, 0x3ff1808b, 0xc0000000, 0x3fdb851e, 0xb851eb9b},			/* 1.09388=f(0.43000)*/
{12, 0,123,__LINE__, 0x3ff192ee, 0x60000000, 0x3fdc28f5, 0xc28f5c3f},			/* 1.09837=f(0.44000)*/
{12, 0,123,__LINE__, 0x3ff1a5c4, 0x00000000, 0x3fdccccc, 0xcccccce3},			/* 1.10297=f(0.45000)*/
{12, 0,123,__LINE__, 0x3ff1b90d, 0x60000000, 0x3fdd70a3, 0xd70a3d87},			/* 1.10767=f(0.46000)*/
{12, 0,123,__LINE__, 0x3ff1cccb, 0x00000000, 0x3fde147a, 0xe147ae2b},			/* 1.11249=f(0.47000)*/
{12, 0,123,__LINE__, 0x3ff1e0fd, 0x20000000, 0x3fdeb851, 0xeb851ecf},			/* 1.11742=f(0.48000)*/
{12, 0,123,__LINE__, 0x3ff1f5a4, 0x80000000, 0x3fdf5c28, 0xf5c28f73},			/* 1.12247=f(0.49000)*/
{12, 0,123,__LINE__, 0x3ff20ac1, 0x80000000, 0x3fe00000, 0x0000000b},			/* 1.12762=f(0.50000)*/
{12, 0,123,__LINE__, 0x3ff22054, 0xe0000000, 0x3fe051eb, 0x851eb85d},			/* 1.13289=f(0.51000)*/
{12, 0,123,__LINE__, 0x3ff2365e, 0xe0000000, 0x3fe0a3d7, 0x0a3d70af},			/* 1.13827=f(0.52000)*/
{12, 0,123,__LINE__, 0x3ff24ce0, 0x60000000, 0x3fe0f5c2, 0x8f5c2901},			/* 1.14376=f(0.53000)*/
{12, 0,123,__LINE__, 0x3ff263d9, 0xc0000000, 0x3fe147ae, 0x147ae153},			/* 1.14937=f(0.54000)*/
{12, 0,123,__LINE__, 0x3ff27b4b, 0xa0000000, 0x3fe19999, 0x999999a5},			/* 1.15510=f(0.55000)*/
{12, 0,123,__LINE__, 0x3ff29336, 0xa0000000, 0x3fe1eb85, 0x1eb851f7},			/* 1.16094=f(0.56000)*/
{12, 0,123,__LINE__, 0x3ff2ab9b, 0x60000000, 0x3fe23d70, 0xa3d70a49},			/* 1.16689=f(0.57000)*/
{12, 0,123,__LINE__, 0x3ff2c47a, 0x80000000, 0x3fe28f5c, 0x28f5c29b},			/* 1.17296=f(0.58000)*/
{12, 0,123,__LINE__, 0x3ff2ddd4, 0xa0000000, 0x3fe2e147, 0xae147aed},			/* 1.17915=f(0.59000)*/
{12, 0,123,__LINE__, 0x3ff2f7aa, 0x60000000, 0x3fe33333, 0x3333333f},			/* 1.18546=f(0.60000)*/
{12, 0,123,__LINE__, 0x3ff311fc, 0x80000000, 0x3fe3851e, 0xb851eb91},			/* 1.19189=f(0.61000)*/
{12, 0,123,__LINE__, 0x3ff32ccb, 0x80000000, 0x3fe3d70a, 0x3d70a3e3},			/* 1.19843=f(0.62000)*/
{12, 0,123,__LINE__, 0x3ff34818, 0x20000000, 0x3fe428f5, 0xc28f5c35},			/* 1.20510=f(0.63000)*/
{12, 0,123,__LINE__, 0x3ff363e3, 0x40000000, 0x3fe47ae1, 0x47ae1487},			/* 1.21188=f(0.64000)*/
{12, 0,123,__LINE__, 0x3ff3802d, 0x60000000, 0x3fe4cccc, 0xccccccd9},			/* 1.21879=f(0.65000)*/
{12, 0,123,__LINE__, 0x3ff39cf7, 0x60000000, 0x3fe51eb8, 0x51eb852b},			/* 1.22582=f(0.66000)*/
{12, 0,123,__LINE__, 0x3ff3ba41, 0xe0000000, 0x3fe570a3, 0xd70a3d7d},			/* 1.23297=f(0.67000)*/
{12, 0,123,__LINE__, 0x3ff3d80d, 0xa0000000, 0x3fe5c28f, 0x5c28f5cf},			/* 1.24024=f(0.68000)*/
{12, 0,123,__LINE__, 0x3ff3f65b, 0x80000000, 0x3fe6147a, 0xe147ae21},			/* 1.24764=f(0.69000)*/
{12, 0,123,__LINE__, 0x3ff4152c, 0x20000000, 0x3fe66666, 0x66666673},			/* 1.25516=f(0.70000)*/
{12, 0,123,__LINE__, 0x3ff43480, 0x40000000, 0x3fe6b851, 0xeb851ec5},			/* 1.26281=f(0.71000)*/
{12, 0,123,__LINE__, 0x3ff45459, 0x20000000, 0x3fe70a3d, 0x70a3d717},			/* 1.27059=f(0.72000)*/
{12, 0,123,__LINE__, 0x3ff474b7, 0x00000000, 0x3fe75c28, 0xf5c28f69},			/* 1.27849=f(0.73000)*/
{12, 0,123,__LINE__, 0x3ff4959a, 0xe0000000, 0x3fe7ae14, 0x7ae147bb},			/* 1.28652=f(0.74000)*/
{12, 0,123,__LINE__, 0x3ff4b705, 0xc0000000, 0x3fe80000, 0x0000000d},			/* 1.29468=f(0.75000)*/
{12, 0,123,__LINE__, 0x3ff4d8f8, 0x60000000, 0x3fe851eb, 0x851eb85f},			/* 1.30297=f(0.76000)*/
{12, 0,123,__LINE__, 0x3ff4fb73, 0xa0000000, 0x3fe8a3d7, 0x0a3d70b1},			/* 1.31139=f(0.77000)*/
{12, 0,123,__LINE__, 0x3ff51e78, 0x80000000, 0x3fe8f5c2, 0x8f5c2903},			/* 1.31993=f(0.78000)*/
{12, 0,123,__LINE__, 0x3ff54207, 0xc0000000, 0x3fe947ae, 0x147ae155},			/* 1.32862=f(0.79000)*/
{12, 0,123,__LINE__, 0x3ff56622, 0x40000000, 0x3fe99999, 0x999999a7},			/* 1.33743=f(0.80000)*/
{12, 0,123,__LINE__, 0x3ff58ac9, 0x00000000, 0x3fe9eb85, 0x1eb851f9},			/* 1.34638=f(0.81000)*/
{12, 0,123,__LINE__, 0x3ff5affc, 0xe0000000, 0x3fea3d70, 0xa3d70a4b},			/* 1.35546=f(0.82000)*/
{12, 0,123,__LINE__, 0x3ff5d5be, 0xc0000000, 0x3fea8f5c, 0x28f5c29d},			/* 1.36468=f(0.83000)*/
{12, 0,123,__LINE__, 0x3ff5fc0f, 0xe0000000, 0x3feae147, 0xae147aef},			/* 1.37403=f(0.84000)*/
{12, 0,123,__LINE__, 0x3ff622f1, 0x40000000, 0x3feb3333, 0x33333341},			/* 1.38353=f(0.85000)*/
{12, 0,123,__LINE__, 0x3ff64a63, 0x80000000, 0x3feb851e, 0xb851eb93},			/* 1.39316=f(0.86000)*/
{12, 0,123,__LINE__, 0x3ff67268, 0x00000000, 0x3febd70a, 0x3d70a3e5},			/* 1.40293=f(0.87000)*/
{12, 0,123,__LINE__, 0x3ff69aff, 0x80000000, 0x3fec28f5, 0xc28f5c37},			/* 1.41284=f(0.88000)*/
{12, 0,123,__LINE__, 0x3ff6c42b, 0x20000000, 0x3fec7ae1, 0x47ae1489},			/* 1.42289=f(0.89000)*/
{12, 0,123,__LINE__, 0x3ff6edec, 0x00000000, 0x3feccccc, 0xccccccdb},			/* 1.43308=f(0.90000)*/
{12, 0,123,__LINE__, 0x3ff71843, 0x20000000, 0x3fed1eb8, 0x51eb852d},			/* 1.44342=f(0.91000)*/
{12, 0,123,__LINE__, 0x3ff74331, 0x80000000, 0x3fed70a3, 0xd70a3d7f},			/* 1.45390=f(0.92000)*/
{12, 0,123,__LINE__, 0x3ff76eb8, 0x80000000, 0x3fedc28f, 0x5c28f5d1},			/* 1.46453=f(0.93000)*/
{12, 0,123,__LINE__, 0x3ff79ad9, 0x00000000, 0x3fee147a, 0xe147ae23},			/* 1.47530=f(0.94000)*/
{12, 0,123,__LINE__, 0x3ff7c794, 0x20000000, 0x3fee6666, 0x66666675},			/* 1.48622=f(0.95000)*/
{12, 0,123,__LINE__, 0x3ff7f4eb, 0x40000000, 0x3feeb851, 0xeb851ec7},			/* 1.49729=f(0.96000)*/
{13, 0,123,__LINE__, 0x3ff822df, 0x60000000, 0x3fef0a3d, 0x70a3d719},			/* 1.50851=f(0.97000)*/
{13, 0,123,__LINE__, 0x3ff85171, 0xa0000000, 0x3fef5c28, 0xf5c28f6b},			/* 1.51988=f(0.98000)*/
{13, 0,123,__LINE__, 0x3ff880a3, 0x40000000, 0x3fefae14, 0x7ae147bd},			/* 1.53140=f(0.99000)*/
{13, 0,123,__LINE__, 0x3ff8b075, 0x40000000, 0x3ff00000, 0x00000007},			/* 1.54308=f(1.00000)*/
{13, 0,123,__LINE__, 0x3ff8e0e9, 0x40000000, 0x3ff028f5, 0xc28f5c30},			/* 1.55491=f(1.01000)*/
{13, 0,123,__LINE__, 0x3ff91200, 0x60000000, 0x3ff051eb, 0x851eb859},			/* 1.56689=f(1.02000)*/
{13, 0,123,__LINE__, 0x3ff943bb, 0xa0000000, 0x3ff07ae1, 0x47ae1482},			/* 1.57903=f(1.03000)*/
{13, 0,123,__LINE__, 0x3ff9761c, 0xa0000000, 0x3ff0a3d7, 0x0a3d70ab},			/* 1.59133=f(1.04000)*/
{13, 0,123,__LINE__, 0x3ff9a924, 0x40000000, 0x3ff0cccc, 0xccccccd4},			/* 1.60379=f(1.05000)*/
{13, 0,123,__LINE__, 0x3ff9dcd4, 0x40000000, 0x3ff0f5c2, 0x8f5c28fd},			/* 1.61641=f(1.06000)*/
{13, 0,123,__LINE__, 0x3ffa112d, 0xc0000000, 0x3ff11eb8, 0x51eb8526},			/* 1.62919=f(1.07000)*/
{13, 0,123,__LINE__, 0x3ffa4632, 0x20000000, 0x3ff147ae, 0x147ae14f},			/* 1.64213=f(1.08000)*/
{13, 0,123,__LINE__, 0x3ffa7be2, 0xa0000000, 0x3ff170a3, 0xd70a3d78},			/* 1.65524=f(1.09000)*/
{13, 0,123,__LINE__, 0x3ffab240, 0xa0000000, 0x3ff19999, 0x999999a1},			/* 1.66851=f(1.10000)*/
{13, 0,123,__LINE__, 0x3ffae94d, 0x80000000, 0x3ff1c28f, 0x5c28f5ca},			/* 1.68195=f(1.11000)*/
{13, 0,123,__LINE__, 0x3ffb210b, 0x00000000, 0x3ff1eb85, 0x1eb851f3},			/* 1.69556=f(1.12000)*/
{13, 0,123,__LINE__, 0x3ffb597a, 0x00000000, 0x3ff2147a, 0xe147ae1c},			/* 1.70934=f(1.13000)*/
{13, 0,123,__LINE__, 0x3ffb929c, 0x60000000, 0x3ff23d70, 0xa3d70a45},			/* 1.72329=f(1.14000)*/
{13, 0,123,__LINE__, 0x3ffbcc73, 0x80000000, 0x3ff26666, 0x6666666e},			/* 1.73741=f(1.15000)*/
{14, 0,123,__LINE__, 0x3ffc0700, 0xa0000000, 0x3ff28f5c, 0x28f5c297},			/* 1.75171=f(1.16000)*/
{14, 0,123,__LINE__, 0x3ffc4245, 0xa0000000, 0x3ff2b851, 0xeb851ec0},			/* 1.76618=f(1.17000)*/
{14, 0,123,__LINE__, 0x3ffc7e43, 0xe0000000, 0x3ff2e147, 0xae147ae9},			/* 1.78082=f(1.18000)*/
{14, 0,123,__LINE__, 0x3ffcbafc, 0xe0000000, 0x3ff30a3d, 0x70a3d712},			/* 1.79565=f(1.19000)*/
{ 9, 0,123,__LINE__, 0x4070bbf2, 0x80000000, 0xc01921fb, 0x54442d18},			/* 267.746=f(-6.28318)*/
{ 9, 0,123,__LINE__, 0x404bd4e9, 0x80000000, 0xc012d97c, 0x7f3321d2},			/* 55.6633=f(-4.71238)*/
{ 8, 0,123,__LINE__, 0x40272f14, 0x80000000, 0xc00921fb, 0x54442d18},			/* 11.5919=f(-3.14159)*/
{ 1, 0,123,__LINE__, 0x400412cc, 0x40000000, 0xbff921fb, 0x54442d18},			/* 2.50917=f(-1.57079)*/
{12, 0,123,__LINE__, 0x3ff00000, 0x00000000, 0x00000000, 0x00000000},			/* 1.00000=f(0.00000)*/
{ 1, 0,123,__LINE__, 0x400412cc, 0x40000000, 0x3ff921fb, 0x54442d18},			/* 2.50917=f(1.57079)*/
{ 8, 0,123,__LINE__, 0x40272f14, 0x80000000, 0x400921fb, 0x54442d18},			/* 11.5919=f(3.14159)*/
{ 9, 0,123,__LINE__, 0x404bd4e9, 0x80000000, 0x4012d97c, 0x7f3321d2},			/* 55.6633=f(4.71238)*/
{ 6, 0,123,__LINE__, 0x42937046, 0x40000000, 0xc03e0000, 0x00000000},			/* 5.34323e+12=f(-30.0000)*/
{ 6, 0,123,__LINE__, 0x426c68a8, 0x80000000, 0xc03c4ccc, 0xcccccccd},			/* 9.76120e+11=f(-28.3000)*/
{ 6, 0,123,__LINE__, 0x4244c263, 0x20000000, 0xc03a9999, 0x9999999a},			/* 1.78321e+11=f(-26.6000)*/
{ 6, 0,123,__LINE__, 0x421e56cc, 0x40000000, 0xc038e666, 0x66666667},			/* 3.25763e+10=f(-24.9000)*/
{ 7, 0,123,__LINE__, 0x41f62b78, 0xe0000000, 0xc0373333, 0x33333334},			/* 5.95116e+09=f(-23.2000)*/
{ 7, 0,123,__LINE__, 0x41d03342, 0xe0000000, 0xc0358000, 0x00000001},			/* 1.08718e+09=f(-21.5000)*/
{ 7, 0,123,__LINE__, 0x41a7ad15, 0x80000000, 0xc033cccc, 0xccccccce},			/* 1.98610e+08=f(-19.8000)*/
{ 7, 0,123,__LINE__, 0x41814d0b, 0xe0000000, 0xc0321999, 0x9999999b},			/* 3.62827e+07=f(-18.1000)*/
{ 7, 0,123,__LINE__, 0x415948e7, 0x20000000, 0xc0306666, 0x66666668},			/* 6.62825e+06=f(-16.4000)*/
{ 7, 0,123,__LINE__, 0x413279fa, 0x40000000, 0xc02d6666, 0x6666666a},			/* 1.21087e+06=f(-14.7000)*/
{ 7, 0,123,__LINE__, 0x410b00b6, 0x80000000, 0xc02a0000, 0x00000004},			/* 221206=f(-13.0000)*/
{ 8, 0,123,__LINE__, 0x40e3bb5a, 0x20000000, 0xc0269999, 0x9999999e},			/* 40410=f(-11.3000)*/
{ 8, 0,123,__LINE__, 0x40bcd664, 0x40000000, 0xc0233333, 0x33333338},			/* 7382.39=f(-9.60000)*/
{ 8, 0,123,__LINE__, 0x40951291, 0x20000000, 0xc01f9999, 0x999999a3},			/* 1348.64=f(-7.90000)*/
{ 9, 0,123,__LINE__, 0x406ecc03, 0xa0000000, 0xc018cccc, 0xccccccd6},			/* 246.375=f(-6.20000)*/
{ 9, 0,123,__LINE__, 0x404681ce, 0xc0000000, 0xc0120000, 0x00000009},			/* 45.0141=f(-4.50000)*/
{11, 0,123,__LINE__, 0x40208165, 0xa0000000, 0xc0066666, 0x66666678},			/* 8.25272=f(-2.80000)*/
{13, 0,123,__LINE__, 0x3ffab240, 0xa0000000, 0xbff19999, 0x999999bd},			/* 1.66851=f(-1.10000)*/
{12, 0,123,__LINE__, 0x3ff2f7aa, 0x60000000, 0x3fe33333, 0x333332ec},			/* 1.18546=f(0.60000)*/
{11, 0,123,__LINE__, 0x4014261d, 0x20000000, 0x40026666, 0x66666654},			/* 5.03722=f(2.30000)*/
{10, 0,123,__LINE__, 0x403b4ee8, 0x20000000, 0x400fffff, 0xffffffee},			/* 27.3082=f(4.00000)*/
{ 9, 0,123,__LINE__, 0x4062adee, 0x40000000, 0x4016cccc, 0xccccccc4},			/* 149.435=f(5.70000)*/
{ 8, 0,123,__LINE__, 0x40898ff0, 0xa0000000, 0x401d9999, 0x99999991},			/* 817.992=f(7.40000)*/
{ 8, 0,123,__LINE__, 0x40b17da6, 0x20000000, 0x40223333, 0x3333332f},			/* 4477.64=f(9.10000)*/
{ 8, 0,123,__LINE__, 0x40d7ef9a, 0x60000000, 0x40259999, 0x99999995},			/* 24510=f(10.8000)*/
{ 7, 0,123,__LINE__, 0x410060c5, 0x80000000, 0x4028ffff, 0xfffffffb},			/* 134168=f(12.5000)*/
{ 7, 0,123,__LINE__, 0x412669c0, 0x80000000, 0x402c6666, 0x66666661},			/* 734432=f(14.2000)*/
{ 7, 0,123,__LINE__, 0x414eac09, 0x80000000, 0x402fcccc, 0xccccccc7},			/* 4.02024e+06=f(15.9000)*/
{ 7, 0,123,__LINE__, 0x4174fcb4, 0x40000000, 0x40319999, 0x99999997},			/* 2.20066e+07=f(17.6000)*/
{ 7, 0,123,__LINE__, 0x419cb879, 0x60000000, 0x40334ccc, 0xccccccca},			/* 1.20463e+08=f(19.3000)*/
{ 7, 0,123,__LINE__, 0x41c3a6e1, 0xa0000000, 0x4034ffff, 0xfffffffd},			/* 6.59408e+08=f(21.0000)*/
{ 7, 0,123,__LINE__, 0x41eae4b4, 0x20000000, 0x4036b333, 0x33333330},			/* 3.60957e+09=f(22.7000)*/
{ 6, 0,123,__LINE__, 0x421266cf, 0xe0000000, 0x40386666, 0x66666663},			/* 1.97586e+10=f(24.4000)*/
{ 6, 0,123,__LINE__, 0x42392ead, 0x00000000, 0x403a1999, 0x99999996},			/* 1.08157e+11=f(26.1000)*/
{ 6, 0,123,__LINE__, 0x42613b16, 0xc0000000, 0x403bcccc, 0xccccccc9},			/* 5.92046e+11=f(27.8000)*/
{ 6, 0,123,__LINE__, 0x42879486, 0x60000000, 0x403d7fff, 0xfffffffc},			/* 3.24083e+12=f(29.5000)*/
0,};
test_coshf(m)   {run_vector_1(m,coshf_vec,(char *)(coshf),"coshf","ff");   }
