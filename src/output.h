#ifndef OUTPUT_H
#define OUTPUT_H
/*
* **** ALL RESULTS ARE VERIFIED ****
//---------------------------------------------------------------------
// Output file from testing sample batch file
//---------------------------------------------------------------------

//****************************************************************************
//		CREATE AND INSERT
//****************************************************************************

//---- employee table ----------
[0] make table employee fields  last, 		first, 			dep
Table Created: employee


SQL: DONE.

[1] insert into employee values Blow, 		Joe, 			CS
SQL::run: inserted into table: employee


SQL: DONE.

[2] insert into employee values Johnson, 	"Jimmy", 		Chemistry
SQL::run: inserted into table: employee


SQL: DONE.

[3] insert into employee values Yang, 		Bo, 			CS
SQL::run: inserted into table: employee


SQL: DONE.

[4] insert into employee values "Jackson",	Billy, 			Math
SQL::run: inserted into table: employee


SQL: DONE.

[5] insert into employee values Johnson,	Billy, 			"Phys Ed"
SQL::run: inserted into table: employee


SQL: DONE.

[6] insert into employee values "Van Gogh",	"Jim Bob", 		"Phys Ed"
SQL::run: inserted into table: employee


SQL: DONE.


[7] select * from employee

Table name: employee_temp_184, records: 6
record           last          first            dep

     0           Blow            Joe             CS
     1        Johnson          Jimmy      Chemistry
     2           Yang             Bo             CS
     3        Jackson          Billy           Math
     4        Johnson          Billy        Phys Ed
     5       Van Gogh        Jim Bob        Phys Ed


SQL: DONE.


//----- student table ----------
[8] make table student fields 	fname, 			lname, 		major, 				age
Table Created: student


SQL: DONE.

[9] insert into student values 	Flo, 			Yao, 		CS, 				20
SQL::run: inserted into table: student


SQL: DONE.

[10] insert into student values 	"Flo", 			"Jackson", 	Math,	 			21
SQL::run: inserted into table: student


SQL: DONE.

[11] insert into student values 	Calvin, 		Woo, 		Physics,			22
SQL::run: inserted into table: student


SQL: DONE.

[12] insert into student values 	"Anna Grace", 	"Del Rio", 	CS,	 				22
SQL::run: inserted into table: student


SQL: DONE.

[13] select * from student

Table name: student_temp_175, records: 4
record          fname          lname          major            age

     0            Flo            Yao             CS             20
     1            Flo        Jackson           Math             21
     2         Calvin            Woo        Physics             22
     3     Anna Grace        Del Rio             CS             22


SQL: DONE.


//****************************************************************************
//		SIMPLE SELECT
//****************************************************************************

[14] select * from student

Table name: student_temp_186, records: 4
record          fname          lname          major            age

     0            Flo            Yao             CS             20
     1            Flo        Jackson           Math             21
     2         Calvin            Woo        Physics             22
     3     Anna Grace        Del Rio             CS             22


SQL: DONE.


//------- simple strings -------------------
[15] select * from student where lname = Jackson

Table name: student_temp_24, records: 1
record          fname          lname          major            age

     0            Flo        Jackson           Math             21


SQL: DONE.


//----- quoted strings ---------------------
[16] select * from student where lname = "Del Rio"

Table name: student_temp_1, records: 1
record          fname          lname          major            age

     0     Anna Grace        Del Rio             CS             22


SQL: DONE.


//-------- non-existing string ------------------
[17] select * from student where lname = "Does Not Exist"

Table name: student_temp_102, records: 0
record          fname          lname          major            age



SQL: DONE.


//****************************************************************************
//		RELATIONAL OPERATORS:
//****************************************************************************

//.................
//:Greater Than   :
//.................
[18] select * from student where lname > Yang

Table name: student_temp_75, records: 1
record          fname          lname          major            age

     0            Flo            Yao             CS             20


SQL: DONE.



//. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
[19] select * from student where age > 50

Table name: student_temp_190, records: 0
record          fname          lname          major            age



SQL: DONE.


//. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .
[20] select * from student where age  > 53

Table name: student_temp_179, records: 0
record          fname          lname          major            age



SQL: DONE.


//. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .
[21] select * from student where age > 54

Table name: student_temp_147, records: 0
record          fname          lname          major            age



SQL: DONE.


//.................
//:Greater Equal  :
//.................
[22] select * from student where lname >= Yang

Table name: student_temp_176, records: 1
record          fname          lname          major            age

     0            Flo            Yao             CS             20


SQL: DONE.

//. . . . . .  (Greater Equal non-existing: ) . . . . . . . . . . .
[23] select * from employee where last >= Jack

Table name: employee_temp_3, records: 5
record           last          first            dep

     0        Jackson          Billy           Math
     1        Johnson          Jimmy      Chemistry
     2        Johnson          Billy        Phys Ed
     3       Van Gogh        Jim Bob        Phys Ed
     4           Yang             Bo             CS


SQL: DONE.



//.................
//:Less Than      :
//.................


//. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
[24] select * from student where lname < Jackson

Table name: student_temp_195, records: 1
record          fname          lname          major            age

     0     Anna Grace        Del Rio             CS             22


SQL: DONE.


//. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .
[25] select * from student where age  < 20

Table name: student_temp_34, records: 0
record          fname          lname          major            age



SQL: DONE.



//. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .

[26] select * from student where age  < 19

Table name: student_temp_4, records: 0
record          fname          lname          major            age



SQL: DONE.



//.................
//:Less Equal     :
//.................

[27] select * from student where lname <= Smith

Table name: student_temp_56, records: 2
record          fname          lname          major            age

     0     Anna Grace        Del Rio             CS             22
     1            Flo        Jackson           Math             21


SQL: DONE.


//. . . . . .  (Less Equal non-existing: ) . . . . . . . . . . .
[28] select * from employee where last <= Peach

Table name: employee_temp_21, records: 4
record           last          first            dep

     0           Blow            Joe             CS
     1        Jackson          Billy           Math
     2        Johnson          Jimmy      Chemistry
     3        Johnson          Billy        Phys Ed


SQL: DONE.


//****************************************************************************
//		LOGICAL OPERATORS
//****************************************************************************


//.................
//:AND            :
//.................

[29] select * from student where fname = "Flo" and lname = "Yao"

Table name: student_temp_69, records: 1
record          fname          lname          major            age

     0            Flo            Yao             CS             20


SQL: DONE.



//.................
//:OR            :
//.................
[30] select * from student where fname = Flo or lname = Jackson

Table name: student_temp_100, records: 2
record          fname          lname          major            age

     0            Flo            Yao             CS             20
     1            Flo        Jackson           Math             21


SQL: DONE.



//.................
//:OR AND         :
//.................
[31] select * from student where fname = Flo or major = CS and age <= 23

Table name: student_temp_166, records: 3
record          fname          lname          major            age

     0            Flo            Yao             CS             20
     1            Flo        Jackson           Math             21
     2     Anna Grace        Del Rio             CS             22


SQL: DONE.



//.................
//:AND OR AND     :
//.................

[32] select * from student where age <30 and major=CS or major = Physics and lname = Jackson

Table name: student_temp_92, records: 2
record          fname          lname          major            age

     0            Flo            Yao             CS             20
     1     Anna Grace        Del Rio             CS             22


SQL: DONE.



//.................
//:OR AND OR      :
//.................

[33] select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson

Table name: student_temp_36, records: 3
record          fname          lname          major            age

     0            Flo            Yao             CS             20
     1            Flo        Jackson           Math             21
     2     Anna Grace        Del Rio             CS             22


SQL: DONE.

---------------------------
End of Batch Process












//---------------------------------------------------------------------
// Output file from testing batch "Pokemon"
//---------------------------------------------------------------------




//Making pokemon table
[0] make table pokemon fields name, type
Table Created: pokemon


SQL: DONE.


//Inserting 151 pokemon into table
[1] insert into pokemon values Bulbasaur, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[2] insert into pokemon values Ivysaur, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[3] insert into pokemon values Venasaur, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[4] insert into pokemon values Charmander, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[5] insert into pokemon values Charmeleon, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[6] insert into pokemon values Charizard, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[7] insert into pokemon values Squirtle, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[8] insert into pokemon values Wartotle, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[9] insert into pokemon values Blastoise, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[10] insert into pokemon values Caterpie, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[11] insert into pokemon values Metapod, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[12] insert into pokemon values Butterfree, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[13] insert into pokemon values Weedle, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[14] insert into pokemon values Kakuna, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[15] insert into pokemon values Beedrill, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[16] insert into pokemon values Pidgey, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[17] insert into pokemon values Pidgeotto, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[18] insert into pokemon values Pidgeot, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[19] insert into pokemon values Rattata, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[20] insert into pokemon values Raticate, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[21] insert into pokemon values Spearow, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[22] insert into pokemon values Fearow, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[23] insert into pokemon values Ekans, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[24] insert into pokemon values Arbok, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[25] insert into pokemon values Pikachu, electric
SQL::run: inserted into table: pokemon


SQL: DONE.

[26] insert into pokemon values Raichu, electric
SQL::run: inserted into table: pokemon


SQL: DONE.

[27] insert into pokemon values Sandshrew, ground
SQL::run: inserted into table: pokemon


SQL: DONE.

[28] insert into pokemon values Sandslash, ground
SQL::run: inserted into table: pokemon


SQL: DONE.

[29] insert into pokemon values Nidoran, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[30] insert into pokemon values Nidorina, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[31] insert into pokemon values Nidoqueen, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[32] insert into pokemon values Nidoran, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[33] insert into pokemon values Nidorino, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[34] insert into pokemon values Nidoking, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[35] insert into pokemon values Clefairy, fairy
SQL::run: inserted into table: pokemon


SQL: DONE.

[36] insert into pokemon values Clefable, fairy
SQL::run: inserted into table: pokemon


SQL: DONE.

[37] insert into pokemon values Vulpix, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[38] insert into pokemon values Ninetales, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[39] insert into pokemon values Jigglypuff, fairy
SQL::run: inserted into table: pokemon


SQL: DONE.

[40] insert into pokemon values Wigglytuff, fairy
SQL::run: inserted into table: pokemon


SQL: DONE.

[41] insert into pokemon values Zubat, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[42] insert into pokemon values Golbat, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[43] insert into pokemon values Oddish, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[44] insert into pokemon values Gloom, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[45] insert into pokemon values Vileplume, Grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[46] insert into pokemon values Paras, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[47] insert into pokemon values Parasect, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[48] insert into pokemon values Venonat, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[49] insert into pokemon values Venomoth, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[50] insert into pokemon values Diglett, ground
SQL::run: inserted into table: pokemon


SQL: DONE.

[51] insert into pokemon values Dugtrio, ground
SQL::run: inserted into table: pokemon


SQL: DONE.

[52] insert into pokemon values Meowth, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[53] insert into pokemon values Persian, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[54] insert into pokemon values Psyduck, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[55] insert into pokemon values Golduck, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[56] insert into pokemon values Mankey, fighting
SQL::run: inserted into table: pokemon


SQL: DONE.

[57] insert into pokemon values Primeape, fighting
SQL::run: inserted into table: pokemon


SQL: DONE.

[58] insert into pokemon values Growlith, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[59] insert into pokemon values Arcanine, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[60] insert into pokemon values Polywag, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[61] insert into pokemon values Polywhirl, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[62] insert into pokemon values Polywrath, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[63] insert into pokemon values Abra, psychic
SQL::run: inserted into table: pokemon


SQL: DONE.

[64] insert into pokemon values Kadabra, psychic
SQL::run: inserted into table: pokemon


SQL: DONE.

[65] insert into pokemon values Alakazam, psychic
SQL::run: inserted into table: pokemon


SQL: DONE.

[66] insert into pokemon values Machop, fighting
SQL::run: inserted into table: pokemon


SQL: DONE.

[67] insert into pokemon values Machoke, fighting
SQL::run: inserted into table: pokemon


SQL: DONE.

[68] insert into pokemon values Machamp, fighting
SQL::run: inserted into table: pokemon


SQL: DONE.

[69] insert into pokemon values Bellsprout, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[70] insert into pokemon values Weepinbell, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[71] insert into pokemon values Victreebel, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[72] insert into pokemon values Tentacool, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[73] insert into pokemon values Tantacruel, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[74] insert into pokemon values Geodude, rock
SQL::run: inserted into table: pokemon


SQL: DONE.

[75] insert into pokemon values Graveler, rock
SQL::run: inserted into table: pokemon


SQL: DONE.

[76] insert into pokemon values Golem, rock
SQL::run: inserted into table: pokemon


SQL: DONE.

[77] insert into pokemon values Ponyta, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[78] insert into pokemon values Rapidash, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[79] insert into pokemon values Slowpoke, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[80] insert into pokemon values Slowbro, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[81] insert into pokemon values Magnemite, electric
SQL::run: inserted into table: pokemon


SQL: DONE.

[82] insert into pokemon values Magneton, electric
SQL::run: inserted into table: pokemon


SQL: DONE.

[83] insert into pokemon values "Farfetch'd", flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[84] insert into pokemon values Doduo, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[85] insert into pokemon values Dodrio, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[86] insert into pokemon values Seel, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[87] insert into pokemon values Dewgong, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[88] insert into pokemon values Grimer, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[89] insert into pokemon values Muk, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[90] insert into pokemon values Shelder, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[91] insert into pokemon values Cloyster, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[92] insert into pokemon values Gastly, ghost
SQL::run: inserted into table: pokemon


SQL: DONE.

[93] insert into pokemon values Haunter, ghost
SQL::run: inserted into table: pokemon


SQL: DONE.

[94] insert into pokemon values Gengar, ghost
SQL::run: inserted into table: pokemon


SQL: DONE.

[95] insert into pokemon values Onix, rock
SQL::run: inserted into table: pokemon


SQL: DONE.

[96] insert into pokemon values Drowzee, psychic
SQL::run: inserted into table: pokemon


SQL: DONE.

[97] insert into pokemon values Hypno, psychic
SQL::run: inserted into table: pokemon


SQL: DONE.

[98] insert into pokemon values Krabby, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[99] insert into pokemon values Kingler, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[100] insert into pokemon values Voltorb, electric
SQL::run: inserted into table: pokemon


SQL: DONE.

[101] insert into pokemon values Electrode, electric
SQL::run: inserted into table: pokemon


SQL: DONE.

[102] insert into pokemon values Exeggcute, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[103] insert into pokemon values Exeggutor, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[104] insert into pokemon values Cubone, ground
SQL::run: inserted into table: pokemon


SQL: DONE.

[105] insert into pokemon values Marowak, ground
SQL::run: inserted into table: pokemon


SQL: DONE.

[106] insert into pokemon values Hitmonlee, fighting
SQL::run: inserted into table: pokemon


SQL: DONE.

[107] insert into pokemon values Hitmonchan, fighting
SQL::run: inserted into table: pokemon


SQL: DONE.

[108] insert into pokemon values Lickitung, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[109] insert into pokemon values Koffing, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[110] insert into pokemon values Weezing, poison
SQL::run: inserted into table: pokemon


SQL: DONE.

[111] insert into pokemon values Rhyhorn, ground
SQL::run: inserted into table: pokemon


SQL: DONE.

[112] insert into pokemon values Rhydon, ground
SQL::run: inserted into table: pokemon


SQL: DONE.

[113] insert into pokemon values Chansey, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[114] insert into pokemon values Tangela, grass
SQL::run: inserted into table: pokemon


SQL: DONE.

[115] insert into pokemon values Kangaskhan, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[116] insert into pokemon values Horsea, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[117] insert into pokemon values Seadra, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[118] insert into pokemon values Goldeen, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[119] insert into pokemon values Seaking, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[120] insert into pokemon values Staryu, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[121] insert into pokemon values Starmie, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[122] insert into pokemon values "Mr. Mime", psychic
SQL::run: inserted into table: pokemon


SQL: DONE.

[123] insert into pokemon values Scyther, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[124] insert into pokemon values Jynx, psychic
SQL::run: inserted into table: pokemon


SQL: DONE.

[125] insert into pokemon values Electabuzz, electric
SQL::run: inserted into table: pokemon


SQL: DONE.

[126] insert into pokemon values Magmar, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[127] insert into pokemon values Pinsir, bug
SQL::run: inserted into table: pokemon


SQL: DONE.

[128] insert into pokemon values Tauros, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[129] insert into pokemon values Magikarp, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[130] insert into pokemon values Gyarados, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[131] insert into pokemon values Lapras, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[132] insert into pokemon values Ditto, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[133] insert into pokemon values Eevee, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[134] insert into pokemon values Vaporeon, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[135] insert into pokemon values Jolteon, electric
SQL::run: inserted into table: pokemon


SQL: DONE.

[136] insert into pokemon values Flareon, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[137] insert into pokemon values Porygon, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[138] insert into pokemon values Omanyte, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[139] insert into pokemon values Omastar, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[140] insert into pokemon values Kabuto, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[141] insert into pokemon values Kabutops, water
SQL::run: inserted into table: pokemon


SQL: DONE.

[142] insert into pokemon values Aerodactyl, flying
SQL::run: inserted into table: pokemon


SQL: DONE.

[143] insert into pokemon values Snorlax, normal
SQL::run: inserted into table: pokemon


SQL: DONE.

[144] insert into pokemon values Articuno, ice
SQL::run: inserted into table: pokemon


SQL: DONE.

[145] insert into pokemon values Zapdos, electric
SQL::run: inserted into table: pokemon


SQL: DONE.

[146] insert into pokemon values Moltres, fire
SQL::run: inserted into table: pokemon


SQL: DONE.

[147] insert into pokemon values Dratini, dragon
SQL::run: inserted into table: pokemon


SQL: DONE.

[148] insert into pokemon values Dragonair, dragon
SQL::run: inserted into table: pokemon


SQL: DONE.

[149] insert into pokemon values Dragonite, dragon
SQL::run: inserted into table: pokemon


SQL: DONE.

[150] insert into pokemon values Mewtwo, psychic
SQL::run: inserted into table: pokemon


SQL: DONE.

[151] insert into pokemon values Mew, psychic
SQL::run: inserted into table: pokemon


SQL: DONE.



//Testing Select
[152] select * from pokemon

Table name: pokemon_temp_90, records: 151
record           name           type

     0      Bulbasaur          grass
     1        Ivysaur          grass
     2       Venasaur          grass
     3     Charmander           fire
     4     Charmeleon           fire
     5      Charizard           fire
     6       Squirtle          water
     7       Wartotle          water
     8      Blastoise          water
     9       Caterpie            bug
    10        Metapod            bug
    11     Butterfree            bug
    12         Weedle            bug
    13         Kakuna            bug
    14       Beedrill            bug
    15         Pidgey         flying
    16      Pidgeotto         flying
    17        Pidgeot         flying
    18        Rattata         normal
    19       Raticate         normal
    20        Spearow         flying
    21         Fearow         flying
    22          Ekans         poison
    23          Arbok         poison
    24        Pikachu       electric
    25         Raichu       electric
    26      Sandshrew         ground
    27      Sandslash         ground
    28        Nidoran         poison
    29       Nidorina         poison
    30      Nidoqueen         poison
    31        Nidoran         poison
    32       Nidorino         poison
    33       Nidoking         poison
    34       Clefairy          fairy
    35       Clefable          fairy
    36         Vulpix           fire
    37      Ninetales           fire
    38     Jigglypuff          fairy
    39     Wigglytuff          fairy
    40          Zubat         flying
    41         Golbat         flying
    42         Oddish          grass
    43          Gloom          grass
    44      Vileplume          Grass
    45          Paras            bug
    46       Parasect            bug
    47        Venonat            bug
    48       Venomoth            bug
    49        Diglett         ground
    50        Dugtrio         ground
    51         Meowth         normal
    52        Persian         normal
    53        Psyduck          water
    54        Golduck          water
    55         Mankey       fighting
    56       Primeape       fighting
    57       Growlith           fire
    58       Arcanine           fire
    59        Polywag          water
    60      Polywhirl          water
    61      Polywrath          water
    62           Abra        psychic
    63        Kadabra        psychic
    64       Alakazam        psychic
    65         Machop       fighting
    66        Machoke       fighting
    67        Machamp       fighting
    68     Bellsprout          grass
    69     Weepinbell          grass
    70     Victreebel          grass
    71      Tentacool          water
    72     Tantacruel          water
    73        Geodude           rock
    74       Graveler           rock
    75          Golem           rock
    76         Ponyta           fire
    77       Rapidash           fire
    78       Slowpoke          water
    79        Slowbro          water
    80      Magnemite       electric
    81       Magneton       electric
    82     Farfetch'd         flying
    83          Doduo         flying
    84         Dodrio         flying
    85           Seel          water
    86        Dewgong          water
    87         Grimer         poison
    88            Muk         poison
    89        Shelder          water
    90       Cloyster          water
    91         Gastly          ghost
    92        Haunter          ghost
    93         Gengar          ghost
    94           Onix           rock
    95        Drowzee        psychic
    96          Hypno        psychic
    97         Krabby          water
    98        Kingler          water
    99        Voltorb       electric
   100      Electrode       electric
   101      Exeggcute          grass
   102      Exeggutor          grass
   103         Cubone         ground
   104        Marowak         ground
   105      Hitmonlee       fighting
   106     Hitmonchan       fighting
   107      Lickitung         normal
   108        Koffing         poison
   109        Weezing         poison
   110        Rhyhorn         ground
   111         Rhydon         ground
   112        Chansey         normal
   113        Tangela          grass
   114     Kangaskhan         normal
   115         Horsea          water
   116         Seadra          water
   117        Goldeen          water
   118        Seaking          water
   119         Staryu          water
   120        Starmie          water
   121       Mr. Mime        psychic
   122        Scyther            bug
   123           Jynx        psychic
   124     Electabuzz       electric
   125         Magmar           fire
   126         Pinsir            bug
   127         Tauros         normal
   128       Magikarp          water
   129       Gyarados          water
   130         Lapras          water
   131          Ditto         normal
   132          Eevee         normal
   133       Vaporeon          water
   134        Jolteon       electric
   135        Flareon           fire
   136        Porygon         normal
   137        Omanyte          water
   138        Omastar          water
   139         Kabuto          water
   140       Kabutops          water
   141     Aerodactyl         flying
   142        Snorlax         normal
   143       Articuno            ice
   144         Zapdos       electric
   145        Moltres           fire
   146        Dratini         dragon
   147      Dragonair         dragon
   148      Dragonite         dragon
   149         Mewtwo        psychic
   150            Mew        psychic


SQL: DONE.

[153] select * from pokemon where type = psychic or type = ghost

Table name: pokemon_temp_41, records: 12
record           name           type

     0           Abra        psychic
     1        Kadabra        psychic
     2       Alakazam        psychic
     3         Gastly          ghost
     4        Haunter          ghost
     5         Gengar          ghost
     6        Drowzee        psychic
     7          Hypno        psychic
     8       Mr. Mime        psychic
     9           Jynx        psychic
    10         Mewtwo        psychic
    11            Mew        psychic


SQL: DONE.

[154] select * from pokemon where name < Gengar

Table name: pokemon_temp_3, records: 40
record           name           type

     0           Abra        psychic
     1     Aerodactyl         flying
     2       Alakazam        psychic
     3          Arbok         poison
     4       Arcanine           fire
     5       Articuno            ice
     6       Beedrill            bug
     7     Bellsprout          grass
     8      Blastoise          water
     9      Bulbasaur          grass
    10     Butterfree            bug
    11       Caterpie            bug
    12        Chansey         normal
    13      Charizard           fire
    14     Charmander           fire
    15     Charmeleon           fire
    16       Clefable          fairy
    17       Clefairy          fairy
    18       Cloyster          water
    19         Cubone         ground
    20        Dewgong          water
    21        Diglett         ground
    22          Ditto         normal
    23         Dodrio         flying
    24          Doduo         flying
    25      Dragonair         dragon
    26      Dragonite         dragon
    27        Dratini         dragon
    28        Drowzee        psychic
    29        Dugtrio         ground
    30          Eevee         normal
    31          Ekans         poison
    32     Electabuzz       electric
    33      Electrode       electric
    34      Exeggcute          grass
    35      Exeggutor          grass
    36     Farfetch'd         flying
    37         Fearow         flying
    38        Flareon           fire
    39         Gastly          ghost


SQL: DONE.

[155] select * from pokemon where name > Dratini and name < Gastly

Table name: pokemon_temp_71, records: 11
record           name           type

     0         Fearow         flying
     1          Ekans         poison
     2        Dugtrio         ground
     3     Farfetch'd         flying
     4        Drowzee        psychic
     5      Electrode       electric
     6      Exeggcute          grass
     7      Exeggutor          grass
     8     Electabuzz       electric
     9          Eevee         normal
    10        Flareon           fire


SQL: DONE.

[156] select * from pokemon where type = fire and name < P or type = electric and name < P

Table name: pokemon_temp_83, records: 14
record           name           type

     0     Charmander           fire
     1     Charmeleon           fire
     2      Charizard           fire
     3      Ninetales           fire
     4       Growlith           fire
     5       Arcanine           fire
     6      Magnemite       electric
     7       Magneton       electric
     8      Electrode       electric
     9     Electabuzz       electric
    10         Magmar           fire
    11        Jolteon       electric
    12        Flareon           fire
    13        Moltres           fire


SQL: DONE.

[157] select * from pokemon where name <= Charmander or name >= Pidgey

Table name: pokemon_temp_61, records: 64
record           name           type

     0      Bulbasaur          grass
     1       Venasaur          grass
     2     Charmander           fire
     3      Charizard           fire
     4       Squirtle          water
     5       Wartotle          water
     6      Blastoise          water
     7       Caterpie            bug
     8     Butterfree            bug
     9         Weedle            bug
    10       Beedrill            bug
    11         Pidgey         flying
    12        Rattata         normal
    13       Raticate         normal
    14        Spearow         flying
    15          Arbok         poison
    16        Pikachu       electric
    17         Raichu       electric
    18      Sandshrew         ground
    19      Sandslash         ground
    20         Vulpix           fire
    21     Wigglytuff          fairy
    22          Zubat         flying
    23      Vileplume          Grass
    24        Venonat            bug
    25       Venomoth            bug
    26        Psyduck          water
    27       Primeape       fighting
    28       Arcanine           fire
    29        Polywag          water
    30      Polywhirl          water
    31      Polywrath          water
    32           Abra        psychic
    33       Alakazam        psychic
    34     Bellsprout          grass
    35     Weepinbell          grass
    36     Victreebel          grass
    37      Tentacool          water
    38     Tantacruel          water
    39         Ponyta           fire
    40       Rapidash           fire
    41       Slowpoke          water
    42        Slowbro          water
    43           Seel          water
    44        Shelder          water
    45        Voltorb       electric
    46        Weezing         poison
    47        Rhyhorn         ground
    48         Rhydon         ground
    49        Chansey         normal
    50        Tangela          grass
    51         Seadra          water
    52        Seaking          water
    53         Staryu          water
    54        Starmie          water
    55        Scyther            bug
    56         Pinsir            bug
    57         Tauros         normal
    58       Vaporeon          water
    59        Porygon         normal
    60     Aerodactyl         flying
    61        Snorlax         normal
    62       Articuno            ice
    63         Zapdos       electric


SQL: DONE.

[158] select * from pokemon where name <= Aaron

Table name: pokemon_temp_32, records: 0
record           name           type



SQL: DONE.

[159] select * from pokemon where name >= Zigzagoon

Table name: pokemon_temp_74, records: 1
record           name           type

     0          Zubat         flying


SQL: DONE.

[160] select * from pokemon where name <= Barkeshli or name >= Sassan

Table name: pokemon_temp_132, records: 37
record           name           type

     0       Venasaur          grass
     1       Squirtle          water
     2       Wartotle          water
     3         Weedle            bug
     4        Spearow         flying
     5          Arbok         poison
     6         Vulpix           fire
     7     Wigglytuff          fairy
     8          Zubat         flying
     9      Vileplume          Grass
    10        Venonat            bug
    11       Venomoth            bug
    12       Arcanine           fire
    13           Abra        psychic
    14       Alakazam        psychic
    15     Weepinbell          grass
    16     Victreebel          grass
    17      Tentacool          water
    18     Tantacruel          water
    19       Slowpoke          water
    20        Slowbro          water
    21           Seel          water
    22        Shelder          water
    23        Voltorb       electric
    24        Weezing         poison
    25        Tangela          grass
    26         Seadra          water
    27        Seaking          water
    28         Staryu          water
    29        Starmie          water
    30        Scyther            bug
    31         Tauros         normal
    32       Vaporeon          water
    33     Aerodactyl         flying
    34        Snorlax         normal
    35       Articuno            ice
    36         Zapdos       electric


SQL: DONE.



---------------------------
End of Batch Process
*/
#endif // OUTPUT_H
