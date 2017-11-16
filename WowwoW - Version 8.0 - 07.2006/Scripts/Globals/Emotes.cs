using System;

namespace Server
{
	public class Emotes : IInitialize
	{
		public static void Initialize()
		{
			//	Emote list
			World.Emote[223] = 0; // SCARED 
			World.Emote[224] = 0; // FLOP 
			World.Emote[226] = 0; // MOO 
			World.Emote[343] = 21; // GOLFCLAP 
			World.Emote[324] = 1; // FOLLOW 
			World.Emote[326] = 1; // HEALME 
			World.Emote[364] = 0; // PAT 
			World.Emote[243] = 21; // COMMEND 
			World.Emote[325] = 1; // WAIT 
			World.Emote[329] = 1; // JOKE 
			World.Emote[1] = 0; // AGREE 
			World.Emote[2] = 0; // AMAZE 
			World.Emote[3] = 14; // ANGRY 
			World.Emote[4] = 0; // APOLOGIZE 
			World.Emote[5] = 21; // APPLAUD 
			World.Emote[6] = 24; // BASHFUL 
			World.Emote[7] = 0; // BECKON 
			World.Emote[8] = 20; // BEG 
			World.Emote[9] = 0; // BITE 
			World.Emote[10] = 0; // BLEED 
			World.Emote[11] = 0; // BLINK 
			World.Emote[12] = 24; // BLUSH 
			World.Emote[13] = 0; // BONK 
			World.Emote[14] = 0; // BORED 
			World.Emote[15] = 0; // BOUNCE 
			World.Emote[16] = 0; // BRB 
			World.Emote[17] = 2; // BOW 
			World.Emote[18] = 0; // BURP 
			World.Emote[19] = 3; // BYE 
			World.Emote[20] = 11; // CACKLE 
			World.Emote[21] = 4; // CHEER 
			World.Emote[22] = 19; // CHICKEN 
			World.Emote[23] = 11; // CHUCKLE 
			World.Emote[24] = 21; // CLAP 
			World.Emote[25] = 6; // CONFUSED 
			World.Emote[26] = 5; // CONGRATULATE 
			World.Emote[27] = 0; // COUGH 
			World.Emote[28] = 0; // COWER 
			World.Emote[29] = 0; // CRACK 
			World.Emote[30] = 0; // CRINGE 
			World.Emote[31] = 18; // CRY 
			World.Emote[32] = 6; // CURIOUS 
			World.Emote[33] = 2; // CURTSEY 
			World.Emote[34] = 10; // DANCE 
			World.Emote[35] = 7; // DRINK 
			World.Emote[36] = 0; // DROOL 
			World.Emote[37] = 7; // EAT 
			World.Emote[38] = 0; // EYE 
			World.Emote[39] = 0; // FART 
			World.Emote[40] = 0; // FIDGET 
			World.Emote[41] = 23; // FLEX 
			World.Emote[42] = 0; // FROWN 
			World.Emote[43] = 5; // GASP 
			World.Emote[44] = 0; // GAZE 
			World.Emote[45] = 11; // GIGGLE 
			World.Emote[46] = 0; // GLARE 
			World.Emote[47] = 11; // GLOAT 
			World.Emote[48] = 3; // GREET 
			World.Emote[49] = 0; // GRIN 
			World.Emote[50] = 0; // GROAN 
			World.Emote[51] = 20; // GROVEL 
			World.Emote[52] = 11; // GUFFAW 
			World.Emote[53] = 3; // HAIL 
			World.Emote[54] = 0; // HAPPY 
			World.Emote[55] = 3; // HELLO 
			World.Emote[56] = 0; // HUG 
			World.Emote[57] = 0; // HUNGRY 
			World.Emote[58] = 17; // KISS 
			World.Emote[59] = 68; // KNEEL 
			World.Emote[60] = 11; // LAUGH 
			World.Emote[61] = 12; // LAYDOWN 
			World.Emote[62] = 0; // MASSAGE 
			World.Emote[63] = 0; // MOAN 
			World.Emote[64] = 0; // MOON 
			World.Emote[65] = 18; // MOURN 
			World.Emote[66] = 274; // NO 
			World.Emote[67] = 273; // NOD 
			World.Emote[68] = 0; // NOSEPICK 
			World.Emote[69] = 0; // PANIC 
			World.Emote[70] = 0; // PEER 
			World.Emote[71] = 20; // PLEAD 
			World.Emote[72] = 25; // POINT 
			World.Emote[73] = 0; // POKE 
			World.Emote[74] = 16; // PRAY 
			World.Emote[75] = 15; // ROAR 
			World.Emote[76] = 11; // ROFL 
			World.Emote[77] = 14; // RUDE 
			World.Emote[78] = 66; // SALUTE 
			World.Emote[79] = 0; // SCRATCH 
			World.Emote[80] = 0; // SEXY 
			World.Emote[81] = 0; // SHAKE 
			World.Emote[82] = 22; // SHOUT 
			World.Emote[83] = 6; // SHRUG 
			World.Emote[84] = 24; // SHY 
			World.Emote[85] = 0; // SIGH 
			World.Emote[86] = 13; // SIT 
			World.Emote[87] = 12; // SLEEP 
			World.Emote[88] = 0; // SNARL 
			World.Emote[89] = 0; // SPIT 
			World.Emote[90] = 0; // STARE 
			World.Emote[91] = 0; // SURPRISED 
			World.Emote[92] = 20; // SURRENDER 
			World.Emote[93] = 1; // TALK 
			World.Emote[94] = 5; // TALKEX 
			World.Emote[95] = 6; // TALKQ 
			World.Emote[96] = 0; // TAP 
			World.Emote[97] = 1; // THANK 
			World.Emote[98] = 0; // THREATEN 
			World.Emote[99] = 0; // TIRED 
			World.Emote[100] = 4; // VICTORY 
			World.Emote[101] = 3; // WAVE 
			World.Emote[102] = 1; // WELCOME 
			World.Emote[103] = 0; // WHINE 
			World.Emote[104] = 0; // WHISTLE 
			World.Emote[105] = 0; // WORK 
			World.Emote[106] = 0; // YAWN 
			World.Emote[107] = 6; // BOGGLE 
			World.Emote[108] = 0; // CALM 
			World.Emote[109] = 0; // COLD 
			World.Emote[110] = 0; // COMFORT 
			World.Emote[111] = 0; // CUDDLE 
			World.Emote[112] = 0; // DUCK 
			World.Emote[113] = 14; // INSULT 
			World.Emote[114] = 0; // INTRODUCE 
			World.Emote[115] = 0; // JK 
			World.Emote[116] = 0; // LICK 
			World.Emote[117] = 0; // LISTEN 
			World.Emote[118] = 6; // LOST 
			World.Emote[119] = 0; // MOCK 
			World.Emote[120] = 6; // PONDER 
			World.Emote[121] = 0; // POUNCE 
			World.Emote[122] = 0; // PRAISE 
			World.Emote[123] = 0; // PURR 
			World.Emote[124] = 6; // PUZZLE 
			World.Emote[125] = 0; // RAISE 
			World.Emote[126] = 0; // READY 
			World.Emote[127] = 0; // SHIMMY 
			World.Emote[128] = 0; // SHIVER 
			World.Emote[129] = 0; // SHOO 
			World.Emote[130] = 0; // SLAP 
			World.Emote[131] = 0; // SMIRK 
			World.Emote[132] = 0; // SNIFF 
			World.Emote[133] = 0; // SNUB 
			World.Emote[134] = 0; // SOOTHE 
			World.Emote[135] = 0; // STINK 
			World.Emote[136] = 19; // TAUNT 
			World.Emote[137] = 0; // TEASE 
			World.Emote[138] = 0; // THIRSTY 
			World.Emote[139] = 0; // VETO 
			World.Emote[140] = 0; // SNICKER 
			World.Emote[141] = 26; // STAND 
			World.Emote[142] = 0; // TICKLE 
			World.Emote[143] = 18; // VIOLIN 
			World.Emote[163] = 0; // SMILE 
			World.Emote[183] = 14; // RASP 
			World.Emote[203] = 0; // PITY 
			World.Emote[204] = 15; // GROWL 
			World.Emote[205] = 0; // BARK 
			World.Emote[225] = 0; // LOVE 
			World.Emote[264] = 275; // TRAIN 
			World.Emote[303] = 22; // HELPME 
			World.Emote[304] = 22; // INCOMING 
			World.Emote[305] = 25; // CHARGE 
			World.Emote[306] = 22; // FLEE 
			World.Emote[307] = 15; // ATTACKMYTARGET 
			World.Emote[323] = 1; // OOM 
			World.Emote[327] = 25; // OPENFIRE 
			World.Emote[328] = 24; // FLIRT 
			World.Emote[363] = 0; // WINK 						
		}
	}
}