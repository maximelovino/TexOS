#include "kernel.h"


void kernelEntry(multiboot_info_t* multibootInfos){
	gdt_init();
	display_init();
	/*
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 10; i++) {
			printString("LE JEU ");
		}
		for (int k = 0; k < 10e6; k++) {
		}
	}*/

	char* text = "It had always been like this. Lara knew no other life. She traveled back and forth, up and down the river path. No single place was home. She liked the seaside, where there was always fish to eat, and the gentle lapping of the waves lulled her to sleep at night. She was less fond of the foothills, where the path grew steep, the nights could be cold, and views of great distances made her dizzy. She felt uneasy in the villages, and was often shy around strangers. The path itself was where she felt most at home. She loved the smell of the river on a hot day, and the croaking of frogs at night. Vines grew amid the lush foliage along the river, with berries that were good to eat. Even on the hottest day, sundown brought a cool breeze off the water, which sighed and sang amid the reeds and tall grasses.The terrain along this stretch of the river was mostly flat, but in the immediate vicinity of the island, the land on the sunrise side was like a rumpled cloth, with hills and ridges and valleys. Among Laras people, there was a wooden babys crib, suitable for strapping to a cart, that had been passed down for generations. The island was shaped like that crib, longer than it was wide and pointed at the upriver end, where the flow had eroded both banks. The island was like a crib, and the group of hills on the sunrise side of the river were like old women mantled in heavy cloaks gathered to have a look at the baby in the crib that was how Laras father had once described the lay of the land.Tarketios and his two older companions were skilled metalworkers from a region some two hundred miles to the north, where the hills were rich with iron, copper, and lead. They had been on a trading journey to the south and were returning home. Just as the river path carried Larths people from the seashore to the hills, so another path, perpendicular to the river, traversed the long coastal plain. Because the island provided an easy place to ford the river, it was here that the two paths intersected. On this occasion, the salt traders and the metal traders happened to arrive at the island on the same day. Now they met for the first time.Fascinus was different. It was unique. It existed in and of itself, without beginning or end. Clearly, from its form, it had something to do with life and the origin of life, yet it seemed to come from a place beyond this world, slipping for a few moments through a breach opened by the heat of the dancing flames. An appearance by Fascinus was always significant. The winged phallus never appeared without giving Larth an answer to a dilemma that had been troubling him, or planting an important new thought in his mind. The guidance given to him by Fascinus had never led Larth astray.";
	//printString(text);
	display_print("%d %d",12345,200);
	while (1) {

	}
}
